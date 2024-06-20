////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/MusicLayer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Audio/MusicInstance.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Resources/Music.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

MusicParameters::MusicParameters()
{
    music = nullptr;
    musicID = "";
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
/*
MusicLayer::MusicLayer(const MusicLayer&)
{
    m_currentInstance = nullptr;
    m_nextInstance = nullptr;
    m_fadeIn = 2.f;
    m_fadeOut = 2.f;
    m_currentFadeTime = 0.f;
    m_isFading = false;
}*/

MusicLayer::~MusicLayer()
{
}

void MusicLayer::SetInstances(MusicInstance* _pInstanceA, MusicInstance* _pInstanceB)
{
    m_currentInstance = _pInstanceA;
    m_nextInstance = _pInstanceB;
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

void MusicLayer::SetNext(const MusicParameters& _kParameters)
{
    PurgeFade();

    m_playlist.clear();
    m_playlistIndex = -1;

    Music* pMusic = _kParameters.music;
    if (!pMusic && !_kParameters.musicID.empty())
        pMusic = GetResources()->GetMusic(_kParameters.musicID);

    m_nextInstance->Reset();
    m_nextInstance->SetMusic(pMusic, _kParameters.loop);
    m_nextInstance->SetVolume(_kParameters.volume);

    m_fadeIn = Max(0.f, _kParameters.fadeIn);
    m_fadeOut = Max(0.f, _kParameters.fadeOut);
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
            const MusicParameters kParameters = m_playlist[m_playlistIndex];

            Music* pMusic = kParameters.music;
            if (!pMusic && !kParameters.musicID.empty())
                pMusic = GetResources()->GetMusic(kParameters.musicID);

            m_nextInstance->Reset();
            m_nextInstance->SetMusic(pMusic, kParameters.loop);
            m_nextInstance->SetVolume(kParameters.volume);

            m_fadeIn = Max(0.f, kParameters.fadeIn);
            m_fadeOut = Max(0.f, kParameters.fadeOut);
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

void MusicLayer::SetPlayList(const std::vector<MusicParameters>& _vecPlaylist, bool loopPlaylist)
{
    PurgeFade();

    m_playlist = _vecPlaylist;
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
        bool bFadeOutFinished = true;
        if (m_currentInstance->IsSet())
        {
            if (m_fadeOut > 0.f)
            {
                float fFadeCoeff = m_currentInstance->GetFadeCoeff();
                fFadeCoeff -= (dt.s() / m_fadeOut);
                if (fFadeCoeff > 0.f)
                {
                    m_currentInstance->SetFadeCoeff(fFadeCoeff);
                    bFadeOutFinished = false;
                }
            }
            
            if (bFadeOutFinished)
            {
                m_currentInstance->Reset();
            }
        }

        bool bFadeInFinished = true;
        if (m_nextInstance->IsSet())
        {
            if (m_fadeIn > 0.f)
            {
                float fFadeCoeff = m_nextInstance->GetFadeCoeff();
                fFadeCoeff += (dt.s() / m_fadeIn);
                if (fFadeCoeff - 1.f < math::Epsilon3)
                {
                    m_nextInstance->SetFadeCoeff(fFadeCoeff);
                    bFadeInFinished = false;
                }
            }

            if (bFadeInFinished)
            {
                m_nextInstance->SetFadeCoeff(1.f);
            }
        }
        
        if (bFadeOutFinished && bFadeInFinished)
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
