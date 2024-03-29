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

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* classDefinition)
{
    if (!classDefinition)
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, "A null ClassDefinition has been provided on VirtualDatasheetObject loading");
        return false;
    }

    m_classDefinition = classDefinition;

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
        if (referenceDatasheet && !referenceDatasheet->m_classDefinition->IsDerivedFromClass(dataMemberDef->objectDefinition))
        {
            referenceDatasheet = nullptr;
        }

        dataValue->value_string = resourceID;
        dataValue->value_objectReference = referenceDatasheet;
    }
}

void VirtualDatasheetObject::ParseInstanceDataValue(const pugi::xml_node& nodeData, DatasheetParser::DataMemberDefinition* dataMemberDef, VirtualDatasheetObject::DataValue* dataValue)
{
    DatasheetParser::ClassDefinition* instanceDefinition = nullptr;

    pugi::xml_attribute attributeType = nodeData.attribute("type");
    if (!attributeType.empty() && StringEquals(attributeType.value(), ""))
    {
        // If an empty type is explicitly declared, the instance is forced as null (should already be null).
        dataValue->value_objectInstanceDefinition = nullptr;
        dataValue->value_objectInstance = nullptr;
    }
    else
    {
        if (attributeType.empty())
        {
            // If the definition is not provided, we fallback on the default definition.
            instanceDefinition = dataMemberDef->objectDefinition;
        }

        if (instanceDefinition || GetEditor()->GetDatasheetParser()->GetClassDefinition(attributeType.value(), instanceDefinition))
        {
            VirtualDatasheetObject* instanceObject = new VirtualDatasheetObject;
            instanceObject->LoadFromXml(nodeData, instanceDefinition);

            dataValue->value_objectInstanceDefinition = instanceDefinition;
            dataValue->value_objectInstance = instanceObject;
        }
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

bool VirtualDatasheetObject::SaveToXml(pugi::xml_node& nodeDatasheetObject) const
{
    // TODO: sort m_dataValues to match the definition.
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
                    SaveInstanceDataValue(nodeData, dataValue, dataValue->dataMemberDefinition->objectDefinition);
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
                        SaveInstanceDataValue(nodeChild, childDataValue, dataValue->dataMemberDefinition->objectDefinition);
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

void VirtualDatasheetObject::SaveInstanceDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::ClassDefinition* classDefinition) const
{
    if (dataValue->value_objectInstanceDefinition)
    {
        nodeData.append_attribute("type") = dataValue->value_objectInstanceDefinition->m_name.c_str();
        dataValue->value_objectInstance->SaveToXml(nodeData);
    }
    else
    {
        // If the definition is null, then the instance itself is null, we use an empty type to explicitely declare that.
        nodeData.append_attribute("type") = "";
    }
}


VirtualDatasheet::VirtualDatasheet(DatasheetParser::ClassDefinition* classDefinition)
    : m_classDefinition(classDefinition)
    , m_rootObject(nullptr)
    , m_parentDatasheet(nullptr)
{
    m_rootObject = new VirtualDatasheetObject;
}

VirtualDatasheet::~VirtualDatasheet()
{
    Unload();

    m_classDefinition = nullptr;
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

bool VirtualDatasheet::IsValidAsParent(VirtualDatasheet* parentDatasheet, bool* invalidRecursiveParent) const
{
    if (!parentDatasheet)
    {
        // A null parent is a valid case.
        return true;
    }

    if (parentDatasheet->m_classDefinition != m_classDefinition)
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

void VirtualDatasheet::Unload()
{
    SafeDelete(m_rootObject);

    m_parentDatasheet = nullptr;
}

bool VirtualDatasheet::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    m_rootObject = new VirtualDatasheetObject;

    pugi::xml_node nodeDatasheetObject = document.child("Datasheet");
    if (!nodeDatasheetObject)
        return false;

    if (!m_rootObject->LoadFromXml(nodeDatasheetObject, m_classDefinition))
    {
        return false;
    }

    std::string parentResourceID = "";
    VirtualDatasheet* parentDatasheet = nullptr;

    pugi::xml_attribute attributeParent = nodeDatasheetObject.attribute("parent");
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
    pugi::xml_node nodeDatasheet = document.append_child("Datasheet");
    nodeDatasheet.append_attribute("serializationVersion") = 1;

    // Binding version, could be used for application binding changes.
    nodeDatasheet.append_attribute("bindingVersion") = 1;

    if (!m_parentDatasheetID.empty())
    {
        nodeDatasheet.append_attribute("parent") = m_parentDatasheetID.c_str();
    }

    if (m_rootObject && !m_rootObject->SaveToXml(nodeDatasheet))
        return false;

    return true;
}

}   // namespace gugu
