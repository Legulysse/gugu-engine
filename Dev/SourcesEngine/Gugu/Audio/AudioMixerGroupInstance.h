#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

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

    void LoadGroupHierarchy(AudioMixerGroupInstance* parentGroup);

protected:

    AudioMixerGroup* m_mixerGroup;
    AudioMixerGroupInstance* m_parentGroup;
    std::vector<AudioMixerGroupInstance*> m_childGroups;
    float m_volume;
};

}   // namespace gugu
