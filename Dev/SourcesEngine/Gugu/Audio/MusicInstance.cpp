////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/MusicInstance.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/AudioClip.h"
#include "Gugu/System/Memory.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
MusicInstance::MusicInstance()
    : m_audioClip(nullptr)
    , m_mixerGroupInstance(nullptr)
    , m_sfMusic(nullptr)
    , m_volume(1.f)
    , m_fadeCoeff(1.f)
{
}

MusicInstance::~MusicInstance()
{
    Reset();
}

void MusicInstance::Reset()
{
    SafeDelete(m_sfMusic);

    m_audioClip = nullptr;
    m_volume = 1.f;
    m_fadeCoeff = 1.f;
}

void MusicInstance::SetAudioClip(AudioClip* audioClip, bool loop)
{
    Reset();

    if (audioClip)
    {
        m_audioClip = audioClip;

        m_sfMusic = m_audioClip->OpenSFMusicStream();
        if (m_sfMusic)
        {
            m_sfMusic->setLooping(loop);
            m_sfMusic->setSpatializationEnabled(false);
            m_sfMusic->setPosition(sf::Vector3f(0, 0, 0));

            RecomputeMixedVolume();
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

AudioClip* MusicInstance::GetAudioClip() const
{
    return m_audioClip;
}

void MusicInstance::SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance)
{
    m_mixerGroupInstance = mixerGroupInstance;
    RecomputeMixedVolume();
}

void MusicInstance::SetVolume(float volume)
{
    m_volume = volume;
    RecomputeMixedVolume();
}

void MusicInstance::SetFadeCoeff(float fadeCoeff)
{
    m_fadeCoeff = fadeCoeff;
    RecomputeMixedVolume();
}

float MusicInstance::GetFadeCoeff() const
{
    return m_fadeCoeff;
}

void MusicInstance::RecomputeMixedVolume()
{
    float volume = m_mixerGroupInstance == nullptr ? m_volume : m_mixerGroupInstance->ComputeMixedVolume(m_volume);
    volume *= m_fadeCoeff;

    if (m_sfMusic)
    {
        m_sfMusic->setVolume(volume * 100.f);
    }
}

void MusicInstance::Play()
{
    if (m_sfMusic && m_sfMusic->getStatus() != sf::SoundSource::Status::Playing)
    {
        m_sfMusic->play();
    }
}

void MusicInstance::Pause()
{
    if (m_sfMusic && m_sfMusic->getStatus() == sf::SoundSource::Status::Playing)
    {
        m_sfMusic->pause();
    }
}

void MusicInstance::Stop()
{
    if (m_sfMusic && m_sfMusic->getStatus() != sf::SoundSource::Status::Stopped)
    {
        m_sfMusic->stop();
    }
}

sf::Time MusicInstance::GetDuration() const
{
    if (m_sfMusic)
        return m_sfMusic->getDuration();
    return sf::Time::Zero;
}

sf::Time MusicInstance::GetPlayOffset() const
{
    if (m_sfMusic)
        return m_sfMusic->getPlayingOffset();
    return sf::Time::Zero;
}

void MusicInstance::SetPlayOffset(const sf::Time& offset)
{
    if (m_sfMusic)
        return m_sfMusic->setPlayingOffset(offset);
}

bool MusicInstance::IsPaused() const
{
    return m_sfMusic ? m_sfMusic->getStatus() == sf::SoundSource::Status::Paused : false;
}

bool MusicInstance::IsFinished() const
{
    if (m_sfMusic)
    {
        return m_sfMusic->getStatus() == sf::SoundSource::Status::Stopped;   //TODO: handle a flag like "explicit stop" to be able to differenciate manual stop and ending stop
    }

    return true;
}

}   // namespace gugu
