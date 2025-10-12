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
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/InputSoundFile.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AudioClip::AudioClip()
    : m_sfSoundBuffer(nullptr)
    , m_duration(sf::Time::Zero)
{
}

AudioClip::~AudioClip()
{
    Unload();
}

sf::SoundBuffer* AudioClip::GetOrLoadSFSoundBuffer()
{
    if (!m_sfSoundBuffer)
    {
        m_sfSoundBuffer = new sf::SoundBuffer;

        if (!m_sfSoundBuffer->loadFromFile(GetFileInfo().GetFileSystemPath()))
        {
            GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("AudioClip file not found : {0}", GetFileInfo().GetFilePath_utf8()));
            return nullptr;
        }

        if (m_duration == sf::Time::Zero)
        {
            m_duration = m_sfSoundBuffer->getDuration();
        }
    }

    return m_sfSoundBuffer;
}

sf::Music* AudioClip::OpenSFMusicStream()
{
    sf::Music* musicStream = new sf::Music;

    if (!musicStream->openFromFile(GetFileInfo().GetFileSystemPath()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("AudioClip file not found : {0}", GetFileInfo().GetFilePath_utf8()));
        return nullptr;
    }

    if (m_duration == sf::Time::Zero)
    {
        m_duration = musicStream->getDuration();
    }

    return musicStream;
}

sf::Time AudioClip::GetOrReadDuration()
{
    if (m_duration == sf::Time::Zero)
    {
        //TODO: Check if I can/need to make this a bit cleaner.
        sf::InputSoundFile soundFile;
        if (soundFile.openFromFile(GetFileInfo().GetFileSystemPath()))
        {
            m_duration = soundFile.getDuration();
        }
    }

    return m_duration;
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

    // We dont load anything yet, since we dont know if the file will be used as a SoundBuffer or a Stream.
    return true;
}

}   // namespace gugu
