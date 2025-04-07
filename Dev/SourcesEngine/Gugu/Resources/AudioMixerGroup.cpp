////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/AudioMixerGroup.h"

////////////////////////////////////////////////////////////////
// Includes

//#include "Gugu/System/Memory.h"
//#include "Gugu/System/String.h"
//#include "Gugu/Debug/Logger.h"

//#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioMixerGroup::AudioMixerGroup()
{
}

AudioMixerGroup::~AudioMixerGroup()
{
    Unload();
}

EResourceType::Type AudioMixerGroup::GetResourceType() const
{
    return EResourceType::AudioMixerGroup;
}

void AudioMixerGroup::Unload()
{
}

bool AudioMixerGroup::LoadFromFile()
{
    Unload();

    return true;
}

}   // namespace gugu
