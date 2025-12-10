#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/AudioUtility.h"

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

class SoundInstance
{
public:

    SoundInstance();
    ~SoundInstance();

    void Reset();
    bool UpdateStatus();
    bool IsActive() const;

    void SetAudioClip(AudioClip* audioClip);
    AudioClip* GetAudioClip() const;

    void SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance);
    void SetVolume(float volume);
    void SetPitch(float pitch);

    void SetSpatialization(bool enabled, float minDistance, float attenuation);
    void SetPosition(const Vector2f& position);

    void RecomputeMixedVolume();

    void Play();

protected:

    AudioClip* m_audioClip;
    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Sound* m_sfSound;
    float m_volume;
};

}   // namespace gugu
