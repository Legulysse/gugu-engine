////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/System/Container.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioMixerGroupInstance::AudioMixerGroupInstance(AudioMixerGroup* mixerGroup)
    : m_mixerGroup(mixerGroup)
    , m_parentMixerGroupInstance(nullptr)
    , m_volume(1.f)
{
}

AudioMixerGroupInstance::~AudioMixerGroupInstance()
{
    ClearStdVector(m_childMixerGroupInstances);
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

    if (m_parentMixerGroupInstance)
    {
        volume = m_parentMixerGroupInstance->ComputeMixedVolume(volume);
    }

    return volume;
}

void AudioMixerGroupInstance::LoadMixerGroupHierarchy(AudioMixerGroupInstance* parentMixerGroupInstance)
{
    m_parentMixerGroupInstance = parentMixerGroupInstance;

    for (const auto& childMixerGroup : m_mixerGroup->GetChildMixerGroups())
    {
        // TODO: check recursive loops.

        AudioMixerGroupInstance* childMixerGroupInstance = new AudioMixerGroupInstance(childMixerGroup);
        m_childMixerGroupInstances.push_back(childMixerGroupInstance);

        childMixerGroupInstance->LoadMixerGroupHierarchy(this);

        GetAudio()->RegisterMixerGroupInstance(childMixerGroup, childMixerGroupInstance);
    }
}

}   // namespace gugu
