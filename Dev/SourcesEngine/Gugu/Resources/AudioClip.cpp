////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/AudioClip.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioClip::AudioClip()
    : m_sfSoundBuffer(nullptr)
{
}

AudioClip::~AudioClip()
{
    Unload();
}

sf::SoundBuffer* AudioClip::GetSFSoundBuffer() const
{
    return m_sfSoundBuffer;
}

EResourceType::Type AudioClip::GetResourceType() const
{
    return EResourceType::AudioClip;
}

void AudioClip::Unload()
{
    SafeDelete(m_sfSoundBuffer);
}

bool AudioClip::LoadFromFile()
{
    Unload();

    m_sfSoundBuffer = new sf::SoundBuffer;
    if (!m_sfSoundBuffer->loadFromFile(GetFileInfo().GetFileSystemPath()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("AudioClip file not found : {0}", GetFileInfo().GetFilePath_utf8()));
        return false;
    }

    return true;
}

}   // namespace gugu
