////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheetObject.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

VirtualDatasheet::VirtualDatasheet()
    : m_rootObject(nullptr)
    , m_parentDatasheet(nullptr)
{
}

VirtualDatasheet::~VirtualDatasheet()
{
    Unload();
}

EResourceType::Type VirtualDatasheet::GetResourceType() const
{
    return EResourceType::Datasheet;    // TODO: Should I use Custom ? Or a dedicated VirtualDatasheet enum value ?
}

void VirtualDatasheet::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (m_parentDatasheet)
    {
        dependencies.insert(m_parentDatasheet);
    }

    m_rootObject->GetDependencies(dependencies);
}

void VirtualDatasheet::OnDependencyRemoved(const Resource* removedDependency)
{
    if (m_parentDatasheet == removedDependency)
    {
        m_parentDatasheetID = "";
        m_parentDatasheet = nullptr;
    }

    m_rootObject->OnDependencyRemoved(removedDependency);
}

bool VirtualDatasheet::InstanciateNewRootObject(DatasheetParser::ClassDefinition* classDefinition)
{
    if (m_rootObject)
    {
        return false;
    }

    UUID uuid = UUID::Generate();

    m_rootObject = new VirtualDatasheetObject;
    m_rootObject->m_uuid = uuid;
    m_rootObject->m_classDefinition = classDefinition;

    return true;
}

VirtualDatasheetObject* VirtualDatasheet::InstanciateNewObject(DatasheetParser::ClassDefinition* classDefinition)
{
    UUID uuid = UUID::Generate();

    VirtualDatasheetObject* instanceObject = new VirtualDatasheetObject;
    instanceObject->m_uuid = uuid;
    instanceObject->m_classDefinition = classDefinition;

    m_instanceObjects.insert(std::make_pair(uuid, instanceObject));
    return instanceObject;
}

bool VirtualDatasheet::DeleteOrphanedInstanceObjects()
{
    // Recursively retrieve all used instance uuids.
    std::set<UUID> instanceUuids;
    m_rootObject->GatherInstanceUuids(instanceUuids);

    // Delete all orphaned instance objects.
    bool foundOrphans = false;

    for (auto it = m_instanceObjects.begin(); it != m_instanceObjects.end();)
    {
        if (!StdSetContains(instanceUuids, it->first))
        {
            SafeDelete(it->second);
            it = m_instanceObjects.erase(it);

            foundOrphans = true;
        }
        else
        {
            ++it;
        }
    }

    return foundOrphans;
}

bool VirtualDatasheet::DeleteInstanceObject(VirtualDatasheetObject* instanceObject)
{
    if (!instanceObject)
        return true;

    auto it = m_instanceObjects.find(instanceObject->m_uuid);
    if (it != m_instanceObjects.end())
    {
        m_instanceObjects.erase(it);
        SafeDelete(instanceObject);
        return true;
    }

    return false;
}

bool VirtualDatasheet::IsValidAsParent(VirtualDatasheet* parentDatasheet, bool* invalidRecursiveParent) const
{
    if (!parentDatasheet)
    {
        // A null parent is a valid case.
        return true;
    }

    if (parentDatasheet->GetClassDefinition() != GetClassDefinition())
    {
        // We dont want a parent of a different class.
        return false;
    }

    const VirtualDatasheet* nextParent = parentDatasheet;
    while (nextParent)
    {
        if (nextParent == this)
        {
            // We dont want an infinite loop of parent links.
            if (invalidRecursiveParent)
            {
                *invalidRecursiveParent = true;
            }

            return false;
        }

        nextParent = nextParent->m_parentDatasheet;
    }

    return true;
}

void VirtualDatasheet::SetParentDatasheet(const std::string& parentReferenceID, VirtualDatasheet* parentDatasheet)
{
    m_parentDatasheetID = parentReferenceID;
    m_parentDatasheet = parentDatasheet;

    m_rootObject->RefreshParentObject(m_parentDatasheet ? m_parentDatasheet->m_rootObject : nullptr);
}

DatasheetParser::ClassDefinition* VirtualDatasheet::GetClassDefinition() const
{
    return m_rootObject ? m_rootObject->m_classDefinition : nullptr;
}

void VirtualDatasheet::SortDataValues()
{
    m_rootObject->SortDataValues();

    for (const auto& kvp : m_instanceObjects)
    {
        kvp.second->SortDataValues();
    }
}

void VirtualDatasheet::Unload()
{
    SafeDelete(m_rootObject);
    ClearStdMap(m_instanceObjects);
    ClearStdMap(m_objectOverrides);

    m_parentDatasheet = nullptr;
}

bool VirtualDatasheet::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node datasheetNode = document.child("Datasheet");
    if (!datasheetNode)
        return false;

    pugi::xml_node rootNode = datasheetNode.child("RootObject");
    if (!rootNode)
        return false;

    m_rootObject = new VirtualDatasheetObject;
    if (!m_rootObject->LoadFromXml(rootNode))
    {
        // TODO: log error ?
        SafeDelete(m_rootObject);
        return false;
    }

    // Parse Instance Object nodes.
    std::set<UUID> orphanObjectUuids;

    for (pugi::xml_node nodeObject = datasheetNode.child("Object"); nodeObject; nodeObject = nodeObject.next_sibling("Object"))
    {
        VirtualDatasheetObject* dataObject = new VirtualDatasheetObject;

        if (dataObject->LoadFromXml(nodeObject))
        {
            m_instanceObjects.insert(std::make_pair(dataObject->m_uuid, dataObject));
            orphanObjectUuids.insert(dataObject->m_uuid);
        }
        else
        {
            // TODO: log error ?
            SafeDelete(dataObject);
        }
    }

    // Parse Object Override nodes.
    for (pugi::xml_node nodeObject = datasheetNode.child("ObjectOverride"); nodeObject; nodeObject = nodeObject.next_sibling("ObjectOverride"))
    {
        VirtualDatasheetObject* dataObject = new VirtualDatasheetObject;

        if (dataObject->LoadFromXml(nodeObject))
        {
            m_objectOverrides.insert(std::make_pair(dataObject->m_uuid, dataObject));
        }
        else
        {
            // TODO: log error ?
            SafeDelete(dataObject);
        }
    }

    // Resolve instance links.
    m_rootObject->ResolveInstances(m_instanceObjects, orphanObjectUuids);

    for (const auto& kvp : m_instanceObjects)
    {
        kvp.second->ResolveInstances(m_instanceObjects, orphanObjectUuids);
    }

    if (!orphanObjectUuids.empty())
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("Datasheet contains {0} orphan instanced objects : {1}", orphanObjectUuids.size(), GetFileInfo().GetFilePath_utf8()));
    }

    // Compute parent data.
    std::string parentResourceID = "";
    VirtualDatasheet* parentDatasheet = nullptr;

    pugi::xml_attribute attributeParent = datasheetNode.attribute("parent");
    if (attributeParent)
    {
        // TODO: I should encapsulate this in some kind of GetOrLoad method.
        parentResourceID = attributeParent.value();
        if (GetResources()->IsResourceLoaded(parentResourceID))
        {
            parentDatasheet = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(parentResourceID));
        }
        else
        {
            parentDatasheet = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(parentResourceID);
        }
    }

    if (!IsValidAsParent(parentDatasheet, nullptr))
    {
        parentDatasheet = nullptr;
    }

    SetParentDatasheet(parentResourceID, parentDatasheet);

    return true;
}

bool VirtualDatasheet::SaveToXml(pugi::xml_document& document) const
{
    if (!m_rootObject)
        return false;

    pugi::xml_node nodeDatasheet = document.append_child("Datasheet");
    nodeDatasheet.append_attribute("serializationVersion") = 2;

    // Binding version, could be used for application binding changes.
    nodeDatasheet.append_attribute("bindingVersion") = 1;

    if (!m_parentDatasheetID.empty())
    {
        nodeDatasheet.append_attribute("parent") = m_parentDatasheetID.c_str();
    }

    // Serialize all objects to xml.
    // Instance objects are naturally sorted by UUID through the map.
    if (!m_rootObject->SaveToXml(nodeDatasheet, "RootObject"))
        return false;

    for (const auto& kvp : m_instanceObjects)
    {
        if (!kvp.second->SaveToXml(nodeDatasheet, "Object"))
            return false;
    }

    for (const auto& kvp : m_objectOverrides)
    {
        if (!kvp.second->SaveToXml(nodeDatasheet, "ObjectOverride"))
            return false;
    }

    return true;
}

bool VirtualDatasheet::HandleMigration(const FileInfo& fileInfo)
{
    pugi::xml_document document;
    pugi::xml_parse_result loadResult = document.load_file(fileInfo.GetFileSystemPath().c_str());
    if (!loadResult)
        return false;

    int serializationVersion = document.child("Datasheet").attribute("serializationVersion").as_int();
    if (serializationVersion <= 1)
    {
        Migrate_v1_to_v2(fileInfo, document);
    }

    document.save_file(fileInfo.GetFileSystemPath().c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);

    return true;
}

namespace impl {

void MigrateInstanceObject_v1_to_v2(
    pugi::xml_node nodeDatasheet,
    pugi::xml_node nodeObjectV1,
    pugi::xml_node nodeObjectV2,
    DatasheetParser::ClassDefinition* instanceClassDefinition);

void MigrateDataMember_v1_to_v2(
    pugi::xml_node nodeDatasheet,
    pugi::xml_node nodeData,
    DatasheetParser::DataMemberDefinition* dataMemberDef)
{
    if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Type::ObjectInstance)
    {
        pugi::xml_attribute attributeType = nodeData.attribute("type");
        nodeData.remove_attribute("type");

        if (attributeType.empty() || StringEquals(attributeType.value(), ""))
        {
            // Explicit null instance
            nodeData.append_attribute("value").set_value("");
        }
        else
        {
            DatasheetParser::ClassDefinition* instanceClassDefinition = nullptr;
            if (!GetEditor()->GetDatasheetParser()->GetClassDefinition(attributeType.as_string(), instanceClassDefinition))
            {
                instanceClassDefinition = dataMemberDef->objectDefinition;
            }

            std::string uuid = GenerateUUIDAsString();

            pugi::xml_node nodeInstanceObject = nodeDatasheet.append_child("Object");
            nodeInstanceObject.append_attribute("type").set_value(instanceClassDefinition->m_name.c_str());
            nodeInstanceObject.append_attribute("uuid").set_value(uuid.c_str());

            nodeData.append_attribute("value").set_value(uuid.c_str());

            impl::MigrateInstanceObject_v1_to_v2(nodeDatasheet, nodeData, nodeInstanceObject, instanceClassDefinition);
        }
    }
}

void MigrateInstanceObject_v1_to_v2(
    pugi::xml_node nodeDatasheet,
    pugi::xml_node nodeObjectV1,
    pugi::xml_node nodeObjectV2,
    DatasheetParser::ClassDefinition* instanceClassDefinition)
{
    pugi::xml_node nextNodeData = nodeObjectV1.child("Data");
    for (pugi::xml_node nodeData = nextNodeData; nodeData; nodeData = nextNodeData)
    {
        nextNodeData = nodeData.next_sibling("Data");

        DatasheetParser::DataMemberDefinition* dataMemberDef = instanceClassDefinition->GetDataMemberDefinition(nodeData.attribute("name").as_string());
        if (dataMemberDef)
        {
            if (dataMemberDef->isArray)
            {
                for (pugi::xml_node nodeChild = nodeData.child("Child"); nodeChild; nodeChild = nodeChild.next_sibling("Child"))
                {
                    MigrateDataMember_v1_to_v2(nodeDatasheet, nodeChild, dataMemberDef);
                }
            }
            else
            {
                MigrateDataMember_v1_to_v2(nodeDatasheet, nodeData, dataMemberDef);
            }
        }

        nodeObjectV2.append_move(nodeData);
    }
}

}   // namespace impl

bool VirtualDatasheet::Migrate_v1_to_v2(const FileInfo& fileInfo, pugi::xml_document& document)
{
    pugi::xml_document migratedDocument;

    pugi::xml_node nodeDatasheet = document.child("Datasheet");
    if (!nodeDatasheet)
        return false;

    std::string attributeValueBindingVersion = nodeDatasheet.attribute("bindingVersion").as_string("1");
    std::string attributeValueParent = nodeDatasheet.attribute("parent").as_string();

    nodeDatasheet.remove_attributes();
    nodeDatasheet.append_attribute("serializationVersion").set_value(2);
    nodeDatasheet.append_attribute("bindingVersion").set_value(attributeValueBindingVersion.c_str());

    if (!attributeValueParent.empty())
    {
        nodeDatasheet.append_attribute("parent").set_value(attributeValueParent.c_str());
    }

    DatasheetParser::ClassDefinition* rootClassDefinition = nullptr;
    if (!GetEditor()->GetDatasheetParser()->GetClassDefinition(fileInfo.GetExtension(), rootClassDefinition))
    {
        return false;
    }

    pugi::xml_node nodeRootObject = nodeDatasheet.append_child("RootObject");
    nodeRootObject.append_attribute("type").set_value(rootClassDefinition->m_name.c_str());
    nodeRootObject.append_attribute("uuid").set_value(GenerateUUIDAsString().c_str());

    impl::MigrateInstanceObject_v1_to_v2(nodeDatasheet, nodeDatasheet, nodeRootObject, rootClassDefinition);

    return true;
}

}   // namespace gugu
