////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/ManagerAudio.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Audio/Listener.hpp>

#include <assert.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAudio::ManagerAudio()
    : m_rootMixerGroupInstance(nullptr)
    , m_soundIndex(0)
    , m_listenerMuted(false)
    , m_listenerVolume(1.f)
{
}

ManagerAudio::~ManagerAudio()
{
    SafeDelete(m_rootMixerGroupInstance);
    m_mixerGroupInstances.clear();
}

void ManagerAudio::Init(const EngineConfig& config)
{
    GetLogEngine()->Print(ELog::Info, ELogEngine::Audio, "Init Manager Audio...");

    m_soundInstances.resize(Max(12, config.maxSoundSourceCount));
    m_musicInstances.resize(Max(4, config.maxMusicSourceCount));
    m_musicLayers.resize(Max(2, (int)m_musicInstances.size() / 2));

    for (size_t i = 0; i < m_musicLayers.size(); ++i)
    {
        m_musicLayers[i].SetInstances(&m_musicInstances[i*2], &m_musicInstances[i*2 + 1]);
    }

    SetRootAudioMixerGroup(GetResources()->GetAudioMixerGroup(config.rootAudioMixerGroup));

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

void ManagerAudio::SetListenerMuted(bool muted)
{
    m_listenerMuted = muted;

    sf::Listener::setGlobalVolume(m_listenerMuted ? 0.f : m_listenerVolume * 100.f);
}

bool ManagerAudio::IsListenerMuted() const
{
    return m_listenerMuted;
}

void ManagerAudio::SetListenerVolume(float volume)
{
    m_listenerVolume = volume;

    sf::Listener::setGlobalVolume(m_listenerMuted ? 0.f : m_listenerVolume * 100.f);
}

float ManagerAudio::GetListenerVolume() const
{
    return m_listenerVolume;
}

void ManagerAudio::SetRootAudioMixerGroup(AudioMixerGroup* rootMixerGroup)
{
    if (!rootMixerGroup)
        return;

    assert(m_rootMixerGroupInstance == nullptr);     // Replacing the root mixer group is not supported.

    m_rootMixerGroupInstance = new AudioMixerGroupInstance(rootMixerGroup);
    m_mixerGroupInstances.insert(std::make_pair(rootMixerGroup, m_rootMixerGroupInstance));

    m_rootMixerGroupInstance->LoadMixerGroupHierarchy(nullptr, m_mixerGroupInstances);
}

AudioMixerGroupInstance* ManagerAudio::GetMixerGroupInstance(const std::string& mixerGroupId) const
{
    for (const auto& kvp : m_mixerGroupInstances)
    {
        if (kvp.first->GetID() == mixerGroupId)
        {
            return kvp.second;
        }
    }

    return nullptr;
}

AudioMixerGroupInstance* ManagerAudio::GetMixerGroupInstance(AudioMixerGroup* mixerGroup) const
{
    auto mixerGroupInstance = m_mixerGroupInstances.find(mixerGroup);
    if (mixerGroupInstance != m_mixerGroupInstances.end())
    {
        return mixerGroupInstance->second;
    }

    return nullptr;
}

void ManagerAudio::RecomputeAllMixedVolumes()
{
    for (auto& soundInstance : m_soundInstances)
    {
        soundInstance.RecomputeMixedVolume();
    }

    for (auto& musicInstance : m_musicInstances)
    {
        musicInstance.RecomputeMixedVolume();
    }
}

bool ManagerAudio::PlaySoundCue(const std::string& soundCueId)
{
    return PlaySoundCue(GetResources()->GetSoundCue(soundCueId));
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

bool ManagerAudio::PlaySound(const std::string& audioClipId, float volume)
{
    SoundParameters parameters;
    parameters.audioClip = GetResources()->GetAudioClip(audioClipId);
    parameters.audioClipId = audioClipId;
    parameters.volume = volume;

    return PlaySound(parameters);
}

bool ManagerAudio::PlaySound(const SoundParameters& parameters)
{
    AudioClip* audioClip = parameters.audioClip;
    if (!audioClip)
    {
        audioClip = GetResources()->GetAudioClip(parameters.audioClipId);
    }

    AudioMixerGroupInstance* mixerGroupInstance = parameters.mixerGroupInstance;
    if (!mixerGroupInstance)
    {
        mixerGroupInstance = GetMixerGroupInstance(GetResources()->GetAudioMixerGroup(parameters.mixerGroupId));
    }

    if (audioClip)
    {
        SoundInstance* soundInstance = &m_soundInstances[m_soundIndex];
        soundInstance->Reset();
        soundInstance->SetAudioClip(audioClip);
        soundInstance->SetMixerGroupInstance(mixerGroupInstance);   // Note: I currently allow a null group instance.
        soundInstance->SetVolume(parameters.volume);

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

bool ManagerAudio::PlayMusic(const std::string& audioClipId, float volume, float fade)
{
    MusicParameters parameters;
    parameters.audioClip = GetResources()->GetAudioClip(audioClipId);
    parameters.audioClipId = audioClipId;
    parameters.volume = volume;
    parameters.fadeOut = fade;
    parameters.fadeIn = fade;

    return PlayMusic(parameters);
}

bool ManagerAudio::PlayMusic(const MusicParameters& parameters)
{
    if (parameters.layer < 0 || parameters.layer >= (int)m_musicLayers.size())
        return false;

    if (parameters.audioClip || GetResources()->HasResource(parameters.audioClipId))
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
        parameters.audioClip = nullptr;
        parameters.audioClipId = "";
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
