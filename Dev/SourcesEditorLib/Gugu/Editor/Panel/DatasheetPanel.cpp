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

        for (DatasheetParser::DataMemberDefinition* dataDefinition : m_datasheet->rootObject->classDefinition->dataMembers)
        {
            ImGui::Text(dataDefinition->name.c_str());
            ImGui::Text(dataDefinition->type.c_str());
            ImGui::Text(dataDefinition->defaultValue.c_str());
        }

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
}

bool DatasheetPanel::Save()
{
    return !m_dirty;
}

}   //namespace gugu
