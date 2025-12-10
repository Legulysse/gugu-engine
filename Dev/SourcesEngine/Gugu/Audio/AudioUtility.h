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

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct SpatializationParameters
{
    bool override = false;
    float minDistance = 0.f;
    float attenuation = 0.f;
};

struct SoundParameters
{
    AudioClip* audioClip = nullptr;
    std::string audioClipId;
    AudioMixerGroupInstance* mixerGroupInstance = nullptr;
    std::string mixerGroupId;
    float volume = 1.f;
    Vector2f volumeRandomRange;
    Vector2f pitchRandomRange;
    bool spatialized = false;
    SpatializationParameters spatializationParameters;
    Vector2f position;
};

}   // namespace gugu
