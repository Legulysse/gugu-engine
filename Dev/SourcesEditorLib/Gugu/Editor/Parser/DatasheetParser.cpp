////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetParser::DatasheetParser()
{
}

DatasheetParser::~DatasheetParser()
{
}

bool DatasheetParser::ParseBinding(const std::string& pathDatasheetBinding)
{
    pugi::xml_document xmlDocument;
    pugi::xml_parse_result result = xmlDocument.load_file(pathDatasheetBinding.c_str());
    if (!result)
        return false;

    pugi::xml_node nodeBinding = xmlDocument.child("Binding");
    if (!nodeBinding)
        return false;

    for (pugi::xml_node nodeEnum = nodeBinding.child("Enum"); nodeEnum; nodeEnum = nodeEnum.next_sibling("Enum"))
    {
        EnumDefinition enumDefinition;
        enumDefinition.name = nodeEnum.attribute("name").value();

        for (pugi::xml_node nodeEnumValue = nodeEnum.child("Value"); nodeEnumValue; nodeEnumValue = nodeEnumValue.next_sibling("Value"))
        {
            std::string enumValue = nodeEnumValue.attribute("name").value();
            enumDefinition.values.push_back(enumValue);
        }

        m_enumDefinitions.push_back(enumDefinition);
    }

    for (pugi::xml_node nodeClass = nodeBinding.child("Class"); nodeClass; nodeClass = nodeClass.next_sibling("Class"))
    {
        ClassDefinition classDefinition;
        classDefinition.name = nodeClass.attribute("name").value();
        classDefinition.base = nodeClass.attribute("base").value();

        for (pugi::xml_node nodeClassData = nodeClass.child("Data"); nodeClassData; nodeClassData = nodeClassData.next_sibling("Data"))
        {
            ClassDataDefinition dataDefinition;
            dataDefinition.type = nodeClassData.attribute("type").value();
            dataDefinition.name = nodeClassData.attribute("name").value();
            dataDefinition.defaultValue = nodeClassData.attribute("default").value();

            classDefinition.dataMembers.push_back(dataDefinition);
        }

        m_classDefinitions.push_back(classDefinition);
    }

    return true;
}

void DatasheetParser::ClearBinding()
{
    m_enumDefinitions.clear();
    m_classDefinitions.clear();
}

bool DatasheetParser::IsDatasheet(const FileInfo& fileInfo) const
{
    for (auto classDefinition : m_classDefinitions)
    {
        if (fileInfo.GetExtension() == classDefinition.name)
        {
            return true;
        }
    }

    return false;
}

bool DatasheetParser::GetClassDefinition(const std::string& name, ClassDefinition& classDefinition) const
{
    for (const ClassDefinition& definition : m_classDefinitions)
    {
        if (definition.name == name)
        {
            classDefinition = definition;
            return true;
        }
    }

    return false;
}

}   //namespace gugu
