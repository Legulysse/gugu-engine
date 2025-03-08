////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/SoundCuePanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SoundCuePanel::SoundCuePanel(SoundCue* resource)
    : DocumentPanel(resource)
    , m_soundCue(resource)
{
}

SoundCuePanel::~SoundCuePanel()
{
}

void SoundCuePanel::UpdatePanelImpl(const DeltaTime& dt)
{
    if (ImGui::Button("Play"))
    {
        GetAudio()->PlaySoundCue(m_soundCue);
    }

    // Note: NoSavedSettings is already applied on the whole document panel, but I keep it here to match property tables.
    ImGuiTableFlags tableFlags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("_SOUNDS_TABLE", 5, tableFlags))
    {
        ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("#", columnFlags, 30.f);
        ImGui::TableSetupColumn("sound", columnFlags, 200.f);
        ImGui::TableSetupColumn("volume", columnFlags, 70.f);
        ImGui::TableSetupColumn("pitch offset-", columnFlags, 0.f);
        ImGui::TableSetupColumn("pitch offset+", columnFlags, 0.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        size_t count = m_soundCue->GetSoundCount();
        for (size_t i = 0; i < count; ++i)
        {
            ImGui::PushID((int)i);

            float row_min_height = ImGui::GetFrameHeight();
            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

            SoundParameters parameters;
            m_soundCue->GetSound(i, parameters);

            int columnIndex = 0;
            ImGui::TableSetColumnIndex(columnIndex++);

            char label[32];
            sprintf(label, "%04d", (int)i);

            ImGui::Text(label);
            //ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;
            //if (ImGui::Selectable(label, rowIndex == m_selectedIndex, selectable_flags, ImVec2(0, row_min_height)))
            //{
            //    m_selectedIndex = rowIndex;
            //}

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text(parameters.soundID.c_str());

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%.03f", parameters.volume);

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%.03f", parameters.pitchLowerOffset);

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%.03f", parameters.pitchUpperOffset);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
}

void SoundCuePanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
}

bool SoundCuePanel::SaveStateImpl(std::string& result)
{
    // Currently soundcue edition is not handled.
    return false;
}

}   //namespace gugu
