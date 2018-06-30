////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Sound.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu//Engine.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Misc/Logger.h"

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

EResourceType::Type	Sound::GetResourceType() const
{
	return EResourceType::Sound;
}

bool Sound::LoadFromFile()
{
	m_sfSoundBuffer = new sf::SoundBuffer;
	if (!m_sfSoundBuffer->loadFromFile(GetFileInfoRef().GetPathName()))
	{
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Sound not found : {0}", GetFileInfoRef().GetPathName()));
        return false;
	}
    return true;
}

}	// namespace gugu
