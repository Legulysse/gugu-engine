////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheetObject.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/String.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

VirtualDatasheetObject::DataValue::~DataValue()
{
    ClearStdVector(value_children);

    dataMemberDefinition = nullptr;
    value_objectInstanceDefinition = nullptr;
    value_objectInstance = nullptr;
    value_objectReference = nullptr;
}

bool VirtualDatasheetObject::DataValue::MoveChildDataValue(size_t indexFrom, size_t indexTo)
{
    DataValue* temp = value_children[indexFrom];
    StdVectorRemoveAt(value_children, indexFrom);
    StdVectorInsertAt(value_children, indexTo, temp);
    return true;
}

bool VirtualDatasheetObject::DataValue::RemoveChildDataValue(size_t index)
{
    SafeDelete(value_children[index]);
    StdVectorRemoveAt(value_children, index);
    return true;
}


VirtualDatasheetObject::VirtualDatasheetObject()
    : m_datasheet(nullptr)
    , m_parentObject(nullptr)
    , m_classDefinition(nullptr)
{
}

VirtualDatasheetObject::~VirtualDatasheetObject()
{
    m_datasheet = nullptr;
    m_parentObject = nullptr;
    m_classDefinition = nullptr;
    ClearStdVector(m_dataValues);
}

void VirtualDatasheetObject::SetParentObject(VirtualDatasheetObject* parentObject)
{
    m_parentObject = parentObject;
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
        else
        {
            GetDependencies(dataValue->value_children, dependencies);
        }
    }
}

void VirtualDatasheetObject::OnDependencyRemoved(const Resource* removedDependency)
{
    OnDependencyRemoved(removedDependency, m_dataValues);
}

void VirtualDatasheetObject::OnDependencyRemoved(const Resource* removedDependency, std::vector<VirtualDatasheetObject::DataValue*>& dataValues)
{
    for (auto dataValue : dataValues)
    {
        if (dataValue->value_objectReference == removedDependency)
        {
            // TODO: Log warning ?
            //dataValue->value_string = "";
            dataValue->value_objectReference = nullptr;
        }
        else
        {
            OnDependencyRemoved(removedDependency, dataValue->value_children);
        }
    }
}

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject, VirtualDatasheet* datasheet)
{
    pugi::xml_attribute attributeType = nodeDatasheetObject.attribute("type");

    // TODO: check that override has the same type as the parent object.
    m_classDefinition = nullptr;
    if (!GetEditor()->GetDatasheetParser()->GetClassDefinition(attributeType.value(), m_classDefinition))
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "A null ClassDefinition has been provided on VirtualDatasheetObject loading");
        return false;
    }

    // Register owning datasheet.
    m_datasheet = datasheet;

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
        DatasheetParser::DataMemberDefinition* dataMemberDef = m_classDefinition->GetDataMemberDefinition(nodeData.attribute("name").value());
        if (dataMemberDef)
        {
            VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
            dataValue->owner = this;
            dataValue->name = dataMemberDef->name;
            dataValue->dataMemberDefinition = dataMemberDef;

            if (!dataMemberDef->isArray)
            {
                ParseDataValue(nodeData, dataMemberDef, dataValue);
            }
            else
            {
                for (pugi::xml_node nodeChild = nodeData.child("Child"); nodeChild; nodeChild = nodeChild.next_sibling("Child"))
                {
                    VirtualDatasheetObject::DataValue* childDataValue = new VirtualDatasheetObject::DataValue;
                    childDataValue->owner = this;

                    dataValue->value_children.push_back(childDataValue);

                    ParseDataValue(nodeChild, dataMemberDef, childDataValue);
                }
            }

            m_dataValues.push_back(dataValue);
        }
        else
        {
            // TODO: store deprecated data in a dedicated array ? add a special flag ? add log error ?
        }
    }

    return true;
}

void VirtualDatasheetObject::ResolveInstances(const std::map<UUID, VirtualDatasheetObject*>& dataObjects, std::set<UUID>& orphanObjectUuids)
{
    for (const auto& dataValue : m_dataValues)
    {
        if (dataValue->dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            if (dataValue->dataMemberDefinition->isArray)
            {
                for (const auto& childDataValue : dataValue->value_children)
                {
                    auto it = dataObjects.find(UUID::FromString(childDataValue->value_string));
                    if (it != dataObjects.end())
                    {
                        VirtualDatasheetObject* instanceObject = it->second;
                        orphanObjectUuids.erase(it->first);

                        childDataValue->value_objectInstanceDefinition = instanceObject->m_classDefinition;
                        childDataValue->value_objectInstance = instanceObject;
                    }
                    else
                    {
                        // TODO: handle missing instances.
                    }
                }
            }
            else
            {
                auto it = dataObjects.find(UUID::FromString(dataValue->value_string));
                if (it != dataObjects.end())
                {
                    VirtualDatasheetObject* instanceObject = it->second;
                    orphanObjectUuids.erase(it->first);

                    dataValue->value_objectInstanceDefinition = instanceObject->m_classDefinition;
                    dataValue->value_objectInstance = instanceObject;
                }
                else
                {
                    // TODO: handle missing instances.
                }
            }
        }
    }
}

void VirtualDatasheetObject::RegenerateInstanceUuidsRecursively()
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
                        UUID uuid = UUID::Generate();
                        childDataValue->value_string = uuid.ToString();
                        childDataValue->value_objectInstance->m_uuid = uuid;

                        childDataValue->value_objectInstance->RegenerateInstanceUuidsRecursively();
                    }
                }
            }
            else
            {
                if (dataValue->value_objectInstance)
                {
                    UUID uuid = UUID::Generate();
                    dataValue->value_string = uuid.ToString();
                    dataValue->value_objectInstance->m_uuid = uuid;

                    dataValue->value_objectInstance->RegenerateInstanceUuidsRecursively();
                }
            }
        }
    }
}

void VirtualDatasheetObject::GatherInstanceUuidsRecursively(std::set<UUID>& instanceUuids) const
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
                        instanceUuids.insert(childDataValue->value_objectInstance->m_uuid);
                        childDataValue->value_objectInstance->GatherInstanceUuidsRecursively(instanceUuids);
                    }
                }
            }
            else
            {
                if (dataValue->value_objectInstance)
                {
                    instanceUuids.insert(dataValue->value_objectInstance->m_uuid);
                    dataValue->value_objectInstance->GatherInstanceUuidsRecursively(instanceUuids);
                }
            }
        }
    }
}

void VirtualDatasheetObject::ParseDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue)
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
    else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
    {
        if (attributeValue.empty() || StringEquals(attributeValue.value(), ""))
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
            dataValue->value_string = attributeValue.as_string();
        }
    }
}

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::InstanciateNewClassMemberDataValue(DatasheetParser::DataMemberDefinition* dataMemberDef)
{
    VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
    dataValue->owner = this;
    dataValue->name = dataMemberDef->name;
    dataValue->dataMemberDefinition = dataMemberDef;

    m_dataValues.push_back(dataValue);
    return dataValue;
}

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::InstanciateNewArrayMemberDataValue(VirtualDatasheetObject::DataValue* arrayDataMember)
{
    // TODO: set default value ?
    VirtualDatasheetObject::DataValue* dataValue = new VirtualDatasheetObject::DataValue;
    dataValue->owner = this;

    arrayDataMember->value_children.push_back(dataValue);
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

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::GetDataValue(const std::string& name) const
{
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

        dataObject = dataObject->m_parentObject;
    }

    return nullptr;
}

void VirtualDatasheetObject::SortDataValues()
{
    // Sort data values alphabetically (It seems more stable than using the definition ordering).
    std::sort(m_dataValues.begin(), m_dataValues.end(), [](const auto& left, const auto& right)
    {
        return left->dataMemberDefinition->name < right->dataMemberDefinition->name;
    });
}

bool VirtualDatasheetObject::SaveToXml(pugi::xml_node& nodeDatasheet, const std::string& xmlObjectType) const
{
    // Serialize object.
    pugi::xml_node nodeDatasheetObject = nodeDatasheet.append_child(xmlObjectType.c_str());
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
                SaveDataValue(nodeData, dataValue, dataValue->dataMemberDefinition->type);
            }
            else
            {
                for (const VirtualDatasheetObject::DataValue* childDataValue : dataValue->value_children)
                {
                    pugi::xml_node nodeChild = nodeData.append_child("Child");
                    SaveDataValue(nodeChild, childDataValue, dataValue->dataMemberDefinition->type);
                }
            }
        }
    }

    return true;
}

void VirtualDatasheetObject::SaveDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::DataMemberDefinition::Type memberType) const
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
    else if (memberType == DatasheetParser::DataMemberDefinition::ObjectInstance)
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
}

}   // namespace gugu
