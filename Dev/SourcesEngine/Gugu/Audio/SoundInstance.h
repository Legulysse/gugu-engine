#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

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
    bool spatialized;
    gugu::Vector2f position;

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

    void SetSpatialization(bool enabled, float minDistance, float attenuation);
    void SetPosition(const gugu::Vector2f& position);

    void RecomputeMixedVolume();

    void Play();

protected:

    AudioClip* m_audioClip;
    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Sound* m_sfSound;
    float m_volume;
};

}   // namespace gugu
