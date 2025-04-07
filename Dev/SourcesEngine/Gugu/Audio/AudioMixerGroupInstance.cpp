////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Resources/AudioMixerGroup.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioMixerGroupInstance::AudioMixerGroupInstance(AudioMixerGroup* mixerGroup)
    : m_mixerGroup(mixerGroup)
    , m_parentGroup(nullptr)
    , m_volume(1.f)
{
}

AudioMixerGroupInstance::~AudioMixerGroupInstance()
{
}

void AudioMixerGroupInstance::SetVolume(float volume)
{
    m_volume = volume;
}

float AudioMixerGroupInstance::GetVolume() const
{
    return m_volume;
}

float AudioMixerGroupInstance::ComputeMixedVolume(float volume) const
{
    volume = volume * m_mixerGroup->GetVolumeAttenuation() * m_volume;

    if (m_parentGroup)
    {
        volume = m_parentGroup->ComputeMixedVolume(volume);
    }

    return volume;
}

void AudioMixerGroupInstance::LoadGroupHierarchy(AudioMixerGroupInstance* parentGroup)
{
    m_parentGroup = parentGroup;

    for (const auto& childGroup : m_mixerGroup->GetChildGroups())
    {
        // TODO: check recursive loops.

        AudioMixerGroupInstance* childGroupInstance = new AudioMixerGroupInstance(childGroup);
        m_childGroups.push_back(childGroupInstance);

        childGroupInstance->LoadGroupHierarchy(this);

        GetAudio()->RegisterAudioMixerGroupInstance(childGroup, childGroupInstance);
    }
}

}   // namespace gugu
