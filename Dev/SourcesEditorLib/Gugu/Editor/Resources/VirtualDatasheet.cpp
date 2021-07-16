////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

VirtualDatasheetObject::DataValue::~DataValue()
{
    SafeDelete(value_objectInstance);
    ClearStdVector(value_children);

    overrideObjectDefinition = nullptr;
    value_objectReference = nullptr;
}

VirtualDatasheetObject::VirtualDatasheetObject()
    : m_classDefinition(nullptr)
{
}

VirtualDatasheetObject::~VirtualDatasheetObject()
{
    m_classDefinition = nullptr;
    ClearStdVector(m_dataValues);
}

bool VirtualDatasheetObject::LoadFromXml(const pugi::xml_node& nodeDatasheetObject, DatasheetParser::ClassDefinition* classDefinition)
{
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

VirtualDatasheetObject::DataValue* VirtualDatasheetObject::GetDataValue(const std::string& name) const
{
    // TODO: parse parent data, return a flag to indicate its a parent data.
    for (VirtualDatasheetObject::DataValue* dataValue : m_dataValues)
    {
        if (dataValue->name == name)
        {
            return dataValue;
        }
    }

    return nullptr;
}

VirtualDatasheet::VirtualDatasheet()
    : rootObject(nullptr)
{
}

VirtualDatasheet::~VirtualDatasheet()
{
    SafeDelete(rootObject);
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

    rootObject = new VirtualDatasheetObject;

    pugi::xml_attribute attributeParent = nodeDatasheetObject.attribute("parent");
    if (attributeParent)
    {
        // TODO: read parent data.
    }

    return rootObject->LoadFromXml(nodeDatasheetObject, classDefinition);
}

}   // namespace gugu
