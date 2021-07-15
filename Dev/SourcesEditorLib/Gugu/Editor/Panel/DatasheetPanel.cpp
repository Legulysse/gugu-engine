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

    // Parse definition.
    ClassDefinitionEntry classEntry;
    classEntry.classDefinition = m_datasheet->classDefinition;
    m_classEntries.push_back(classEntry);

    DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->classDefinition->baseDefinition;
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

    if (ImGui::Begin(m_resourceID.c_str(), false, flags))
    {
        if (ImGui::IsWindowFocused())
        {
            m_focused = true;
        }

        // TODO: I should reverse-parse the bases to go top-bottom in the hierarchy.
        DatasheetParser::ClassDefinition* baseDefinition = m_datasheet->classDefinition->baseDefinition;
        while (baseDefinition != nullptr)
        {
            ImGui::Text("---- base: %s ----", baseDefinition->name.c_str());

            for (DatasheetParser::DataMemberDefinition* dataDefinition : baseDefinition->dataMembers)
            {
                ImGui::Text(dataDefinition->name.c_str());
                //ImGui::Text(dataDefinition->type.c_str());
                //ImGui::Text(dataDefinition->defaultValue.c_str());
            }

            baseDefinition = baseDefinition->baseDefinition;
        }

        ImGui::Text("---- class: %s ----", m_datasheet->rootObject->classDefinition->name.c_str());

        for (DatasheetParser::DataMemberDefinition* dataDefinition : m_datasheet->rootObject->classDefinition->dataMembers)
        {
            ImGui::Text(dataDefinition->name.c_str());
            //ImGui::Text(dataDefinition->type.c_str());
            //ImGui::Text(dataDefinition->defaultValue.c_str());
        }

        ImGui::Text("---- values ----");

        for (VirtualDatasheetObject::DataValue* dataValue : m_datasheet->rootObject->dataValues)
        {
            ImGui::Text(dataValue->name.c_str());
            ImGui::Text(dataValue->value.c_str());
        }
    }
    ImGui::End();
}

void DatasheetPanel::UpdateProperties(const gugu::DeltaTime& dt)
{
    // TODO: display parent datasheet.

    for (ClassDefinitionEntry classEntry : m_classEntries)
    {
        ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader(classEntry.classDefinition->name.c_str(), headerFlags))
        {
            ImGui::Indent();

            for (DatasheetParser::DataMemberDefinition* dataDefinition : classEntry.classDefinition->dataMembers)
            {
                if (!dataDefinition->isArray)
                {
                    if (dataDefinition->type == DatasheetParser::DataMemberDefinition::Bool)
                    {
                        bool dummy = dataDefinition->defaultValue_bool;
                        ImGui::Checkbox(dataDefinition->name.c_str(), &dummy);
                    }
                    else if (dataDefinition->type == DatasheetParser::DataMemberDefinition::Int)
                    {
                        int dummy = dataDefinition->defaultValue_int;
                        ImGui::InputInt(dataDefinition->name.c_str(), &dummy);
                    }
                    else if (dataDefinition->type == DatasheetParser::DataMemberDefinition::Float)
                    {
                        float dummy = dataDefinition->defaultValue_float;
                        ImGui::InputFloat(dataDefinition->name.c_str(), &dummy);
                    }
                    else if (dataDefinition->type == DatasheetParser::DataMemberDefinition::String)
                    {
                        std::string dummy = dataDefinition->defaultValue_string;
                        ImGui::InputText(dataDefinition->name.c_str(), &dummy);
                    }
                    else if (dataDefinition->type == DatasheetParser::DataMemberDefinition::Enum)
                    {
                        std::string dummy = dataDefinition->defaultValue_string;
                        ImGui::InputText(dataDefinition->name.c_str(), &dummy);
                    }
                    else
                    {
                        ImGui::Text("object");
                        ImGui::SameLine();
                        ImGui::Text(dataDefinition->name.c_str());
                    }
                }
                else
                {
                    ImGui::Text("array");
                    ImGui::SameLine();
                    ImGui::Text(dataDefinition->name.c_str());
                }
            }

            ImGui::Unindent();
        }
    }
}

bool DatasheetPanel::Save()
{
    return !m_dirty;
}

}   //namespace gugu
