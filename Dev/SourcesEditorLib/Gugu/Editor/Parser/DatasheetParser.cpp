////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Resources/VirtualDatasheet.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetParser::DataMemberDefinition* DatasheetParser::ClassDefinition::GetDataMemberDefinition(const std::string& name) const
{
    const ClassDefinition* classDefinition = this;
    while (classDefinition)
    {
        for (DatasheetParser::DataMemberDefinition* dataMemberDef : classDefinition->dataMembers)
        {
            if (dataMemberDef->name == name)
            {
                return dataMemberDef;
            }
        }

        classDefinition = classDefinition->baseDefinition;
    }

    return nullptr;
}

bool DatasheetParser::ClassDefinition::IsDerivedFromClass(const ClassDefinition* parentClassDefinition) const
{
    const ClassDefinition* baseClassDefinition = this;
    while (baseClassDefinition)
    {
        if (baseClassDefinition == parentClassDefinition)
        {
            return true;
        }

        baseClassDefinition = baseClassDefinition->baseDefinition;
    }

    return false;
}


DatasheetParser::DatasheetParser()
{
}

DatasheetParser::~DatasheetParser()
{
    ClearBinding();
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

    // Parse all enums.
    for (pugi::xml_node nodeEnum = nodeBinding.child("Enum"); nodeEnum; nodeEnum = nodeEnum.next_sibling("Enum"))
    {
        EnumDefinition* enumDefinition = new EnumDefinition;
        enumDefinition->name = nodeEnum.attribute("name").value();

        for (pugi::xml_node nodeEnumValue = nodeEnum.child("Value"); nodeEnumValue; nodeEnumValue = nodeEnumValue.next_sibling("Value"))
        {
            std::string enumValue = nodeEnumValue.attribute("name").value();
            enumDefinition->values.push_back(enumValue);
        }

        m_enumDefinitions.push_back(enumDefinition);
    }

    // Parse all classes.
    for (pugi::xml_node nodeClass = nodeBinding.child("Class"); nodeClass; nodeClass = nodeClass.next_sibling("Class"))
    {
        ClassDefinition* classDefinition = new ClassDefinition;
        classDefinition->m_name = nodeClass.attribute("name").value();
        classDefinition->baseName = nodeClass.attribute("base").value();
        classDefinition->baseDefinition = nullptr;

        m_classDefinitions.push_back(classDefinition);
    }

    // Resolve base class definitions.
    for (ClassDefinition* classDefinition : m_classDefinitions)
    {
        GetClassDefinition(classDefinition->baseName, classDefinition->baseDefinition);
    }

    // Gather inheritance trees.
    for (ClassDefinition* classDefinition : m_classDefinitions)
    {
        ClassDefinition* baseDefinition = classDefinition;
        while (baseDefinition != nullptr)
        {
            StdVectorPushFront(classDefinition->m_combinedInheritedClasses, baseDefinition);
            baseDefinition->m_derivedClasses.push_back(classDefinition);

            baseDefinition = baseDefinition->baseDefinition;
        }
    }

    // Parse all classes data.
    for (pugi::xml_node nodeClass = nodeBinding.child("Class"); nodeClass; nodeClass = nodeClass.next_sibling("Class"))
    {
        // TODO: Add a LOT of error messages and checks.
        ClassDefinition* classDefinition;
        if (GetClassDefinition(nodeClass.attribute("name").value(), classDefinition))
        {
            for (pugi::xml_node nodeClassData = nodeClass.child("Data"); nodeClassData; nodeClassData = nodeClassData.next_sibling("Data"))
            {
                DataMemberDefinition* dataDefinition = new DataMemberDefinition;
                dataDefinition->name = nodeClassData.attribute("name").value();

                std::string parseType = nodeClassData.attribute("type").value();
                pugi::xml_attribute attributeDefaultValue = nodeClassData.attribute("default");

                DataMemberDefinition::Type dataType = DataMemberDefinition::Unknown;
                std::string deducedType = parseType;

                std::vector<std::string> tokens;
                StdStringSplit(parseType, ":", tokens);
                if (tokens.size() >= 2 && tokens[0] == "array")
                {
                    dataDefinition->isArray = true;

                    if (tokens.size() == 2)
                    {
                        deducedType = tokens[1];
                    }
                    else if (tokens.size() == 3 && tokens[1] == "reference")
                    {
                        dataType = DataMemberDefinition::ObjectReference;
                        deducedType = tokens[2];
                    }
                    else if (tokens.size() == 3 && tokens[1] == "instance")
                    {
                        dataType = DataMemberDefinition::ObjectInstance;
                        deducedType = tokens[2];
                    }
                }
                else if (tokens.size() == 2 && tokens[0] == "reference")
                {
                    dataType = DataMemberDefinition::ObjectReference;
                    deducedType = tokens[1];
                }
                else if (tokens.size() == 2 && tokens[0] == "instance")
                {
                    dataType = DataMemberDefinition::ObjectInstance;
                    deducedType = tokens[1];
                }

                if (dataType == DataMemberDefinition::Unknown)
                {
                    if (deducedType == "bool")
                    {
                        dataType = DataMemberDefinition::Bool;
                    }
                    else if (deducedType == "int")
                    {
                        dataType = DataMemberDefinition::Int;
                    }
                    else if (deducedType == "float")
                    {
                        dataType = DataMemberDefinition::Float;
                    }
                    else if (deducedType == "string")
                    {
                        dataType = DataMemberDefinition::String;
                    }
                }

                if (dataType == DataMemberDefinition::Unknown && GetEnumDefinition(deducedType, dataDefinition->enumDefinition))
                {
                    dataType = DataMemberDefinition::Enum;
                }
                
                if (dataType == DataMemberDefinition::ObjectInstance || dataType == DataMemberDefinition::ObjectReference)
                {
                    GetClassDefinition(deducedType, dataDefinition->objectDefinition);
                }

                dataDefinition->type = dataType;

                if (!dataDefinition->isArray)
                {
                    if (dataDefinition->type == DataMemberDefinition::Bool)
                    {
                        dataDefinition->defaultValue_bool = attributeDefaultValue.as_bool();
                    }
                    else if (dataDefinition->type == DataMemberDefinition::Int)
                    {
                        dataDefinition->defaultValue_int = attributeDefaultValue.as_int();
                    }
                    else if (dataDefinition->type == DataMemberDefinition::Float)
                    {
                        dataDefinition->defaultValue_float = attributeDefaultValue.as_float();
                    }
                    else if (dataDefinition->type == DataMemberDefinition::String)
                    {
                        dataDefinition->defaultValue_string = attributeDefaultValue.value();
                    }
                    else if (dataDefinition->type == DataMemberDefinition::Enum)
                    {
                        dataDefinition->defaultValue_string = attributeDefaultValue.value();
                    }
                }

                classDefinition->dataMembers.push_back(dataDefinition);
            }
        }
    }

    return true;
}

void DatasheetParser::ClearBinding()
{
    for (ClassDefinition* classDefinition : m_classDefinitions)
    {
        ClearStdVector(classDefinition->dataMembers);
    }

    ClearStdVector(m_classDefinitions);
    ClearStdVector(m_enumDefinitions);
}

bool DatasheetParser::IsDatasheet(const FileInfo& fileInfo) const
{
    for (ClassDefinition* classDefinition : m_classDefinitions)
    {
        if (fileInfo.GetExtension() == classDefinition->m_name)
        {
            return true;
        }
    }

    return false;
}

bool DatasheetParser::GetEnumDefinition(const std::string& name, EnumDefinition*& enumDefinition) const
{
    for (EnumDefinition* definition : m_enumDefinitions)
    {
        if (definition->name == name)
        {
            enumDefinition = definition;
            return true;
        }
    }

    return false;
}

bool DatasheetParser::GetClassDefinition(const std::string& name, ClassDefinition*& classDefinition) const
{
    for (ClassDefinition* definition : m_classDefinitions)
    {
        if (definition->m_name == name)
        {
            classDefinition = definition;
            return true;
        }
    }

    return false;
}

VirtualDatasheet* DatasheetParser::InstanciateDatasheetResource(const std::string& resourceID)
{
    // TODO: Maybe I can use the manager factory delegate instead, along with a datasheet type detection delegate ?
    if (GetResources()->HasResource(resourceID) && !GetResources()->IsResourceLoaded(resourceID))
    {
        // TODO: Maybe I should test the datasheet type against a provided base type, to avoid loading invalid references ?
        VirtualDatasheet* datasheet = new VirtualDatasheet;
        if (GetResources()->InjectResource(resourceID, datasheet))
        {
            // TODO: I should move this part inside LoadFromFile, and move the LoadFromFile call inside InjectResource.
            std::string className = datasheet->GetFileInfo().GetExtension();
            if (GetClassDefinition(className, datasheet->m_classDefinition))
            {
                datasheet->LoadFromFile();
                return datasheet;
            }
        }

        // Safety, should not happen, just in case.
        SafeDelete(datasheet);
    }

    return nullptr;
}

}   //namespace gugu
