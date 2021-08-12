////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

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

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* parentObject)
{
    m_parentObject = parentObject;
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
    // If the definition is null, then the instance itself is null.
    DatasheetParser::ClassDefinition* instanceDefinition = nullptr;
    if (GetEditor()->GetDatasheetParser()->GetClassDefinition(nodeData.attribute("type").value(), instanceDefinition))
    {
        VirtualDatasheetObject* instanceObject = new VirtualDatasheetObject;
        instanceObject->LoadFromXml(nodeData, instanceDefinition, nullptr);

        dataValue->value_objectInstanceDefinition = instanceDefinition;
        dataValue->value_objectInstance = instanceObject;
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
        pugi::xml_node nodeData = nodeDatasheetObject.append_child("Data");
        nodeData.append_attribute("name") = dataValue->name.c_str();

        if (!dataValue->dataMemberDefinition)
        {
            // Store deprecated data as string.
            nodeData.append_attribute("value") = dataValue->backupValue.c_str();
        }
        else
        {
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
    else if (memberType == DatasheetParser::DataMemberDefinition::ObjectReference)
    {
        //nodeData.append_attribute("value") = dataValue->value_objectReference ? dataValue->value_objectReference->GetID().c_str() : "";
        nodeData.append_attribute("value") = dataValue->value_string.c_str();
    }
}

void VirtualDatasheetObject::SaveInstanceDataValue(pugi::xml_node& nodeData, const VirtualDatasheetObject::DataValue* dataValue, DatasheetParser::ClassDefinition* classDefinition) const
{
    // If the definition is null, then the instance itself is null.
    if (dataValue->value_objectInstanceDefinition)
    {
        nodeData.append_attribute("type") = dataValue->value_objectInstanceDefinition->m_name.c_str();
        dataValue->value_objectInstance->SaveToXml(nodeData);
    }
}


VirtualDatasheet::VirtualDatasheet()
    : m_classDefinition(nullptr)
    , m_rootObject(nullptr)
    , m_parentDatasheet(nullptr)
{
}

VirtualDatasheet::~VirtualDatasheet()
{
    SafeDelete(m_rootObject);
    m_classDefinition = nullptr;
    m_parentDatasheet = nullptr;
}

EResourceType::Type VirtualDatasheet::GetResourceType() const
{
    return EResourceType::Datasheet;    // TODO: Should I use Custom ? Or a dedicated VirtualDatasheet enum value ?
}

bool VirtualDatasheet::LoadFromFile()
{
    pugi::xml_document xmlDocument;
    pugi::xml_parse_result result = xmlDocument.load_file(GetFileInfoRef().GetPathName().c_str());
    if (!result)
        return false;

    pugi::xml_node nodeDatasheetObject = xmlDocument.child("Datasheet");
    if (!nodeDatasheetObject)
        return false;

    m_rootObject = new VirtualDatasheetObject;

    pugi::xml_attribute attributeParent = nodeDatasheetObject.attribute("parent");
    if (attributeParent)
    {
        // TODO: I should encapsulate this in some kind of GetOrLoad method.
        std::string parentResourceID = attributeParent.value();
        if (GetResources()->IsResourceLoaded(parentResourceID))
        {
            m_parentDatasheet = dynamic_cast<VirtualDatasheet*>(GetResources()->GetResource(parentResourceID));
        }
        else
        {
            m_parentDatasheet = GetEditor()->GetDatasheetParser()->InstanciateDatasheetResource(parentResourceID);
        }
    }

    return m_rootObject->LoadFromXml(nodeDatasheetObject, m_classDefinition, m_parentDatasheet ? m_parentDatasheet->m_rootObject : nullptr);
}

bool VirtualDatasheet::SaveToFile()
{
    pugi::xml_document xmlDocument;
    pugi::xml_node nodeDatasheetObject = xmlDocument.append_child("Datasheet");

    if (m_parentDatasheet)
    {
        nodeDatasheetObject.append_attribute("parent") = m_parentDatasheet->GetID().c_str();
    }

    if (!m_rootObject->SaveToXml(nodeDatasheetObject))
        return false;

    return xmlDocument.save_file(GetFileInfoRef().GetPathName().c_str());
}

}   // namespace gugu
