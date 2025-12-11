////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/AudioMixerGroupPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioMixerGroupPanel::AudioMixerGroupPanel(AudioMixerGroup* resource)
    : DocumentPanel(resource)
    , m_mixerGroup(resource)
{
}

AudioMixerGroupPanel::~AudioMixerGroupPanel()
{
}

void AudioMixerGroupPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    bool updated = false;

    float volumeAttenuation = m_mixerGroup->GetVolumeAttenuation();
    if (ImGui::SliderFloat("Volume Attenuation", &volumeAttenuation, 0.f, 1.f))
    {
        m_mixerGroup->SetVolumeAttenuation(volumeAttenuation);
        updated |= true;
    }

    // Spatialization parameters.
    ImGui::Spacing();

    SpatializationParameters spatializationParameters = m_mixerGroup->GetSpatializationParameters();

    if (ImGui::Checkbox("Override Spatialization Parameters", &spatializationParameters.override))
    {
        if (spatializationParameters.override)
        {
            // Set default value when toggling to true.
            spatializationParameters.minDistance = GetAudio()->GetDefaultSpatializationParameters().minDistance;
            spatializationParameters.attenuation = GetAudio()->GetDefaultSpatializationParameters().attenuation;
        }

        m_mixerGroup->SetSpatializationParameters(spatializationParameters);
        updated |= true;
    }

    if (spatializationParameters.override)
    {
        if (ImGui::InputFloat("Min Distance", &spatializationParameters.minDistance))
        {
            m_mixerGroup->SetSpatializationParameters(spatializationParameters);
            updated |= true;
        }

        if (ImGui::InputFloat("Attenuation", &spatializationParameters.attenuation))
        {
            m_mixerGroup->SetSpatializationParameters(spatializationParameters);
            updated |= true;
        }
    }

    // Child MixerGroups.
    ImGui::Spacing();

    const auto& childMixerGroups = m_mixerGroup->GetChildMixerGroups();
    ImGui::Text(StringFormat("Child MixerGroups : {0}", childMixerGroups.size()));
    for (const auto& childMixerGroup : childMixerGroups)
    {
        ImGui::Text(StringFormat("- {0}", childMixerGroup ? childMixerGroup->GetID() : "NULL"));
    }

    // Finalize
    if (updated)
    {
        RaiseDirty();
    }
}

void AudioMixerGroupPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
}

}   //namespace gugu
