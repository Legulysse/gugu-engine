#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>
#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AudioMixerGroup;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AudioMixerGroupInstance
{
public:

    AudioMixerGroupInstance(AudioMixerGroup* mixerGroup);
    ~AudioMixerGroupInstance();

    void SetVolume(float volume);
    float GetVolume() const;

    float ComputeMixedVolume(float volume) const;

    void LoadMixerGroupHierarchy(AudioMixerGroupInstance* parentMixerGroupInstance, std::map<AudioMixerGroup*, AudioMixerGroupInstance*>& registeredMixerGroupInstances);

protected:

    AudioMixerGroup* m_mixerGroup;
    AudioMixerGroupInstance* m_parentMixerGroupInstance;
    std::vector<AudioMixerGroupInstance*> m_childMixerGroupInstances;
    float m_volume;
};

}   // namespace gugu
