////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DataBindingUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataObject.h"
#include "Gugu/Data/DatasheetObject.h"
#include "Gugu/Data/DatasaveObject.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

namespace binding {

namespace impl {

pugi::xml_node FindNodeData(DataParseContext& _kContext, const std::string& _strName)
{
    return _kContext.currentNode->find_child_by_attribute("Data", "name", _strName.c_str());
}

pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName)
{
    pugi::xml_node nodeData = _kContext.currentNode->append_child("Data");
    nodeData.append_attribute("name").set_value(_strName.c_str());
    return nodeData;
}

bool ReadEnumValue(DataParseContext& context, const std::string& name, const std::string& enumTypeName, int& value)
{
    if (const DataEnumInfos* enumInfos = GetResources()->GetDataEnumInfos(enumTypeName))
    {
        if (pugi::xml_node node = FindNodeData(context, name))
        {
            std::string enumValue = node.attribute("value").as_string("");
            for (size_t i = 0; i < enumInfos->values.size(); ++i)
            {
                if (enumInfos->values[i] == enumValue)
                {
                    value = (int)i;
                    return true;
                }
            }
        }
    }

    return false;
}

bool ReadEnumValues(DataParseContext& context, const std::string& name, const std::string& enumTypeName, std::vector<int>& values)
{
    if (const DataEnumInfos* enumInfos = GetResources()->GetDataEnumInfos(enumTypeName))
    {
        if (pugi::xml_node node = FindNodeData(context, name))
        {
            for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
            {
                std::string enumValue = child.attribute("value").as_string("");
                for (size_t i = 0; i < enumInfos->values.size(); ++i)
                {
                    if (enumInfos->values[i] == enumValue)
                    {
                        values.push_back((int)i);
                    }
                }
            }

            return true;
        }
    }

    return false;
}

void WriteEnumValue(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, int _iValue)
{
    const DataEnumInfos* pEnum = GetResources()->GetDataEnumInfos(_strType);
    if (pEnum && _iValue >= 0 && _iValue < pEnum->values.size())
    {
        impl::AddNodeData(_kContext, _strName).append_attribute("value").set_value(pEnum->values[_iValue].c_str());
    }
}

void WriteEnumValues(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<int>& _vecValues)
{
    const DataEnumInfos* pEnum = GetResources()->GetDataEnumInfos(_strType);
    if (pEnum)
    {
        pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

        for (size_t i = 0; i < _vecValues.size(); ++i)
        {
            int value = _vecValues[i];
            if (value >= 0 && value < pEnum->values.size())
            {
                pNode.append_child("Child").append_attribute("value").set_value(pEnum->values[value].c_str());
            }
        }
    }
}

const DatasheetObject* ResolveDatasheetLink(const std::string& _strName)
{
    Datasheet* datasheet = GetResources()->GetDatasheet(_strName);
    return datasheet ? datasheet->GetRootObject() : nullptr;
}

bool ResolveDatasheetLink(DataParseContext& _kContext, const std::string& _strName, const DatasheetObject*& _pNewDatasheet)
{
    if (pugi::xml_node pNode = FindNodeData(_kContext, _strName))
    {
        // Reference can be null.
        std::string datasheetID = pNode.attribute("value").as_string();
        if (datasheetID != "")
        {
            std::string strSheetName = datasheetID;
            _pNewDatasheet = ResolveDatasheetLink(strSheetName);
        }

        return true;
    }

    return false;
}

bool ResolveDatasheetLinks(DataParseContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecReferences)
{
    if (pugi::xml_node node = FindNodeData(_kContext, _strName))
    {
        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            // Reference can be null.
            std::string datasheetID = child.attribute("value").as_string();
            if (datasheetID != "")
            {
                const DatasheetObject* reference = ResolveDatasheetLink(datasheetID);
                _vecReferences.push_back(reference);
            }
            else
            {
                _vecReferences.push_back(nullptr);
            }
        }

        return true;
    }

    return false;
}

DataObject* InstanciateDataObject(DataParseContext& _kContext, const std::string& _strType)
{
    DataObject* instance = GetResources()->InstanciateDataObject(_strType);
    if (instance)
    {
        instance->ParseMembers(_kContext);
        return instance;
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Could not instantiate Datasheet : {0}", _strType));
    }

    return nullptr;
}

bool InstanciateDataObject(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, DataObject*& _pInstance)
{
    if (pugi::xml_node pNode = FindNodeData(_kContext, _strName))
    {
        // Check type overload.
        // If serialized type is explicitely empty, we force a null instance (can be used to erase a parent instance through inheritance).
        // If type is missing, no object will be instanced here, and we will keep the inherited object from parent instance.
        pugi::xml_attribute pAttributeType = pNode.attribute("type");
        std::string strType = (pAttributeType) ? pAttributeType.as_string() : _strDefaultType;

        if (strType != "")
        {
            pugi::xml_node* pNodeParent = _kContext.currentNode;
            _kContext.currentNode = &pNode;

            _pInstance = InstanciateDataObject(_kContext, strType);

            _kContext.currentNode = pNodeParent;
        }

        return true;
    }

    return false;
}

bool InstanciateDataObjects(DataParseContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<DataObject*>& _vecInstances)
{
    if (pugi::xml_node node = FindNodeData(_kContext, _strName))
    {
        pugi::xml_node* pNodeParent = _kContext.currentNode;

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            // Check type overload.
            // If serialized type is explicitely empty, we force a null instance (can be used for explicit empty array entries).
            // If type is missing, we will also force a null instance (this may evolve later if I handle inherited arrays override per entry).
            pugi::xml_attribute pAttributeType = child.attribute("type");
            std::string strType = (pAttributeType) ? pAttributeType.as_string() : _strDefaultType;

            if (strType != "")
            {
                // Instance can be null.
                _kContext.currentNode = &child;
                DataObject* pInstance = InstanciateDataObject(_kContext, strType);

                _vecInstances.push_back(pInstance);
            }
            else
            {
                _vecInstances.push_back(nullptr);
            }
        }

        _kContext.currentNode = pNodeParent;

        return true;
    }

    return false;
}

void WriteDatasheetReferences(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const DatasheetObject*>& _pMember)
{
    pugi::xml_node pNode = AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        const Datasheet* datasheet = _pMember[i] == nullptr ? nullptr : _pMember[i]->GetDatasheet();
        if (datasheet)
        {
            pNode.append_child("Child").append_attribute("value").set_value(datasheet->GetID().c_str());
        }
        else
        {
            pNode.append_child("Child").append_attribute("value");
        }
    }
}

void WriteDatasaveInstances(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<DatasaveObject*>& _pMember)
{
    pugi::xml_node* pNodeParent = _kContext.currentNode;
    pugi::xml_node pNode = AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _pMember.size(); ++i)
    {
        pugi::xml_node childNode = pNode.append_child("Child");

        if (_pMember[i] != nullptr)
        {
            childNode.append_attribute("type").set_value(_pMember[i]->GetDataInstanceType().c_str());

            _kContext.currentNode = &childNode;
            _pMember[i]->SerializeMembers(_kContext);
        }
        else
        {
            childNode.append_attribute("type");
        }
    }

    _kContext.currentNode = pNodeParent;
}

}   // namespace impl

void ReadString(DataParseContext& context, const std::string& name, std::string& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        value = node.attribute("value").as_string(value.c_str());
    }
}

void ReadInt(DataParseContext& context, const std::string& name, int& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        value = node.attribute("value").as_int(value);
    }
}

void ReadFloat(DataParseContext& context, const std::string& name, float& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        value = node.attribute("value").as_float(value);
    }
}

void ReadBool(DataParseContext& context, const std::string& name, bool& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        value = node.attribute("value").as_bool(value);
    }
}

void ReadStringArray(DataParseContext& context, const std::string& name, std::vector<std::string>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();     //TODO: find a way to allow append instead of clear on arrays inheritance

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(child.attribute("value").as_string(""));
        }
    }
}

void ReadIntArray(DataParseContext& context, const std::string& name, std::vector<int>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(child.attribute("value").as_int(0));
        }
    }
}

void ReadFloatArray(DataParseContext& context, const std::string& name, std::vector<float>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(child.attribute("value").as_float(0.f));
        }
    }
}

void ReadBoolArray(DataParseContext& context, const std::string& name, std::vector<bool>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(child.attribute("value").as_bool(false));
        }
    }
}

void WriteString(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strMember)
{
    impl::AddNodeData(_kContext, _strName).append_attribute("value").set_value(_strMember.c_str());
}

void WriteInt(DataSaveContext& _kContext, const std::string& _strName, int _iMember)
{
    impl::AddNodeData(_kContext, _strName).append_attribute("value").set_value(_iMember);
}

void WriteFloat(DataSaveContext& _kContext, const std::string& _strName, float _fMember)
{
    impl::AddNodeData(_kContext, _strName).append_attribute("value").set_value(_fMember);
}

void WriteBool(DataSaveContext& _kContext, const std::string& _strName, bool _bMember)
{
    impl::AddNodeData(_kContext, _strName).append_attribute("value").set_value(_bMember);
}

void WriteStringArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<std::string>& _vecMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i].c_str());
    }
}

void WriteIntArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<int>& _vecMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void WriteFloatArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<float>& _vecMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void WriteBoolArray(DataSaveContext& _kContext, const std::string& _strName, const std::vector<bool>& _vecMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void ReadVector2(DataParseContext& context, const std::string& name, Vector2i& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        xml::ParseVector2i(node, value, value);
    }
}

void ReadVector2(DataParseContext& context, const std::string& name, Vector2f& value)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        xml::ParseVector2f(node, value, value);
    }
}

void ReadVector2Array(DataParseContext& context, const std::string& name, std::vector<sf::Vector2i>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(xml::ReadVector2i(child, Vector2::Zero_i));
        }
    }
}

void ReadVector2Array(DataParseContext& context, const std::string& name, std::vector<sf::Vector2f>& values)
{
    if (pugi::xml_node node = impl::FindNodeData(context, name))
    {
        values.clear();

        for (pugi::xml_node child = node.child("Child"); child; child = child.next_sibling("Child"))
        {
            values.push_back(xml::ReadVector2f(child, Vector2::Zero_f));
        }
    }
}

void WriteVector2(DataSaveContext& context, const std::string& name, Vector2i& value)
{
    xml::WriteVector2i(impl::AddNodeData(context, name), value);
}

void WriteVector2(DataSaveContext& context, const std::string& name, Vector2f& value)
{
    xml::WriteVector2f(impl::AddNodeData(context, name), value);
}

void WriteVector2Array(DataSaveContext& context, const std::string& name, const std::vector<Vector2i>& values)
{
    pugi::xml_node node = impl::AddNodeData(context, name);

    for (size_t i = 0; i < values.size(); ++i)
    {
        xml::WriteVector2i(node.append_child("Child"), values[i]);
    }
}

void WriteVector2Array(DataSaveContext& context, const std::string& name, const std::vector<Vector2f>& values)
{
    pugi::xml_node node = impl::AddNodeData(context, name);

    for (size_t i = 0; i < values.size(); ++i)
    {
        xml::WriteVector2f(node.append_child("Child"), values[i]);
    }
}

void WriteDatasheetReference(DataSaveContext& _kContext, const std::string& _strName, const DatasheetObject* _pMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    const Datasheet* datasheet = _pMember == nullptr ? nullptr : _pMember->GetDatasheet();
    if (datasheet)
    {
        pNode.append_attribute("value").set_value(datasheet->GetID().c_str());
    }
    else
    {
        pNode.append_attribute("value").set_value("");
    }
}

void WriteDatasaveInstance(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const DatasaveObject* _pMember)
{
    pugi::xml_node pNode = impl::AddNodeData(_kContext, _strName);

    if (_pMember)
    {
        pNode.append_attribute("type").set_value(_pMember->GetDataInstanceType().c_str());

        pugi::xml_node* pNodeParent = _kContext.currentNode;
        _kContext.currentNode = &pNode;

        _pMember->SerializeMembers(_kContext);

        _kContext.currentNode = pNodeParent;
    }
    else
    {
        pNode.append_attribute("type").set_value("");
    }
}

}   // namespace binding

}   // namespace gugu
