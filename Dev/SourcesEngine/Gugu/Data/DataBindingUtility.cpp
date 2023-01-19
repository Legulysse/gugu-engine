////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DataBindingUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DatasheetObject.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

namespace DataBinding {

namespace Impl {

pugi::xml_node FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName)
{
    return _kContext.currentNode->find_child_by_attribute("Data", "name", _strName.c_str());
}

pugi::xml_node AddNodeData(DataSaveContext& _kContext, const std::string& _strName)
{
    pugi::xml_node nodeData = _kContext.currentNode->append_child("Data");
    nodeData.append_attribute("name").set_value(_strName.c_str());
    return nodeData;
}

bool ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue)
{
    const DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
    if (pEnum)
    {
        pugi::xml_node pNode = FindNodeData(_kContext, _strName);
        if (pNode)
        {
            pugi::xml_attribute pAttributeValue = pNode.attribute("value");
            if (pAttributeValue)
            {
                std::string strEnumValue = pAttributeValue.as_string("");
                for (size_t i = 0; i < pEnum->values.size(); ++i)
                {
                    if (pEnum->values[i] == strEnumValue)
                    {
                        _iValue = (int)i;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues)
{
    const DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
    if (pEnum)
    {
        pugi::xml_node pNode = FindNodeData(_kContext, _strName);
        if (pNode)
        {
            pugi::xml_node pNodeChild = pNode.child("Child");
            while (pNodeChild)
            {
                pugi::xml_attribute pAttributeValue = pNodeChild.attribute("value");
                if (pAttributeValue)
                {
                    std::string strEnumValue = pAttributeValue.as_string("");
                    for (size_t i = 0; i < pEnum->values.size(); ++i)
                    {
                        if (pEnum->values[i] == strEnumValue)
                        {
                            _vecValues.push_back((int)i);
                        }
                    }
                }

                pNodeChild = pNodeChild.next_sibling("Child");
            }

            return true;
        }
    }

    return false;
}

void WriteEnumValue(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, int _iValue)
{
    const DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
    if (pEnum && _iValue >= 0 && _iValue < pEnum->values.size())
    {
        pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
        pNode.append_attribute("value").set_value(pEnum->values[_iValue].c_str());
    }
}

void WriteEnumValues(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strType, const std::vector<int>& _vecValues)
{
    const DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
    if (pEnum)
    {
        pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

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

bool ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, const DatasheetObject*& _pNewDatasheet)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttributeValue = pNode.attribute("value");
        if (pAttributeValue)
        {
            // Reference can be null.
            std::string datasheetID = pAttributeValue.as_string();
            if (datasheetID != "")
            {
                std::string strSheetName = pAttributeValue.as_string();
                _pNewDatasheet = ResolveDatasheetLink(strSheetName);
            }
        }

        return true;
    }

    return false;
}

bool ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecReferences)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            pugi::xml_attribute pAttributeValue = pNodeChild.attribute("value");
            if (pAttributeValue)
            {
                // Reference can be null.
                std::string datasheetID = pAttributeValue.as_string();
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

            pNodeChild = pNodeChild.next_sibling("Child");
        }

        return true;
    }

    return false;
}

}   // namespace Impl

void ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _strMember = pAttribute.as_string(_strMember.c_str());
    }
}

void ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _iMember = pAttribute.as_int(_iMember);
    }
}

void ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _fMember = pAttribute.as_float(_fMember);
    }
}

void ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _bMember = pAttribute.as_bool(_bMember);
    }
}

void ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        _vecMember.clear();     //TODO: find a way to allow append instead of clear on arrays inheritance

        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            pugi::xml_attribute pAttribute = pNodeChild.attribute("value");
            if (pAttribute)
                _vecMember.push_back(pAttribute.as_string());
            pNodeChild = pNodeChild.next_sibling("Child");
        }
    }
}

void ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        _vecMember.clear();

        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            pugi::xml_attribute pAttribute = pNodeChild.attribute("value");
            if (pAttribute)
                _vecMember.push_back(pAttribute.as_int());
            pNodeChild = pNodeChild.next_sibling("Child");
        }
    }
}

void ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        _vecMember.clear();

        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            pugi::xml_attribute pAttribute = pNodeChild.attribute("value");
            if (pAttribute)
                _vecMember.push_back(pAttribute.as_float());
            pNodeChild = pNodeChild.next_sibling("Child");
        }
    }
}

void ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember)
{
    pugi::xml_node pNode = Impl::FindNodeData(_kContext, _strName);
    if (pNode)
    {
        _vecMember.clear();

        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            pugi::xml_attribute pAttribute = pNodeChild.attribute("value");
            if (pAttribute)
                _vecMember.push_back(pAttribute.as_bool());
            pNodeChild = pNodeChild.next_sibling("Child");
        }
    }
}

void WriteString(DataSaveContext& _kContext, const std::string& _strName, const std::string& _strMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
    pNode.append_attribute("value").set_value(_strMember.c_str());
}

void WriteInt(DataSaveContext& _kContext, const std::string& _strName, int _iMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
    pNode.append_attribute("value").set_value(_iMember);
}

void WriteFloat(DataSaveContext& _kContext, const std::string& _strName, float _fMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
    pNode.append_attribute("value").set_value(_fMember);
}

void WriteBool(DataSaveContext& _kContext, const std::string& _strName, bool _bMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
    pNode.append_attribute("value").set_value(_bMember);
}

void WriteArrayString(DataSaveContext& _kContext, const std::string& _strName, const std::vector<std::string>& _vecMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i].c_str());
    }
}

void WriteArrayInt(DataSaveContext& _kContext, const std::string& _strName, const std::vector<int>& _vecMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void WriteArrayFloat(DataSaveContext& _kContext, const std::string& _strName, const std::vector<float>& _vecMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void WriteArrayBool(DataSaveContext& _kContext, const std::string& _strName, const std::vector<bool>& _vecMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

    for (size_t i = 0; i < _vecMember.size(); ++i)
    {
        pNode.append_child("Child").append_attribute("value").set_value(_vecMember[i]);
    }
}

void WriteReference(DataSaveContext& _kContext, const std::string& _strName, const DatasheetObject* _pMember)
{
    const Datasheet* datasheet = _pMember->GetDatasheet();
    if (datasheet)
    {
        pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);
        pNode.append_attribute("value").set_value(datasheet->GetID().c_str());
    }
}

void WriteArrayReference(DataSaveContext& _kContext, const std::string& _strName, const std::vector<const DatasheetObject*>& _pMember)
{
    pugi::xml_node pNode = Impl::AddNodeData(_kContext, _strName);

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

}   // namespace DataBinding

}   // namespace gugu
