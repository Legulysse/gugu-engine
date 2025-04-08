////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/MusicLayer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Audio/MusicInstance.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Music.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

MusicParameters::MusicParameters()
{
    music = nullptr;
    musicID = "";
    mixerGroupInstance = nullptr;
    mixerGroupID = "";
    volume = 1.f;
    fadeIn = 2.f;
    fadeOut = 2.f;
	layer = 0;
    loop = false;
}

MusicLayer::MusicLayer()
{
    m_currentInstance = nullptr;
    m_nextInstance = nullptr;
    m_fadeIn = 2.f;
    m_fadeOut = 2.f;
    m_currentFadeTime = 0.f;
    m_isFading = false;
    m_playlistIndex = -1;
    m_loopPlaylist = false;
}

MusicLayer::~MusicLayer()
{
}

void MusicLayer::SetInstances(MusicInstance* instanceA, MusicInstance* instanceB)
{
    m_currentInstance = instanceA;
    m_nextInstance = instanceB;
}

void MusicLayer::PurgeFade()
{
    m_fadeIn = 2.f;
    m_fadeOut = 2.f;
    m_currentFadeTime = 0.f;
    m_isFading = false;

    if (m_nextInstance->IsSet())
    {
        m_nextInstance->SetFadeCoeff(1.f);     //TODO: Refacto individual fade to avoid this line and allow smooth fade-out during a fade-in

        if (m_currentInstance->IsSet())
            m_currentInstance->Reset();

        std::swap(m_currentInstance, m_nextInstance);
    }
}

void MusicLayer::SetNext(const MusicParameters& parameters)
{
    PurgeFade();

    m_playlist.clear();
    m_playlistIndex = -1;

    Music* music = parameters.music;
    if (!music)
    {
        music = GetResources()->GetMusic(parameters.musicID);
    }

    AudioMixerGroupInstance* mixerGroupInstance = parameters.mixerGroupInstance;
    if (!mixerGroupInstance)
    {
        mixerGroupInstance = GetAudio()->GetMixerGroupInstance(GetResources()->GetAudioMixerGroup(parameters.mixerGroupID));
    }

    m_nextInstance->Reset();
    m_nextInstance->SetMusic(music, parameters.loop);
    m_nextInstance->SetMixerGroupInstance(mixerGroupInstance);
    m_nextInstance->SetVolume(parameters.volume);

    m_fadeIn = Max(0.f, parameters.fadeIn);
    m_fadeOut = Max(0.f, parameters.fadeOut);
}

void MusicLayer::FadeToNext()
{
    bool fadeToNext = true;

    if (!m_playlist.empty())
    {
        ++m_playlistIndex;
        if (m_playlistIndex >= (int)m_playlist.size())
        {
            if (m_loopPlaylist)
            {
                m_playlistIndex = 0;
            }
            else
            {
                m_playlistIndex = -1;
            }
        }

        if (m_playlistIndex >= 0)
        {
            const MusicParameters parameters = m_playlist[m_playlistIndex];

            Music* music = parameters.music;
            if (!music)
            {
                music = GetResources()->GetMusic(parameters.musicID);
            }

            AudioMixerGroupInstance* mixerGroupInstance = parameters.mixerGroupInstance;
            if (!mixerGroupInstance)
            {
                mixerGroupInstance = GetAudio()->GetMixerGroupInstance(GetResources()->GetAudioMixerGroup(parameters.mixerGroupID));
            }

            m_nextInstance->Reset();
            m_nextInstance->SetMusic(music, parameters.loop);
            m_nextInstance->SetMixerGroupInstance(mixerGroupInstance);
            m_nextInstance->SetVolume(parameters.volume);

            m_fadeIn = Max(0.f, parameters.fadeIn);
            m_fadeOut = Max(0.f, parameters.fadeOut);
        }
        else
        {
            Reset();
            fadeToNext = false;
        }
    }

    if (m_nextInstance && fadeToNext)
    {
        m_currentFadeTime = 0.f;
        m_isFading = true;

        m_nextInstance->SetFadeCoeff(0.f);
        m_nextInstance->Play();

        if (m_fadeIn == 0.f && m_fadeOut == 0.f)
        {
            PurgeFade();
        }
    }
}

void MusicLayer::SetPlayList(const std::vector<MusicParameters>& playlist, bool loopPlaylist)
{
    PurgeFade();

    m_playlist = playlist;
    m_playlistIndex = -1;
    m_loopPlaylist = loopPlaylist;
}

void MusicLayer::Reset()
{
    if (m_currentInstance && m_currentInstance->IsSet())
    {
        m_currentInstance->Stop();
        m_currentInstance->Reset();
    }

    if (m_nextInstance && m_nextInstance->IsSet())
    {
        m_nextInstance->Stop();
        m_nextInstance->Reset();
    }

    PurgeFade();

    m_playlist.clear();
    m_playlistIndex = -1;
    m_loopPlaylist = false;
}

void MusicLayer::Update(const DeltaTime& dt)
{
    if (m_isFading)
    {
        bool fadeOutFinished = true;
        if (m_currentInstance->IsSet())
        {
            if (m_fadeOut > 0.f)
            {
                float fadeCoeff = m_currentInstance->GetFadeCoeff();
                fadeCoeff -= (dt.s() / m_fadeOut);
                if (fadeCoeff > 0.f)
                {
                    m_currentInstance->SetFadeCoeff(fadeCoeff);
                    fadeOutFinished = false;
                }
            }
            
            if (fadeOutFinished)
            {
                m_currentInstance->Reset();
            }
        }

        bool fadeInFinished = true;
        if (m_nextInstance->IsSet())
        {
            if (m_fadeIn > 0.f)
            {
                float fadeCoeff = m_nextInstance->GetFadeCoeff();
                fadeCoeff += (dt.s() / m_fadeIn);
                if (fadeCoeff - 1.f < math::Epsilon3)
                {
                    m_nextInstance->SetFadeCoeff(fadeCoeff);
                    fadeInFinished = false;
                }
            }

            if (fadeInFinished)
            {
                m_nextInstance->SetFadeCoeff(1.f);
            }
        }
        
        if (fadeOutFinished && fadeInFinished)
        {
            PurgeFade();
        }
    }
    else
    {
        if (!m_playlist.empty())
        {
            if (m_currentInstance->IsFinished())
            {
                FadeToNext();
            }
        }
        else if (m_currentInstance->IsFinished())
        {
            Reset();
        }
    }
}

MusicInstance* MusicLayer::GetCurrentMusicInstance() const
{
    return (m_currentInstance->IsSet()) ? m_currentInstance : nullptr;
}

}   // namespace gugu
