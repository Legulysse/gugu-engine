////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

VirtualDatasheetObject::DataValue::~DataValue()
{
    SafeDelete(value_objectInstance);
    ClearStdVector(value_children);

    dataMemberDefinition = nullptr;
    value_objectInstanceDefinition = nullptr;
    value_objectReference = nullptr;
}

VirtualDatasheetObject::VirtualDatasheetObject()
    : m_parentObject(nullptr)
    , m_classDefinition(nullptr)
{
}

VirtualDatasheetObject::~VirtualDatasheetObject()
{
    m_parentObject = nullptr;
    m_classDefinition = nullptr;
    ClearStdVector(m_dataValues);
}

void VirtualDatasheetObject::GetDependencies(std::set<Resource*>& dependencies) const
{
    GetDependencies(m_dataValues, dependencies);
}

void VirtualDatasheetObject::GetDependencies(const std::vector<VirtualDatasheetObject::DataValue*>& dataValues, std::set<Resource*>& dependencies) const
{
    for (auto dataValue : dataValues)
    {
        if (dataValue->value_objectReference)
        {
            dependencies.insert(dataValue->value_objectReference);
        }
        else if (dataValue->value_objectInstance)
        {
            dataValue->value_objectInstance->GetDependencies(dependencies);
        }
        else
        {
            GetDependencies(dataValue->value_children, dependencies);
        }
    }
}

void VirtualDatasheetObject::OnDependencyRemoved(const Resource* removedDependency)
{
    const VirtualDatasheet* removedDatasheet = dynamic_cast<const VirtualDatasheet*>(removedDependency);

    // TODO: Handle recursive reparenting on child objects (this could be tied to RefreshParentObject future updates).
    if (removedDatasheet && removedDatasheet->m_rootObject == m_parentObject)
    {
        m_parentObject = nullptr;
    }

    OnDependencyRemoved(removedDependency, m_dataValues);
}

void VirtualDatasheetObject::OnDependencyRemoved(const Resource* removedDependency, std::vector<VirtualDatasheetObject::DataValue*>& dataValues)
{
    for (auto dataValue : dataValues)
    {
        if (dataValue->value_objectReference == removedDependency)
        {
            dataValue->value_string = "";
            dataValue->value_objectReference = nullptr;
        }
        else if (dataValue->value_objectInstance)
        {
            dataValue->value_objectInstance->OnDependencyRemoved(removedDependency);
        }
        else
        {
            OnDependencyRemoved(removedDependency, dataValue->value_children);
        }
    }
}

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject)
{
    pugi::xml_attribute attributeType = nodeDatasheetObject.attribute("type");

    m_classDefinition = nullptr;
    if (!GetEditor()->GetDatasheetParser()->GetClassDefinition(attributeType.value(), m_classDefinition))
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "A null ClassDefinition has been provided on VirtualDatasheetObject loading");
        return false;
    }

    // Retrieve or generate object UUID.
    std::string uuid;
    if (xml::TryParseAttribute(nodeDatasheetObject, "uuid", uuid))
    {
        m_uuid = UUID::FromString(uuid);
    }
    else
    {
        m_uuid = UUID::Generate();
    }

    // Parse Data nodes.
    for (pugi::xml_node nodeData = nodeDatasheetObject.child("Data"); nodeData; nodeData = nodeData.next_sibling("Data"))
    {
        VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
        dataValue->name = nodeData.attribute("name").value();

        DatasheetParser::DataMemberDefinition* dataMemberDef = m_classDefinition->GetDataMemberDefinition(dataValue->name);
        if (dataMemberDef)
        {
            dataValue->dataMemberDefinition = dataMemberDef;

            if (!dataMemberDef->isArray)
            {
                if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                {
                    ParseInstanceDataValue(nodeData, dataMemberDef, dataValue);
                }
                else
                {
                    ParseInlineDataValue(nodeData, dataMemberDef, dataValue);
                }
            }
            else
            {
                for (pugi::xml_node nodeChild = nodeData.child("Child"); nodeChild; nodeChild = nodeChild.next_sibling("Child"))
                {
                    if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                    {
                        VirtualDatasheetObject::DataValue* childDataValue = new VirtualDatasheetObject::DataValue;
                        ParseInstanceDataValue(nodeChild, dataMemberDef, childDataValue);
                        dataValue->value_children.push_back(childDataValue);
                    }
                    else
                    {
                        VirtualDatasheetObject::DataValue* childDataValue = new VirtualDatasheetObject::DataValue;
                        ParseInlineDataValue(nodeChild, dataMemberDef, childDataValue);
                        dataValue->value_children.push_back(childDataValue);
                    }
                }
            }

            m_dataValues.push_back(dataValue);
        }
        else
        {
            // TODO: store deprecated data in a dedicated array ? add a special flag ? For now I just use a null dataMemberDef.
            pugi::xml_attribute attributeValue = nodeData.attribute("value");
            dataValue->backupValue = attributeValue.value();
            m_dataValues.push_back(dataValue);
        }
    }

    return true;
}

void VirtualDatasheetObject::ResolveInstances(std::map<UUID, VirtualDatasheetObject*>& dataObjects)
{
    for (const auto& dataValue : m_dataValues)
    {
        if (dataValue->dataMemberDefinition->isArray)
        {
            if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
            {
                for (const auto& childDataValue : dataValue->value_children)
                {
                    auto it = dataObjects.find(UUID::FromString(childDataValue->value_string));
                    if (it != dataObjects.end())
                    {
                        VirtualDatasheetObject* instanceObject = it->second;
                        dataObjects.erase(it);

                        childDataValue->value_objectInstanceDefinition = instanceObject->m_classDefinition;
                        childDataValue->value_objectInstance = instanceObject;

                        // Recurse instances resolution.
                        instanceObject->ResolveInstances(dataObjects);
                    }
                    else
                    {
                        // TODO: handle missing instances.
                    }
                }
            }
        }
        else
        {
            if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
            {
                auto it = dataObjects.find(UUID::FromString(dataValue->value_string));
                if (it != dataObjects.end())
                {
                    VirtualDatasheetObject* instanceObject = it->second;
                    dataObjects.erase(it);

                    dataValue->value_objectInstanceDefinition = instanceObject->m_classDefinition;
                    dataValue->value_objectInstance = instanceObject;

                    // Recurse instances resolution.
                    instanceObject->ResolveInstances(dataObjects);
                }
                else
                {
                    // TODO: handle missing instances.
                }
            }
        }
    }
}

void VirtualDatasheetObject::RefreshParentObject(VirtualDatasheetObject* parentObject)
{
    // TODO: Handle recursive reparenting on child objects when it is supported (this could be tied to OnDependencyRemoved future updates).
    m_parentObject = parentObject;
}

void VirtualDatasheetObject::ParseInlineDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue)
{
    pugi::xml_attribute attributeValue = nodeData.attribute("value");

    if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Bool)
    {
        dataValue->value_bool = attributeValue.as_bool();
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Int)
    {
        dataValue->value_int = attributeValue.as_int();
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Float)
    {
        dataValue->value_float = attributeValue.as_float();
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::String)
    {
        dataValue->value_string = attributeValue.value();
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Enum)
    {
        dataValue->value_string = attributeValue.value();
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Vector2i)
    {
        xml::ParseVector2i(nodeData, dataValue->value_vector2i);
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::Vector2f)
    {
        xml::ParseVector2f(nodeData, dataValue->value_vector2f);
    }
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        VirtualDatasheet* referenceDatasheet = nullptr;

        // TODO: I should encapsulate this in some kind of GetOrLoad method.
        std::string resourceID = attributeValue.value();
        if (GetResources()->IsResourceLoaded(resourceID))
        {
            referenceDatasheet = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(resourceID));
        }
        else
        {
            referenceDatasheet = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(resourceID);
        }

        // TODO: This may need to be refreshed more often, to handle created/deleted assets.
        if (referenceDatasheet && !referenceDatasheet->GetClassDefinition()->IsDerivedFromClass(dataMemberDef->objectDefinition))
        {
            referenceDatasheet = nullptr;
        }

        dataValue->value_string = resourceID;
        dataValue->value_objectReference = referenceDatasheet;
    }
}

void VirtualDatasheetObject::ParseInstanceDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue)
{
    pugi::xml_attribute instanceUUIDAttribute = nodeData.attribute("value");
    if (instanceUUIDAttribute.empty() || StringEquals(instanceUUIDAttribute.value(), ""))
    {
        // If an empty uuid is explicitly declared, the instance is forced as null (should already be null).
        dataValue->value_objectInstanceDefinition = nullptr;
        dataValue->value_objectInstance = nullptr;
        dataValue->value_string = "";
    }
    else
    {
        // Only store uuid here, wait for ResolveInstances to actually retrieve the object instance.
        dataValue->value_objectInstanceDefinition = nullptr;
        dataValue->value_objectInstance = nullptr;
        dataValue->value_string = instanceUUIDAttribute.as_string();
    }
}

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::RegisterDataValue(DatasheetParser::DataMemberDefinition* dataMemberDef)
{
    VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
    dataValue->name = dataMemberDef->name;
    dataValue->dataMemberDefinition = dataMemberDef;
    m_dataValues.push_back(dataValue);
    return dataValue;
}

bool VirtualDatasheetObject::RemoveDataValue(const std::string& name)
{
    for (size_t i = 0; i < m_dataValues.size(); ++i)
    {
        if (m_dataValues[i]->name == name)
        {
            SafeDelete(m_dataValues[i]);
            StdVectorRemoveAt(m_dataValues, i);
            return true;
        }
    }

    return false;
}

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::GetDataValue(const std::string& name, bool& isParentData) const
{
    isParentData = false;
    const VirtualDatasheetObject* dataObject = this;
    while (dataObject)
    {
        for (VirtualDatasheetObject::DataValue* dataValue : dataObject->m_dataValues)
        {
            if (dataValue->name == name)
            {
                return dataValue;
            }
        }

        isParentData = true;
        dataObject = dataObject->m_parentObject;
    }

    isParentData = false;
    return nullptr;
}

void VirtualDatasheetObject::SortDataValues()
{
    // Sort data values to match the definition ordering.
    std::sort(m_dataValues.begin(), m_dataValues.end(), [](const auto& left, const auto& right)
    {
        return left->dataMemberDefinition->name < right->dataMemberDefinition->name;
    });

    for (const auto& dataValue : m_dataValues)
    {
        if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            if (dataValue->dataMemberDefinition->isArray)
            {
                for (const auto& childDataValue : dataValue->value_children)
                {
                    if (childDataValue->value_objectInstance)
                    {
                        childDataValue->value_objectInstance->SortDataValues();
                    }
                }
            }
            else
            {
                if (dataValue->value_objectInstance)
                {
                    dataValue->value_objectInstance->SortDataValues();
                }
            }
        }
    }
}

void VirtualDatasheetObject::GatherObjectsToSave(std::vector<VirtualDatasheetObject*>& objects) const
{
    for (const auto& dataValue : m_dataValues)
    {
        if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            if (dataValue->dataMemberDefinition->isArray)
            {
                for (const auto& childDataValue : dataValue->value_children)
                {
                    if (childDataValue->value_objectInstance)
                    {
                        objects.push_back(childDataValue->value_objectInstance);

                        // Recurse instances gathering.
                        childDataValue->value_objectInstance->GatherObjectsToSave(objects);
                    }
                }
            }
            else
            {
                if (dataValue->value_objectInstance)
                {
                    objects.push_back(dataValue->value_objectInstance);

                    // Recurse instances gathering.
                    dataValue->value_objectInstance->GatherObjectsToSave(objects);
                }
            }
        }
    }
}

bool VirtualDatasheetObject::SaveToXml(pugi::xml_node& nodeDatasheet, bool isRoot) const
{
    // Serialize object.
    pugi::xml_node nodeDatasheetObject = nodeDatasheet.append_child(isRoot ? "RootObject" : "Object");
    nodeDatasheetObject.append_attribute("type") = m_classDefinition->m_name.c_str();
    nodeDatasheetObject.append_attribute("uuid")= m_uuid.ToString().c_str();

    for (const VirtualDatasheetObject::DataValue* dataValue : m_dataValues)
    {
        if (!dataValue->dataMemberDefinition)
        {
            // TODO: Add a flag to keep deprecated data ?
            // - It would be inconsistant between properties being kept but instances and arrays being lost.
            //// Store deprecated data as string.
            //pugi::xml_node nodeData = nodeDatasheetObject.append_child("Data");
            //nodeData.append_attribute("name") = dataValue->name.c_str();
            //nodeData.append_attribute("value") = dataValue->backupValue.c_str();
        }
        else
        {
            pugi::xml_node nodeData = nodeDatasheetObject.append_child("Data");
            nodeData.append_attribute("name") = dataValue->name.c_str();

            if (!dataValue->dataMemberDefinition->isArray)
            {
                if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                {
                    SaveInstanceDataValue(nodeDatasheet, nodeData, dataValue, dataValue->dataMemberDefinition->objectDefinition);
                }
                else
                {
                    SaveInlineDataValue(nodeData, dataValue, dataValue->dataMemberDefinition->type);
                }
            }
            else
            {
                for (const VirtualDatasheetObject::DataValue* childDataValue : dataValue->value_children)
                {
                    if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                    {
                        pugi::xml_node nodeChild = nodeData.append_child("Child");
                        SaveInstanceDataValue(nodeDatasheet, nodeChild, childDataValue, dataValue->dataMemberDefinition->objectDefinition);
                    }
                    else
                    {
                        pugi::xml_node nodeChild = nodeData.append_child("Child");
                        SaveInlineDataValue(nodeChild, childDataValue, dataValue->dataMemberDefinition->type);
                    }
                }
            }
        }
    }

    return true;
}

void VirtualDatasheetObject::SaveInlineDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::DataMemberDefinition::Type memberType) const
{
    if (memberType == DatasheetParser::DataMemberDefinition::Bool)
    {
        nodeData.append_attribute("value") = dataValue->value_bool;
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::Int)
    {
        nodeData.append_attribute("value") = dataValue->value_int;
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::Float)
    {
        nodeData.append_attribute("value") = dataValue->value_float;
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::String)
    {
        nodeData.append_attribute("value") = dataValue->value_string.c_str();
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::Enum)
    {
        nodeData.append_attribute("value") = dataValue->value_string.c_str();
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::Vector2i)
    {
        xml::WriteVector2i(nodeData, dataValue->value_vector2i);
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::Vector2f)
    {
        xml::WriteVector2f(nodeData, dataValue->value_vector2f);
    }
    else if (memberType == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        //nodeData.append_attribute("value") = dataValue->value_objectReference ? dataValue->value_objectReference->GetID().c_str() : "";
        nodeData.append_attribute("value") = dataValue->value_string.c_str();
    }
}

void VirtualDatasheetObject::SaveInstanceDataValue(pugi::xml_node& nodeDatasheet, pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::ClassDefinition* classDefinition) const
{
    if (dataValue->value_objectInstanceDefinition)
    {
        //nodeData.append_attribute("value") = dataValue->value_objectInstance->m_uuid.ToString().c_str();
        nodeData.append_attribute("value") = dataValue->value_string.c_str();
    }
    else
    {
        // If the definition is null, then the instance itself is null, we use an empty value to explicitely declare that.
        nodeData.append_attribute("value") = "";
    }
}


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

bool VirtualDatasheet::InstanciateRootObject(DatasheetParser::ClassDefinition* classDefinition)
{
    if (m_rootObject)
    {
        return false;
    }

    UUID newInstanceUuid = UUID::Generate();

    m_rootObject = new VirtualDatasheetObject;
    m_rootObject->m_uuid = newInstanceUuid;
    m_rootObject->m_classDefinition = classDefinition;

    return true;
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
}

void VirtualDatasheet::Unload()
{
    SafeDelete(m_rootObject);

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

    // Parse Object nodes.
    std::map<UUID, VirtualDatasheetObject*> dataObjects;
    UUID rootUuid;
    
    for (pugi::xml_node nodeObject = datasheetNode.child("Object"); nodeObject; nodeObject = nodeObject.next_sibling("Object"))
    {
        VirtualDatasheetObject* dataObject = new VirtualDatasheetObject;

        if (dataObject->LoadFromXml(nodeObject))
        {
            dataObjects.insert(std::make_pair(dataObject->m_uuid, dataObject));
        }
        else
        {
            // TODO: log error ?
            SafeDelete(dataObject);
        }
    }

    // Resolve instance links.
    m_rootObject->ResolveInstances(dataObjects);

    // Delete unreferenced objects.
    for (auto it : dataObjects)
    {
        // TODO: log error ?
        SafeDelete(it.second);
    }

    dataObjects.clear();

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

    // Retrieve and sort objects to save.
    std::vector<VirtualDatasheetObject*> pendingObjects;
    m_rootObject->GatherObjectsToSave(pendingObjects);

    std::sort(pendingObjects.begin(), pendingObjects.end(), [](const auto& left, const auto& right)
    {
        return left->m_uuid < right->m_uuid;
    });

    // Serialize all objects to xml.
    if (!m_rootObject->SaveToXml(nodeDatasheet, true))
        return false;

    for (size_t i = 0; i < pendingObjects.size(); ++i)
    {
        if (!pendingObjects[i]->SaveToXml(nodeDatasheet, false))
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
