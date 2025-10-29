#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AudioClip;
    class AudioMixerGroupInstance;
}

namespace sf
{
    class Sound;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct SoundParameters
{
    AudioClip* audioClip;
    std::string audioClipId;
    AudioMixerGroupInstance* mixerGroupInstance;
    std::string mixerGroupId;
    float volume;
    float pitchLowerOffset;
    float pitchUpperOffset;

    SoundParameters();
};

class SoundInstance
{
public:

    SoundInstance();
    ~SoundInstance();

    void Reset();

    void SetAudioClip(AudioClip* audioClip);
    AudioClip* GetAudioClip() const;

    void SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance);
    void SetVolume(float volume);
    void SetPitch(float pitch);

    void RecomputeMixedVolume();

    void Play();

protected:

    AudioClip* m_audioClip;
    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Sound* m_sfSound;
    float m_volume;
};

}   // namespace gugu
