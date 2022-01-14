////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/MusicInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Resources/Music.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
MusicInstance::MusicInstance()
{
    m_sfMusic      = nullptr;
    m_volume       = 1.f;
    m_fadeCoeff    = 1.f;

    //Reset();
}
/*
MusicInstance::MusicInstance(const MusicInstance&)
{
    m_sfMusic      = nullptr;
    m_volume       = 1.f;
    m_fadeCoeff    = 1.f;

    //Reset();
}*/

MusicInstance::~MusicInstance()
{
    Reset();
}

void MusicInstance::Reset()
{
    SafeDelete(m_sfMusic);

    m_music        = nullptr;
    m_volume       = 1.f;
    m_fadeCoeff    = 1.f;
}

void MusicInstance::SetMusic(Music* _pMusic, bool _bLoop)
{
    Reset();

    if (_pMusic)
    {
        m_music = _pMusic;

        if (!m_sfMusic)
            m_sfMusic = new sf::Music;

        if (m_music->LoadSFMusic(m_sfMusic))
        {
            m_sfMusic->setLoop(_bLoop);
            m_sfMusic->setVolume(m_volume * m_fadeCoeff * 100.f);
        }
        else
        {
            Reset();
        }
    }
}

bool MusicInstance::IsSet() const
{
    return (m_sfMusic != nullptr);
}

Music* MusicInstance::GetMusic() const
{
    return m_music;
}

void MusicInstance::SetVolume(float _fVolume)
{
    m_volume = _fVolume;

    if (m_sfMusic)
        m_sfMusic->setVolume(m_volume * m_fadeCoeff * 100.f);
}

void MusicInstance::SetFadeCoeff(float _fFadeCoeff)
{
    m_fadeCoeff = _fFadeCoeff;

    if (m_sfMusic)
        m_sfMusic->setVolume(m_volume * m_fadeCoeff * 100.f);
}

float MusicInstance::GetFadeCoeff() const
{
    return m_fadeCoeff;
}

void MusicInstance::Play()
{
    if (m_sfMusic && m_sfMusic->getStatus() != sf::Sound::Playing)
    {
        m_sfMusic->play();
    }
}

void MusicInstance::Pause()
{
    if (m_sfMusic && m_sfMusic->getStatus() == sf::Sound::Playing)
    {
        m_sfMusic->pause();
    }
}

void MusicInstance::Stop()
{
    if (m_sfMusic && m_sfMusic->getStatus() != sf::Sound::Stopped)
    {
        m_sfMusic->stop();
    }
}

DeltaTime MusicInstance::GetDuration() const
{
    if (m_sfMusic)
        return m_sfMusic->getDuration();
    return DeltaTime(0);
}

DeltaTime MusicInstance::GetPlayOffset() const
{
    if (m_sfMusic)
        return m_sfMusic->getPlayingOffset();
    return DeltaTime(0);
}

void MusicInstance::SetPlayOffset(DeltaTime _kOffset)
{
    if (m_sfMusic)
        return m_sfMusic->setPlayingOffset(_kOffset.GetSFTime());
}

bool MusicInstance::IsPaused() const
{
    return m_sfMusic ? m_sfMusic->getStatus() == sf::Sound::Paused : false;
}

bool MusicInstance::IsFinished() const
{
    if (m_sfMusic)
    {
        return m_sfMusic->getStatus() == sf::Sound::Stopped;   //TODO: handle a flag like "explicit stop" to be able to differenciate manual stop and ending stop
    }

    return true;
}

}   // namespace gugu
