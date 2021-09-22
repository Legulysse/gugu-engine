////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Datasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/External/PugiXmlWrap.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Debug/Logger.h"

#include <stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Datasheet::Datasheet()
{
}

Datasheet::~Datasheet()
{
}

EResourceType::Type Datasheet::GetResourceType() const
{
    return EResourceType::Datasheet;
}

bool Datasheet::LoadFromFile()
{
    std::vector<Datasheet*> ancestors;
    ancestors.push_back(this);
    return LoadFromXml(GetFileInfoRef().GetPathName(), ancestors);
}

bool Datasheet::LoadFromXml(const std::string& _strPathName, std::vector<Datasheet*>& ancestors)
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
        Datasheet* parentSheet = ResolveDatasheetLink(pAttributeParent.as_string());

        if (parentSheet)
        {
            if (StdVectorContains(ancestors, parentSheet))
            {
                std::string ancestorsLog;
                for (Datasheet* ancestor : ancestors)
                {
                    ancestorsLog += ancestor->GetFileInfoRef().GetName() + ", ";
                }

                ancestorsLog += parentSheet->GetFileInfoRef().GetName();
                GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet {0} ancestors create an infinite loop : {1}", GetFileInfoRef().GetName(), ancestorsLog));
            }
            else
            {
                ancestors.push_back(parentSheet);
                LoadFromXml(parentSheet->GetFileInfoRef().GetPathName(), ancestors);
            }
        }
    }

    DatasheetParserContext kContext;
    kContext.currentNode = &oNodeRoot;
    ParseMembers(kContext);

    return true;
}

void Datasheet::ParseMembers(DatasheetParserContext& _kContext)
{
    //Virtual
}

pugi::xml_node Datasheet::FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName)
{
    return _kContext.currentNode->find_child_by_attribute("Data", "name", _strName.c_str());
}

void Datasheet::ReadString(DatasheetParserContext& _kContext, const std::string& _strName, std::string& _strMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _strMember = pAttribute.as_string(_strMember.c_str());
    }
}

void Datasheet::ReadInt(DatasheetParserContext& _kContext, const std::string& _strName, int& _iMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _iMember = pAttribute.as_int(_iMember);
    }
}

void Datasheet::ReadFloat(DatasheetParserContext& _kContext, const std::string& _strName, float& _fMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _fMember = pAttribute.as_float(_fMember);
    }
}

void Datasheet::ReadBool(DatasheetParserContext& _kContext, const std::string& _strName, bool& _bMember)
{
    pugi::xml_node pNode = FindNodeData(_kContext, _strName);
    if (pNode)
    {
        pugi::xml_attribute pAttribute = pNode.attribute("value");
        if (pAttribute)
            _bMember = pAttribute.as_bool(_bMember);
    }
}

void Datasheet::ReadArrayString(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<std::string>& _vecMember)
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

void Datasheet::ReadArrayInt(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<int>& _vecMember)
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

void Datasheet::ReadArrayFloat(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<float>& _vecMember)
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

void Datasheet::ReadArrayBool(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<bool>& _vecMember)
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

Datasheet* Datasheet::InstanciateDatasheet(DatasheetParserContext& _kContext, const std::string& _strType)
{
    Datasheet* pDatasheet = GetResources()->InstanciateDatasheet(_strType);
    if (pDatasheet)
    {
        pDatasheet->ParseMembers(_kContext);
        return pDatasheet;
    }
    else
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Could not instantiate Datasheet : {0}", _strType));
    }

    return nullptr;
}

bool Datasheet::InstanciateDatasheet(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, Datasheet*& _pInstance)
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

            _pInstance = InstanciateDatasheet(_kContext, strType);

            _kContext.currentNode = pNodeParent;
        }

        return true;
    }

    return false;
}

bool Datasheet::InstanciateDatasheets(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strDefaultType, std::vector<Datasheet*>& _vecInstances)
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
                Datasheet* pInstance = InstanciateDatasheet(_kContext, strType);
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

Datasheet* Datasheet::ResolveDatasheetLink(const std::string& _strName)
{
    return GetResources()->GetDatasheet(_strName);
}

bool Datasheet::ResolveDatasheetLink(DatasheetParserContext& _kContext, const std::string& _strName, Datasheet*& _pNewDatasheet)
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

bool Datasheet::ResolveDatasheetLinks(DatasheetParserContext& _kContext, const std::string& _strName, std::vector<Datasheet*>& _vecDatasheets)
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
                    Datasheet* pDatasheet = ResolveDatasheetLink(datasheetID);
                    _vecDatasheets.push_back(pDatasheet);
                }
                else
                {
                    _vecDatasheets.push_back(nullptr);
                }
            }

            pNodeChild = pNodeChild.next_sibling("Child");
        }

        return true;
    }

    return false;
}

bool Datasheet::ReadEnumValue(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, int& _iValue)
{
    DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
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

bool Datasheet::ReadEnumValues(DatasheetParserContext& _kContext, const std::string& _strName, const std::string& _strType, std::vector<int>& _vecValues)
{
    DatasheetEnum* pEnum = GetResources()->GetDatasheetEnum(_strType);
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

}   // namespace gugu
