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
    // This will only be set for the actual datasheet, and null when parsing parent datasheets.
    bool isActualRoot = false;
    if (ownerDatasheet != nullptr)
    {
        m_datasheet = ownerDatasheet;
        isActualRoot = true;
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

    UUID rootUuid = UUID::FromString(rootNode.attribute("uuid").as_string());
    if (rootUuid.IsZero())
        return false;

    // TODO: check root type exactitude ?

    if (isActualRoot)
    {
        m_uuid = rootUuid;
        m_datasheet->m_instanceObjects.insert(std::make_pair(m_uuid, this));
    }

    pugi::xml_attribute pAttributeParent = datasheetNode.attribute("parent");
    if (pAttributeParent)
    {
        // TODO: Do I need to actually load the parent sheet ?
        // TODO: check same type.
        // TODO: error message if invalid base.
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

    // Instanciate Objects.
    std::map<DataObject*, pugi::xml_node> pendingNodes;
    pendingNodes.insert(std::make_pair(this, rootNode));

    for (pugi::xml_node objectNode = datasheetNode.child("Object"); objectNode; objectNode = objectNode.next_sibling("Object"))
    {
        pugi::xml_attribute typeAttribute = objectNode.attribute("type");
        pugi::xml_attribute uuidAttribute = objectNode.attribute("uuid");
        if (typeAttribute && uuidAttribute)
        {
            UUID uuid = UUID::FromString(uuidAttribute.as_string());
            DataObject* instanceDataObject = GetResources()->InstanciateDataObject(typeAttribute.as_string());
            DatasheetObject* instanceDatasheetObject = dynamic_cast<DatasheetObject*>(instanceDataObject);
            if (instanceDatasheetObject)
            {
                instanceDatasheetObject->m_uuid = uuid;
                m_datasheet->m_instanceObjects.insert(std::make_pair(uuid, instanceDatasheetObject));

                pendingNodes.insert(std::make_pair(instanceDatasheetObject, objectNode));
            }
            else
            {
                SafeDelete(instanceDataObject);
            }
        }
    }

    // Prepare data parser.
    DataParseContext context;
    context.objectByUUID = &m_datasheet->m_instanceObjects;

    // Parse instance objects data.
    for (auto& it : pendingNodes)
    {
        context.currentNode = &it.second;
        it.first->ParseMembers(context);
    }

    // Parse object overrides data.
    for (pugi::xml_node objectOverrideNode = datasheetNode.child("ObjectOverride"); objectOverrideNode; objectOverrideNode = objectOverrideNode.next_sibling("ObjectOverride"))
    {
        UUID objectOverrideUuid = UUID::FromString(objectOverrideNode.attribute("uuid").as_string());
        if (!objectOverrideUuid.IsZero())
        {
            auto it = m_datasheet->m_instanceObjects.find(objectOverrideUuid);
            if (it != m_datasheet->m_instanceObjects.end())
            {
                context.currentNode = &objectOverrideNode;
                it->second->ParseMembers(context);
            }
        }
    }

    return true;
}

const UUID& DatasheetObject::GetUuid() const
{
    return m_uuid;
}

Datasheet* DatasheetObject::GetDatasheet() const
{
    return m_datasheet;
}

}   // namespace gugu
