////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Data/DatasheetObject.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/System/SystemUtility.h"
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

bool DatasheetObject::LoadFromFile(const std::string& path, Datasheet* ownerDatasheet, std::vector<Datasheet*>& ancestors)
{
    if (ownerDatasheet != nullptr)
    {
        m_datasheet = ownerDatasheet;
    }

    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(path.c_str());
    if (!result)
        return false;

    pugi::xml_node datasheetNode = document.child("Datasheet");
    if (datasheetNode.empty())
        return false;

    pugi::xml_node rootNode = datasheetNode.child("RootObject");
    if (rootNode.empty())
        return false;

    // TODO: check root type exactitude ?

    pugi::xml_attribute pAttributeParent = datasheetNode.attribute("parent");
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
                    ancestorsLog += ancestor->GetID() + ", ";
                }

                ancestorsLog += parentSheet->GetID();
                GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet ancestors create an infinite loop : {0}", ancestorsLog));
            }
            else
            {
                ancestors.push_back(parentSheet);
                LoadFromFile(parentSheet->GetFileInfo().GetFileSystemPath(), nullptr, ancestors);
            }
        }
    }

    DataParseContext context;
    std::map<DataObject*, pugi::xml_node> pendingNodes;
    pendingNodes.insert(std::make_pair(this, rootNode));

    // Instanciate all objects.
    for (pugi::xml_node objectNode = datasheetNode.child("Object"); objectNode; objectNode = objectNode.next_sibling("Object"))
    {
        pugi::xml_attribute typeAttribute = objectNode.attribute("type");
        pugi::xml_attribute uuidAttribute = objectNode.attribute("uuid");
        if (typeAttribute && uuidAttribute)
        {
            UUID uuid = UUID::FromString(uuidAttribute.as_string());
            DataObject* instanceObject = GetResources()->InstanciateDataObject(typeAttribute.as_string());

            if (instanceObject)
            {
                context.objectByUUID.insert(std::make_pair(uuid, instanceObject));
                pendingNodes.insert(std::make_pair(instanceObject, objectNode));
            }
        }
    }

    // Parse objects data.
    for (auto it : pendingNodes)
    {
        context.currentNode = &it.second;
        it.first->ParseMembers(context);
    }

    return true;
}

Datasheet* DatasheetObject::GetDatasheet() const
{
    return m_datasheet;
}

}   // namespace gugu
