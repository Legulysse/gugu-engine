////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/ManagerAudio.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Resources/ManagerResources.h"

#include "Gugu/Resources/Sound.h"
#include "Gugu/Resources/Music.h"
#include "Gugu/Resources/SoundCue.h"

#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Audio/Listener.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAudio::ManagerAudio()
    : m_soundIndex(0)
    , m_masterMuted(false)
    , m_masterVolume(1.f)
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

void ManagerAudio::SetMasterMuted(bool muted)
{
    m_masterMuted = muted;

    sf::Listener::setGlobalVolume(m_masterMuted ? 0.f : m_masterVolume * 100.f);
}

bool ManagerAudio::IsMasterMuted() const
{
    return m_masterMuted;
}

void ManagerAudio::SetMasterVolume(float volume)
{
    m_masterVolume = volume;

    sf::Listener::setGlobalVolume(m_masterMuted ? 0.f : m_masterVolume * 100.f);
}

void ManagerAudio::SetMasterVolume100(int volume)
{
    m_masterVolume = volume * 0.01f;

    sf::Listener::setGlobalVolume(m_masterMuted ? 0.f : m_masterVolume * 100.f);
}

float ManagerAudio::GetMasterVolume() const
{
    return m_masterVolume;
}

int ManagerAudio::GetMasterVolume100() const
{
    return (int)(m_masterVolume * 100.f);
}

bool ManagerAudio::PlaySoundCue(const std::string& soundCueID)
{
    return PlaySoundCue(GetResources()->GetSoundCue(soundCueID));
}

bool ManagerAudio::PlaySoundCue(SoundCue* soundCue)
{
    if (!soundCue)
        return false;

    SoundParameters parameters;
    if (!soundCue->GetRandomSound(parameters))
        return false;

    return PlaySound(parameters);
}

bool ManagerAudio::PlaySound(const std::string& soundID, float volume, int group)
{
    SoundParameters parameters;
    parameters.sound = GetResources()->GetSound(soundID);
    parameters.volume = volume;
    parameters.group = group;

    return PlaySound(parameters);
}

bool ManagerAudio::PlaySound(const SoundParameters& parameters)
{
    Sound* sound = parameters.sound;
    if (!sound)
        sound = GetResources()->GetSound(parameters.soundID);

    if (sound)
    {
        SoundInstance* soundInstance = &m_soundInstances[m_soundIndex];
        soundInstance->Reset();
        soundInstance->SetSound(sound);
        soundInstance->SetVolume(parameters.volume);
        soundInstance->SetGroup(parameters.group);

        if (parameters.pitchLowerOffset != 0 || parameters.pitchUpperOffset != 0)
        {
            // TODO: sanitize parameters when loading the soundcue.
            float pitchLowerOffset = Absolute(parameters.pitchLowerOffset) * -1.f;
            float pitchUpperOffset = Absolute(parameters.pitchUpperOffset);
            soundInstance->SetPitch(1.f + GetRandomf(pitchLowerOffset, pitchUpperOffset));
        }

        soundInstance->Play();

        ++m_soundIndex;
        if (m_soundIndex == m_soundInstances.size())
            m_soundIndex = 0;

        return true;
    }

    return false;
}

bool ManagerAudio::PlayMusic(const std::string& musicID, float volume, float fade)
{
    MusicParameters parameters;
    parameters.music = GetResources()->GetMusic(musicID);
    parameters.volume = volume;
    parameters.fadeOut = fade;
    parameters.fadeIn = fade;

    return PlayMusic(parameters);
}

bool ManagerAudio::PlayMusic(const MusicParameters& parameters)
{
    if (parameters.layer < 0 || parameters.layer >= (int)m_musicLayers.size())
        return false;

    if (parameters.music || GetResources()->HasResource(parameters.musicID))
    {
        MusicLayer* musicLayer = &m_musicLayers[parameters.layer];
        musicLayer->SetNext(parameters);
        musicLayer->FadeToNext();

        return true;
    }

    return false;
}

bool ManagerAudio::PlayMusicList(const std::vector<MusicParameters>& playlist, bool loopPlaylist, int layer)
{
    if (layer < 0 || layer >= (int)m_musicLayers.size())
        return false;

    MusicLayer* musicLayer = &m_musicLayers[layer];
    musicLayer->SetPlayList(playlist, loopPlaylist);
    musicLayer->FadeToNext();

    return true;
}

bool ManagerAudio::StopMusic(float fade, int layer)
{
    if (layer < 0 || layer >= (int)m_musicLayers.size())
        return false;

    MusicLayer* musicLayer = &m_musicLayers[layer];
    if (fade > 0.f)
    {
        MusicParameters parameters;
        parameters.music = nullptr;
        parameters.volume = 0.f;
        parameters.fadeOut = fade;
        parameters.fadeIn = fade;

        musicLayer->SetNext(parameters);
        musicLayer->FadeToNext();
    }
    else
    {
        musicLayer->Reset();
    }

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
