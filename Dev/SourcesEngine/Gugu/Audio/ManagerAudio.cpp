////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Manager/ManagerAudio.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"

#include "Gugu/Resources/Sound.h"
#include "Gugu/Resources/Music.h"
#include "Gugu/Resources/SoundCue.h"

#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Misc/Logger.h"

#include <SFML/Audio/Listener.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAudio::ManagerAudio()
{
}

ManagerAudio::~ManagerAudio()
{
}

void ManagerAudio::Init(const EngineConfig& config)
{
    GetLogEngine()->Print(ELog::Info, ELogEngine::Audio, "Init Manager Audio...");

    m_soundInstances.resize(Max(12, config.maxSoundTracks));
    m_musicInstances.resize(Max(4, config.maxMusicTracks));
    m_musicLayers.resize(Max(2, (int)m_musicInstances.size() / 2));

    for (size_t i = 0; i < m_musicLayers.size(); ++i)
    {
        m_musicLayers[i].SetInstances(&m_musicInstances[i*2], &m_musicInstances[i*2 + 1]);
    }

    m_soundIndex = 0;

    SetVolumeMaster(1.f);

    GetLogEngine()->Print(ELog::Info, ELogEngine::Audio, "Manager Audio Ready");
}

void ManagerAudio::Release()
{
    m_musicLayers.clear();
    m_musicInstances.clear();
    m_soundInstances.clear();
}

void ManagerAudio::Update(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_musicLayers.size(); ++i)
    {
        m_musicLayers[i].Update(dt);
    }
}

void ManagerAudio::SetVolumeMaster(float _fVolume)
{
    sf::Listener::setGlobalVolume(_fVolume * 100.f);
}

float ManagerAudio::GetVolumeMaster() const
{
    return sf::Listener::getGlobalVolume() * 0.01f;
}

bool ManagerAudio::PlaySoundCue(const std::string& _strFile)
{
    SoundCue* pSoundCue = GetResources()->GetSoundCue(_strFile);
    if (!pSoundCue)
        return false;

    SoundParameters kParameters;
    if (!pSoundCue->GetRandomSound(kParameters))
        return false;

    return PlaySound(kParameters);
}

bool ManagerAudio::PlaySound(const std::string& _strFile, float _fVolume, int _iGroup)
{
    SoundParameters kParameters;
    kParameters.sound = GetResources()->GetSound(_strFile);
    kParameters.volume = _fVolume;
    kParameters.group = _iGroup;

    return PlaySound(kParameters);
}

bool ManagerAudio::PlaySound(const SoundParameters& _kParameters)
{
    Sound* pSound = _kParameters.sound;
    if (!pSound)
        pSound = GetResources()->GetSound(_kParameters.soundID);

    if (pSound)
    {
        SoundInstance* pInstance = &m_soundInstances[m_soundIndex];
        pInstance->Reset();
        pInstance->SetSound(pSound);
        pInstance->SetVolume(_kParameters.volume);
        pInstance->SetGroup(_kParameters.group);
        pInstance->Play();

        ++m_soundIndex;
        if (m_soundIndex == m_soundInstances.size())
            m_soundIndex = 0;

        return true;
    }

    return false;
}

bool ManagerAudio::PlayMusic(const std::string& _strFile, float _fVolume, float _fFade)
{
    MusicParameters kParameters;
    kParameters.music = GetResources()->GetMusic(_strFile);
    kParameters.volume = _fVolume;
    kParameters.fadeOut = _fFade;
    kParameters.fadeIn = _fFade;

    return PlayMusic(kParameters);
}

bool ManagerAudio::PlayMusic(const MusicParameters& _kParameters)
{
    if (_kParameters.layer < 0 || _kParameters.layer >= (int)m_musicLayers.size())
        return false;

    if (_kParameters.music || GetResources()->HasResource(_kParameters.musicID))
    {
        MusicLayer* pLayer = &m_musicLayers[_kParameters.layer];
        pLayer->SetNext(_kParameters);
        pLayer->FadeToNext();

        return true;
    }

    return false;
}

bool ManagerAudio::PlayMusicList(const std::vector<MusicParameters>& _vecPlaylist, int layer)
{
    if (layer < 0 || layer >= (int)m_musicLayers.size())
        return false;

    MusicLayer* pLayer = &m_musicLayers[layer];
    pLayer->SetPlayList(_vecPlaylist);
    pLayer->FadeToNext();

    return true;
}

bool ManagerAudio::StopMusic(float _fFade, int layer)
{
    if (layer < 0 || layer >= (int)m_musicLayers.size())
        return false;

    MusicParameters kParameters;
    kParameters.music = nullptr;
    kParameters.volume = 0.f;
    kParameters.fadeOut = _fFade;
    kParameters.fadeIn = _fFade;

    MusicLayer* pLayer = &m_musicLayers[layer];
    pLayer->SetNext(kParameters);
    pLayer->FadeToNext();

    return true;
}

MusicInstance* ManagerAudio::GetCurrentMusicInstance(int layer) const
{
    if (layer < 0 || layer >= (int)m_musicLayers.size())
        return nullptr;

    return m_musicLayers[layer].GetCurrentMusicInstance();
}

ManagerAudio* GetAudio()
{
    return GetEngine()->GetManagerAudio();
}

}   // namespace gugu
