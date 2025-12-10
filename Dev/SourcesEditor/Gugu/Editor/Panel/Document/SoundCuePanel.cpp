////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/SoundCuePanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/AudioMixerGroup.h"
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
    bool updated = false;

    if (ImGui::Button("Play"))
    {
        GetAudio()->PlaySoundCue(m_soundCue, m_playPosition);
    }

    ImGui::SameLine();
    if (ImGui::Button("Play (no random offsets)"))
    {
        SoundParameters parameters;
        if (m_soundCue->GetRandomSound(parameters))
        {
            parameters.volumeRandomRange = Vector2::Zero_f;
            parameters.pitchRandomRange = Vector2::Zero_f;
            parameters.position = m_playPosition;
            GetAudio()->PlaySound(parameters);
        }
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.f);
    ImGui::InputFloat2("Play Position", &m_playPosition);

    ImGui::Spacing();

    std::string mixerGroupID = m_soundCue->GetMixerGroup() == nullptr ? "" : m_soundCue->GetMixerGroup()->GetID();
    if (ImGui::InputText("MixerGroup", &mixerGroupID, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        m_soundCue->SetMixerGroup((mixerGroupID == "") ? nullptr : GetResources()->GetAudioMixerGroup(mixerGroupID));
        updated |= true;
    }

    ImGui::Spacing();

    float volumeAttenuation = m_soundCue->GetVolumeAttenuation();
    if (ImGui::SliderFloat("Volume Attenuation", &volumeAttenuation, 0.f, 1.f))
    {
        m_soundCue->SetVolumeAttenuation(volumeAttenuation);
        updated |= true;
    }

    ImGui::Spacing();

    {
        const char* tooltipValue = "Range used to compute a random multiplier on volume : volume x (1 + random(min, max))";
        const float sliderSize = 200;

        Vector2f range = m_soundCue->GetVolumeRandomRange();
        ImGui::PushItemWidth(sliderSize);
        bool updatedRange = ImGui::SliderFloat("##_VOLUME_RANDOM_RANGE_X", &range.x, -1.f, 0.f);
        ImGui::SetItemTooltip(tooltipValue);
        ImGui::PopItemWidth();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SameLine();
        ImGui::PushItemWidth(sliderSize);
        updatedRange |= ImGui::SliderFloat("Volume Random Range##_VOLUME_RANDOM_RANGE_Y", &range.y, 0.f, 1.f);
        ImGui::SetItemTooltip(tooltipValue);
        ImGui::PopItemWidth();
        if (updatedRange)
        {
            m_soundCue->SetVolumeRandomRange(range);
            updated |= true;
        }
    }

    {
        const char* tooltipValue = "Range used to compute a random multiplier on pitch : pitch x (1 + random(min, max))";
        const float sliderSize = 200;

        Vector2f range = m_soundCue->GetPitchRandomRange();
        ImGui::PushItemWidth(sliderSize);
        bool updatedRange = ImGui::SliderFloat("##_PITCH_RANDOM_RANGE_X", &range.x, -1.f, 0.f);
        ImGui::SetItemTooltip(tooltipValue);
        ImGui::PopItemWidth();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SameLine();
        ImGui::PushItemWidth(sliderSize);
        updatedRange |= ImGui::SliderFloat("Pitch Random Range##_PITCH_RANDOM_RANGE_Y", &range.y, 0.f, 1.f);
        ImGui::SetItemTooltip(tooltipValue);
        ImGui::PopItemWidth();
        if (updatedRange)
        {
            m_soundCue->SetPitchRandomRange(range);
            updated |= true;
        }
    }

    ImGui::Spacing();

    bool spatialized = m_soundCue->IsSpatialized();
    if (ImGui::Checkbox("Spatialized", &spatialized))
    {
        m_soundCue->SetSpatialized(spatialized);
        updated |= true;
    }

    // Spatialization parameters
    SpatializationParameters spatializationParameters = m_soundCue->GetSpatializationParameters();

    ImGui::SameLine();
    if (ImGui::Checkbox("Override Parameters", &spatializationParameters.override))
    {
        m_soundCue->SetSpatializationParameters(spatializationParameters);
        updated |= true;
    }

    if (spatializationParameters.override)
    {
        if (ImGui::InputFloat("Attenuation", &spatializationParameters.attenuation))
        {
            m_soundCue->SetSpatializationParameters(spatializationParameters);
            updated |= true;
        }

        if (ImGui::InputFloat("Min Distance", &spatializationParameters.minDistance))
        {
            m_soundCue->SetSpatializationParameters(spatializationParameters);
            updated |= true;
        }
    }
    else
    {
        // Display default MixerGroup values.
        ImGui::BeginDisabled(true);
        float attenuation = 0.5f;
        float minDistance = 200.f;
        ImGui::InputFloat("Attenuation", &attenuation);
        ImGui::InputFloat("Min Distance", &minDistance);
        ImGui::EndDisabled();
    }

    ImGui::Spacing();

    // Note: NoSavedSettings is already applied on the whole document panel, but I keep it here to match property tables.
    ImGuiTableFlags tableFlags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("_SOUNDS_TABLE", 3, tableFlags))
    {
        ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("#", columnFlags, 30.f);
        ImGui::TableSetupColumn("audio clip", columnFlags, 200.f);
        ImGui::TableSetupColumn("volume", columnFlags, 70.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        size_t count = m_soundCue->GetSoundCount();
        for (size_t i = 0; i < count; ++i)
        {
            ImGui::PushID((int)i);

            float row_min_height = ImGui::GetFrameHeight();
            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

            SoundCue::ClipEntry clip;
            m_soundCue->GetClip(i, clip);

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
            ImGui::Text(clip.audioClipId.c_str());

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%.03f", clip.volume);

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    // Finalize
    if (updated)
    {
        RaiseDirty();
    }
}

void SoundCuePanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
}

}   //namespace gugu
