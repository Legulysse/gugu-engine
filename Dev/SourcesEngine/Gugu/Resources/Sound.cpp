////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Sound.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Sound::Sound()
: m_sfSoundBuffer(nullptr)
{
}

Sound::~Sound()
{
    SafeDelete(m_sfSoundBuffer);
}

sf::SoundBuffer* Sound::GetSFSoundBuffer() const
{
    return m_sfSoundBuffer;
}

EResourceType::Type Sound::GetResourceType() const
{
    return EResourceType::Sound;
}

bool Sound::LoadFromFile()
{
    m_sfSoundBuffer = new sf::SoundBuffer;
    if (!m_sfSoundBuffer->loadFromFile(GetFileInfo().GetPathName()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Sound not found : {0}", GetFileInfo().GetPathName()));
        return false;
    }
    return true;
}

}   // namespace gugu
