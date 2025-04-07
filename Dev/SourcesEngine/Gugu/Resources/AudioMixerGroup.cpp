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
    : m_volumeAttenuation(1.f)
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

float AudioMixerGroup::GetVolumeAttenuation() const
{
    return m_volumeAttenuation;
}

const std::vector<AudioMixerGroup*>& AudioMixerGroup::GetChildGroups() const
{
    return m_childGroups;
}

void AudioMixerGroup::Unload()
{
}

bool AudioMixerGroup::LoadFromXml(const pugi::xml_document& document)
{
    Unload();
    return true;
}

bool AudioMixerGroup::SaveToXml(pugi::xml_document& document) const
{
    return true;
}

}   // namespace gugu
