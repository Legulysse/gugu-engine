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
        pugi::xml_attribute attributeValue = nodeData.attribute("value");

        DatasheetParser::DataMemberDefinition* dataMemberDef = m_classDefinition->GetDataMemberDefinition(dataValue->name);
        if (dataMemberDef)
        {
            if (!dataMemberDef->isArray)
            {
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
                else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::String || dataMemberDef->type == DatasheetParser::DataMemberDefinition::Enum)
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

                    dataValue->value_objectReference = referenceDatasheet;
                }
                else if (dataMemberDef->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
                {
                    VirtualDatasheetObject* instanceObject = new VirtualDatasheetObject;
                    DatasheetParser::ClassDefinition* instanceDefinition = nullptr;

                    if (!GetEditor()->GetDatasheetParser()->GetClassDefinition(nodeData.attribute("type").value(), instanceDefinition))
                    {
                        instanceDefinition = dataMemberDef->objectDefinition;
                    }

                    instanceObject->LoadFromXml(nodeData, instanceDefinition, nullptr);

                    dataValue->value_objectInstanceDefinition = instanceDefinition;
                    dataValue->value_objectInstance = instanceObject;
                }
            }

            m_dataValues.push_back(dataValue);
        }
        else
        {
            // TODO: store deprecated data in a dedicated array ? add a special flag ?
            dataValue->backupValue = attributeValue.value();
            m_dataValues.push_back(dataValue);
        }
    }

    return true;
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

}   // namespace gugu
