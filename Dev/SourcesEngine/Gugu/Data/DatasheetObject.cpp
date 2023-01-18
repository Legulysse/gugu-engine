////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DatasheetObject.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetObject::DatasheetObject()
    : m_datasheet(nullptr)
{
}

DatasheetObject::~DatasheetObject()
{
}

bool DatasheetObject::LoadFromXml(const std::string& _strPathName, Datasheet* ownerDatasheet, std::vector<Datasheet*>& ancestors)
{
    if (ownerDatasheet != nullptr)
    {
        m_datasheet = ownerDatasheet;
    }

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
                    ancestorsLog += ancestor->GetFileInfo().GetFileName() + ", ";
                }

                ancestorsLog += parentSheet->GetFileInfo().GetFileName();
                GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet ancestors create an infinite loop : {0}", ancestorsLog));
            }
            else
            {
                ancestors.push_back(parentSheet);
                LoadFromXml(parentSheet->GetFileInfo().GetFilePath(), nullptr, ancestors);
            }
        }
    }

    DatasheetParserContext kContext;
    kContext.currentNode = &oNodeRoot;
    ParseMembers(kContext);

    return true;
}

Datasheet* DatasheetObject::GetDatasheet() const
{
    return m_datasheet;
}

pugi::xml_node DatasheetObject::FindNodeData(DatasheetParserContext& _kContext, const std::string& _strName)
{
    return _kContext.currentNode->find_child_by_attribute("Data", "name", _strName.c_str());
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

}   // namespace gugu
