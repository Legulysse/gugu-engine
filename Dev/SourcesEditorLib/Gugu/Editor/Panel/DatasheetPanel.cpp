////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DatasheetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DatasheetPanel::DatasheetPanel(const std::string& resourceID, const FileInfo& resourceFileInfo)
{
    m_resourceID = resourceID;
    m_resourceFileInfo = resourceFileInfo;
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

        std::string className = m_resourceFileInfo.GetExtension();
        DatasheetParser::ClassDefinition classDefinition;
        if (GetEditor()->GetDatasheetParser()->GetClassDefinition(className, classDefinition))
        {
            for (const DatasheetParser::ClassDataDefinition& dataDefinition : classDefinition.dataMembers)
            {
                ImGui::Text(dataDefinition.name.c_str());
                ImGui::Text(dataDefinition.type.c_str());
                ImGui::Text(dataDefinition.defaultValue.c_str());
            }
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
