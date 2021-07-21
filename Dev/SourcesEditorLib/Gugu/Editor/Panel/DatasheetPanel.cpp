////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"
#include "Gugu/Editor/Resources/VirtualDatasheet.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetPanel::DatasheetPanel(VirtualDatasheet* datasheet)
{
    m_resourceID = datasheet->GetID();
    m_datasheet = datasheet;

    m_title = m_resourceID;

    // Parse definition.
    ClassDefinitionEntry classEntry;
    classEntry.classDefinition = m_datasheet->m_classDefinition;
    m_classEntries.push_back(classEntry);

    DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->m_classDefinition->baseDefinition;
    while (baseDefinition != nullptr)
    {
        ClassDefinitionEntry baseClassEntry;
        baseClassEntry.classDefinition = baseDefinition;
        m_classEntries.insert(m_classEntries.begin(), baseClassEntry);

        baseDefinition = baseDefinition->baseDefinition;
    }
}

DatasheetPanel::~DatasheetPanel()
{
}

void DatasheetPanel::UpdatePanel(const DeltaTime& dt)
{
    m_focused = false;

    ImGuiWindowFlags flags = 0;
    if (m_dirty)
    {
        flags |= ImGuiWindowFlags_UnsavedDocument;
    }

    if (ImGui::Begin(m_title.c_str(), false, flags))
    {
        if (ImGui::IsWindowFocused())
        {
            m_focused = true;
        }

        // TODO: I should reverse-parse the bases to go top-bottom in the hierarchy.
        DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->m_classDefinition->baseDefinition;
        while (baseDefinition != nullptr)
        {
            ImGui::Text("---- base: %s ----", baseDefinition->m_name.c_str());

            for (DatasheetParser::DataMemberDefinition* dataDefinition : baseDefinition->dataMembers)
            {
                ImGui::Text(dataDefinition->name.c_str());
                //ImGui::Text(dataDefinition->type.c_str());
                //ImGui::Text(dataDefinition->defaultValue.c_str());
            }

            baseDefinition = baseDefinition->baseDefinition;
        }

        ImGui::Text("---- class: %s ----", m_datasheet->m_rootObject->m_classDefinition->m_name.c_str());

        for (DatasheetParser::DataMemberDefinition* dataDefinition : m_datasheet->m_rootObject->m_classDefinition->dataMembers)
        {
            ImGui::Text(dataDefinition->name.c_str());
            //ImGui::Text(dataDefinition->type.c_str());
            //ImGui::Text(dataDefinition->defaultValue.c_str());
        }

        ImGui::Text("---- values ----");

        for (VirtualDatasheetObject::DataValue* dataValue : m_datasheet->m_rootObject->m_dataValues)
        {
            ImGui::Text(dataValue->name.c_str());
            //ImGui::Text(dataValue->value.c_str());
        }
    }
    ImGui::End();
}

void DatasheetPanel::UpdateProperties(const gugu::DeltaTime& dt)
{
    std::string dummyParent = m_datasheet->m_parentDatasheet ? "Ref: " + m_datasheet->m_parentDatasheet->GetID() : "";
    ImGui::InputText("parent##root", &dummyParent);

    for (ClassDefinitionEntry classEntry : m_classEntries)
    {
        ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader(classEntry.classDefinition->m_name.c_str(), headerFlags))
        {
            //ImGui::Indent();

            // TODO: imgui will probably need me to inject IDs to ensure fields are all unique in the stack.
            DisplayDataClass(classEntry.classDefinition, m_datasheet->m_rootObject);

            //ImGui::Unindent();
        }
    }
}

void DatasheetPanel::DisplayDataClass(DatasheetParser::ClassDefinition* classDefinition, VirtualDatasheetObject* dataObject)
{
    for (DatasheetParser::DataMemberDefinition* dataMemberDef : classDefinition->dataMembers)
    {
        DisplayDataMember(dataMemberDef, dataObject);
    }

    if (classDefinition->dataMembers.empty())
    {
        ImGui::Text("default");
        ImGui::SameLine();
        ImGui::Text("No Data Member");
    }
}

void DatasheetPanel::DisplayDataMember(DatasheetParser::DataMemberDefinition* dataMemberDefinition, VirtualDatasheetObject* dataObject)
{
    bool isParentData = false;
    VirtualDatasheetObject::DataValue* dataValue = dataObject->GetDataValue(dataMemberDefinition->name, isParentData);

    ImGui::Text(dataValue ? (isParentData ? "parent " : "self   ") : "default");
    ImGui::SameLine();

    if (!dataMemberDefinition->isArray)
    {
        if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Bool)
        {
            bool dummy = dataValue ? dataValue->value_bool : dataMemberDefinition->defaultValue_bool;
            ImGui::Checkbox(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Int)
        {
            int dummy = dataValue ? dataValue->value_int : dataMemberDefinition->defaultValue_int;
            ImGui::InputInt(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Float)
        {
            float dummy = dataValue ? dataValue->value_float : dataMemberDefinition->defaultValue_float;
            ImGui::InputFloat(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::String)
        {
            std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
            ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::Enum)
        {
            std::string dummy = dataValue ? dataValue->value_string : dataMemberDefinition->defaultValue_string;
            ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectReference)
        {
            std::string objectDefinition = dataMemberDefinition->objectDefinition ? dataMemberDefinition->objectDefinition->m_name : "Invalid Definition";
            std::string dummy = dataValue && dataValue->value_objectReference ? "Ref: " + dataValue->value_objectReference->GetID() : StringFormat("Empty Ref (Def: {0})", objectDefinition);
            ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
        }
        else if (dataMemberDefinition->type == DatasheetParser::DataMemberDefinition::ObjectInstance)
        {
            if (dataValue)
            {
                // TODO: force PushDisable for instanced data if the data comes from the parent.
                std::string objectDefinition = dataValue->value_objectInstanceDefinition ? dataValue->value_objectInstanceDefinition->m_name : "Invalid Definition";
                std::string dummy = StringFormat("Instance (Def: {0})", objectDefinition);
                ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);

                ImGui::Indent();
                DisplayDataClass(dataValue->value_objectInstanceDefinition, dataValue->value_objectInstance);
                ImGui::Unindent();
            }
            else
            {
                std::string objectDefinition = dataMemberDefinition->objectDefinition ? dataMemberDefinition->objectDefinition->m_name : "Invalid Definition";
                std::string dummy = StringFormat("Empty Instance (Def: {0})", objectDefinition);
                ImGui::InputText(dataMemberDefinition->name.c_str(), &dummy);
            }
        }
    }
    else
    {
        ImGui::Text("array");
        ImGui::SameLine();
        ImGui::Text(dataMemberDefinition->name.c_str());
    }
}

bool DatasheetPanel::Save()
{
    return !m_dirty;
}

}   //namespace gugu
