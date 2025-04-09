#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Audio/Sound.hpp>

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AudioClip;
    class AudioMixerGroupInstance;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct SoundParameters
{
    AudioClip* audioClip;
    std::string audioClipID;
    AudioMixerGroupInstance* mixerGroupInstance;
    std::string mixerGroupID;
    float volume;
    float pitchLowerOffset;
    float pitchUpperOffset;
    int group;

    SoundParameters();
};

class SoundInstance
{
public:

    SoundInstance();
    ~SoundInstance();

    void Reset();

    void SetGroup(int group);
    int GetGroup() const;

    void SetAudioClip(AudioClip* audioClip);
    void SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance);
    void SetVolume(float volume);
    void SetPitch(float pitch);

    void RecomputeMixedVolume();

    void Play();

protected:

    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Sound m_sfSound;
    float m_volume;
    int m_group;
};

}   // namespace gugu
