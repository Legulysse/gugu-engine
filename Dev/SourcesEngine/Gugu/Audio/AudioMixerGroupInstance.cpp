////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/System/Container.h"

#include <assert.h>

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

    GetAudio()->RecomputeAllMixedVolumes();
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

void AudioMixerGroupInstance::LoadMixerGroupHierarchy(AudioMixerGroupInstance* parentMixerGroupInstance, std::map<AudioMixerGroup*, AudioMixerGroupInstance*>& registeredMixerGroupInstances)
{
    m_parentMixerGroupInstance = parentMixerGroupInstance;

    for (const auto& childMixerGroup : m_mixerGroup->GetChildMixerGroups())
    {
        if (childMixerGroup == nullptr)
            continue;

        // Check redundancy and recursive loops.
        if (StdMapContainsKey(registeredMixerGroupInstances, childMixerGroup))
        {
            assert(false);   // An AudioMixerGroup should appear only once in the AudioMixer hierarchy.
            continue;
        }

        AudioMixerGroupInstance* childMixerGroupInstance = new AudioMixerGroupInstance(childMixerGroup);
        registeredMixerGroupInstances.insert(std::make_pair(childMixerGroup, childMixerGroupInstance));
        m_childMixerGroupInstances.push_back(childMixerGroupInstance);

        childMixerGroupInstance->LoadMixerGroupHierarchy(this, registeredMixerGroupInstances);
    }
}

}   // namespace gugu
