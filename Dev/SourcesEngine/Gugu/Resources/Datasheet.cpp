////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Datasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Debug/Logger.h"

#include <stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetObject::DatasheetObject()
{
}

DatasheetObject::~DatasheetObject()
{
}

bool DatasheetObject::LoadFromXml(const std::string& _strPathName, std::vector<Datasheet*>& ancestors)
{
    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(_strPathName.c_str());
    if (!result)
        return false;

    pugi::xml_node oNodeRoot = oDoc.child("Datasheet");
    if (oNodeRoot.empty())
        return false;

    pugi::xml_attribute pAttributeParent = oNodeRoot.attribute("parent");
    if (pAttributeParent)
    {
        //TODO: check same type
        //TODO: error message if invalid base
        Datasheet* parentSheet = GetResources()->GetDatasheet(pAttributeParent.as_string());

        if (parentSheet)
        {
            if (StdVectorContains(ancestors, parentSheet))
            {
                std::string ancestorsLog;
                for (Datasheet* ancestor : ancestors)
                {
                    ancestorsLog += ancestor->GetFileInfo().GetName() + ", ";
                }

                ancestorsLog += parentSheet->GetFileInfo().GetName();
                GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet ancestors create an infinite loop : {0}", ancestorsLog));
            }
            else
            {
                ancestors.push_back(parentSheet);
                LoadFromXml(parentSheet->GetFileInfo().GetPathName(), ancestors);
            }
        }
    }

    DatasheetParserContext kContext;
    kContext.currentNode = &oNodeRoot;
    ParseMembers(kContext);

    return true;
}

pugi::xml_node DatasheetObject::FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName)
{
    return _kContext.currentNode->find_child_by_attribute("Data", "name", _strName.c_str());
}

void DatasheetObject::ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _strMember = pAttribute.as_string(_strMember.c_str());
    }
}

void DatasheetObject::ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _iMember = pAttribute.as_int(_iMember);
    }
}

void DatasheetObject::ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _fMember = pAttribute.as_float(_fMember);
    }
}

void DatasheetObject::ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _bMember = pAttribute.as_bool(_bMember);
    }
}

void DatasheetObject::ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
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

void DatasheetObject::ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
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

void DatasheetObject::ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
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

void DatasheetObject::ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
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

const DatasheetObject* DatasheetObject::InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strType)
{
    DatasheetObject* instance = GetResources()->InstanciateDatasheetObject(_strType);
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

bool DatasheetObject::InstanciateDatasheetObject(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, const DatasheetObject*& _pInstance)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        // Check type overload. If serialized type is explicitely empty, we force a null instance.
        pugi::xml_attribute pAttributeType = pNode.attribute("type");
        std::string strType = (pAttributeType) ? pAttributeType.as_string() : _strDefaultType;

        if (strType != "")
        {
            pugi::xml_node* pNodeParent = _kContext.currentNode;
            _kContext.currentNode = &pNode;

            _pInstance = InstanciateDatasheetObject(_kContext, strType);

            _kContext.currentNode = pNodeParent;
        }

        return true;
    }

    return false;
}

bool DatasheetObject::InstanciateDatasheetObjects(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<const DatasheetObject*>& _vecInstances)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_node* pNodeParent = _kContext.currentNode;

        pugi::xml_node pNodeChild = pNode.child("Child");
        while (pNodeChild)
        {
            // Check type overload. If serialized type is explicitely empty, we force a null instance.
            pugi::xml_attribute pAttributeType = pNodeChild.attribute("type");
            std::string strType = (pAttributeType) ? pAttributeType.as_string() : _strDefaultType;

            if (strType != "")
            {
                _kContext.currentNode = &pNodeChild;
                const DatasheetObject* pInstance = InstanciateDatasheetObject(_kContext, strType);
                if (pInstance)
                {
                    _vecInstances.push_back(pInstance);
                }
            }
            else
            {
                _vecInstances.push_back(nullptr);
            }

            pNodeChild = pNodeChild.next_sibling("Child");
        }

        _kContext.currentNode = pNodeParent;

        return true;
    }

    return false;
}

const DatasheetObject* DatasheetObject::ResolveDatasheetLink(const std::string& _strName)
{
    Datasheet* datasheet = GetResources()->GetDatasheet(_strName);
    return datasheet ? datasheet->GetRootObject() : nullptr;
}

bool DatasheetObject::ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, const DatasheetObject*& _pNewDatasheet)
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

bool DatasheetObject::ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<const DatasheetObject*>& _vecReferences)
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

bool DatasheetObject::ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue)
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

bool DatasheetObject::ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues)
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


Datasheet::Datasheet()
    : m_rootObject(nullptr)
{
}

Datasheet::~Datasheet()
{
    Unload();
}

EResourceType::Type Datasheet::GetResourceType() const
{
    return EResourceType::Datasheet;
}

void Datasheet::Unload()
{
    SafeDelete(m_rootObject);
}

bool Datasheet::LoadFromFile()
{
    Unload();

    // TODO: handle extensions with several dots (like name.type.xml instead of name.type).
    m_rootObject = GetResources()->InstanciateDatasheetObject(m_resourceInfos->fileInfo.GetExtension());
    if (!m_rootObject)
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Could not instantiate Datasheet : {0}", GetID()));
    }

    std::vector<Datasheet*> ancestors;
    ancestors.push_back(this);
    return m_rootObject->LoadFromXml(GetFileInfo().GetPathName(), ancestors);
}

const DatasheetObject* Datasheet::GetRootObject() const
{
    return m_rootObject;
}

}   // namespace gugu
