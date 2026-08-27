////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/ManagerAudio.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Audio/AudioMixerGroupInstance.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/SoundCue.h"
#include "Gugu/Resources/AudioMixerGroup.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/Container.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Audio/Listener.hpp>

#include <assert.h>

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_AUDIO_SOUNDCUE_RESTRICTION_BY_COOLDOWN true        // Toggle restriction on the replay of a same soundcue file.

#define GUGU_AUDIO_AUDIOCLIP_RESTRICTION_BY_COOLDOWN true       // Toggle restriction on the replay of a same audioclip (wav) file.
#define GUGU_AUDIO_AUDIOCLIP_RESTRICTION_COOLDOWN_VALUE 0.05f   // Cooldown to restrict the replay of a same audioclip (wav) file.
#define GUGU_AUDIO_AUDIOCLIP_RESTRICTION_BY_INSTANCES true      // Toggle restriction on instances of a same audioclip (wav) file.
#define GUGU_AUDIO_AUDIOCLIP_RESTRICTION_INSTANCE_COUNT 8       // Max number of instances of a same audioclip (wav) file.

#define GUGU_AUDIO_SPATIALIZATION_MIN_DISTANCE 200.f            // Default minimum distance between listener and audio instance to apply attenuation.
#define GUGU_AUDIO_SPATIALIZATION_ATTENUATION 0.5f              // Default attenuation factor.

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAudio::ManagerAudio()
    : m_rootMixerGroupInstance(nullptr)
    , m_elapsedTimeReference(0)
    , m_listenerMuted(false)
    , m_listenerVolume(1.f)
    , m_defaultAudioListenerDistance(0.f)
{
    m_spatializationParameters.minDistance = GUGU_AUDIO_SPATIALIZATION_MIN_DISTANCE;
    m_spatializationParameters.attenuation = GUGU_AUDIO_SPATIALIZATION_ATTENUATION;
}

ManagerAudio::~ManagerAudio()
{
    SafeDelete(m_rootMixerGroupInstance);
    m_mixerGroupInstances.clear();
}

void ManagerAudio::Init(const EngineConfig& config)
{
    GetLogEngine()->Print(ELog::Info, ELogEngine::Audio, "Init Manager Audio...");

    // Notes:
    // - Instantiating sf::Sound objects may generate some constant noise in headphones.
    // - Allowing zero sources would make it possible for the engine to keep audio disconnected and silent.
    // - Another approach would be to only instantiate those on demand (need some refactor).
    m_soundInstances.resize(Max(12, config.maxSoundSourceCount));
    m_musicInstances.resize(Max(4, config.maxMusicSourceCount));
    m_musicLayers.resize(Max(2, (int)m_musicInstances.size() / 2));

    for (size_t i = 0; i < m_musicLayers.size(); ++i)
    {
        m_musicLayers[i].SetInstances(&m_musicInstances[i*2], &m_musicInstances[i*2 + 1]);
    }

    SetRootAudioMixerGroup(GetResources()->GetAudioMixerGroup(config.rootAudioMixerGroup));

    // Default spatialization settings.
    m_defaultAudioListenerDistance = (float)config.audioListenerDistance;
    SetListenerPosition(Vector2::Zero_f);

    GetLogEngine()->Print(ELog::Info, ELogEngine::Audio, "Manager Audio Ready");
}

void ManagerAudio::Release()
{
    m_audioClipCooldowns.clear();
    m_soundCueCooldowns.clear();

    m_musicLayers.clear();
    m_musicInstances.clear();
    m_soundInstances.clear();
}

void ManagerAudio::Update(const DeltaTime& dt, EngineStats& stats)
{
    stats.soundInstanceCount = 0;

    // Update elapsed update count (used to track sound instance lifetimes).
    m_elapsedTimeReference += 1;

    // Update audioclip cooldowns.
    auto itAudioClip = m_audioClipCooldowns.begin();
    while (itAudioClip != m_audioClipCooldowns.end())
    {
        itAudioClip->second -= dt.unscaled_s(); // Unscaled because it is only here to reduce saturation.

        if (itAudioClip->second <= 0.f)
            itAudioClip = m_audioClipCooldowns.erase(itAudioClip);
        else
            ++itAudioClip;
    }

    // Update soundcue cooldowns.
    auto itSoundCue = m_soundCueCooldowns.begin();
    while (itSoundCue != m_soundCueCooldowns.end())
    {
        itSoundCue->second -= dt.s();           // Scaled to match gameplay timings.

        if (itSoundCue->second <= 0.f)
            itSoundCue = m_soundCueCooldowns.erase(itSoundCue);
        else
            ++itSoundCue;
    }

    // Update sound instances (reset if not active).
    for (size_t i = 0; i < m_soundInstances.size(); ++i)
    {
        if (m_soundInstances[i].UpdateStatus())
        {
            ++stats.soundInstanceCount;
        }
    }

    // Update layers (fades).
    for (size_t i = 0; i < m_musicLayers.size(); ++i)
    {
        m_musicLayers[i].Update(dt);
    }
}

void ManagerAudio::SetListenerMuted(bool muted)
{
    m_listenerMuted = muted;

    float computedVolume = ComputeVolumeCurve(m_listenerVolume);
    sf::Listener::setGlobalVolume(m_listenerMuted ? 0.f : computedVolume * 100.f);
}

bool ManagerAudio::IsListenerMuted() const
{
    return m_listenerMuted;
}

void ManagerAudio::SetListenerVolume(float volume)
{
    m_listenerVolume = volume;

    float computedVolume = ComputeVolumeCurve(m_listenerVolume);
    sf::Listener::setGlobalVolume(m_listenerMuted ? 0.f : computedVolume * 100.f);
}

float ManagerAudio::GetListenerVolume() const
{
    return m_listenerVolume;
}

void ManagerAudio::SetListenerPosition(const Vector2f& position)
{
    sf::Listener::setPosition(sf::Vector3f(position.x, m_defaultAudioListenerDistance, position.y));
}

const SpatializationParameters& ManagerAudio::GetDefaultSpatializationParameters() const
{
    return m_spatializationParameters;
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

bool ManagerAudio::PlaySoundCue(const std::string& soundCueId, const Vector2f& position)
{
    return PlaySoundCue(GetResources()->GetSoundCue(soundCueId), position);
}

bool ManagerAudio::PlaySoundCue(SoundCue* soundCue, const Vector2f& position)
{
    if (!soundCue || StdMapContainsKey(m_soundCueCooldowns, soundCue))
        return false;

    // TODO: I could check with running instances if I should pick an AudioClip that is not currently being played.
    SoundParameters parameters;
    if (!soundCue->GetRandomSound(parameters))
        return false;

    parameters.position = position;

    bool result = PlaySound(parameters);

#if GUGU_AUDIO_SOUNDCUE_RESTRICTION_BY_COOLDOWN
    if (result && parameters.cooldownRange.x > 0.f)
    {
        if (parameters.useRandomCooldown)
        {
            float cooldown = GetRandomf(parameters.cooldownRange.x, parameters.cooldownRange.y);
            m_soundCueCooldowns.insert(std::make_pair(soundCue, cooldown));
        }
        else
        {
            m_soundCueCooldowns.insert(std::make_pair(soundCue, parameters.cooldownRange.x));
        }
    }
#endif

    return result;
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

    if (audioClip && !StdMapContainsKey(m_audioClipCooldowns, audioClip))
    {
#if GUGU_AUDIO_AUDIOCLIP_RESTRICTION_BY_INSTANCES
        // Upper count limit.
        int count = 0;
        for (auto& soundInstance : m_soundInstances)
        {
            if (soundInstance.GetAudioClip() == audioClip)
            {
                ++count;
            }
        }

        // TODO: Pop oldest instead of arbitrary order.
        int removeCount = count - (GUGU_AUDIO_AUDIOCLIP_RESTRICTION_INSTANCE_COUNT - 1);
        for (auto& soundInstance : m_soundInstances)
        {
            if (soundInstance.GetAudioClip() == audioClip && removeCount > 0)
            {
                soundInstance.Reset();
                --removeCount;
            }
        }
#endif

        // Find an available instance.
        size_t soundIndex = 0;
        size_t bestPopIndex = system::InvalidIndex;
        uint64 bestPopIndexTimeReference = 0;

        while (soundIndex < m_soundInstances.size()
            && m_soundInstances[soundIndex].IsActive())
        {
            uint64 timeReference = m_soundInstances[soundIndex].GetStartTimeReference();

            // Default : pop oldest.
            if (bestPopIndex == system::InvalidIndex
                || timeReference < bestPopIndexTimeReference)
            {
                bestPopIndex = soundIndex;
                bestPopIndexTimeReference = timeReference;
            }

            ++soundIndex;
        }

        // If no instance is available, pop the best candidate.
        if (soundIndex == m_soundInstances.size())
        {
            assert(bestPopIndex < m_soundInstances.size());
            soundIndex = bestPopIndex;
        }

        assert(soundIndex < m_soundInstances.size());

        SoundInstance* soundInstance = &m_soundInstances[soundIndex];
        soundInstance->Reset();
        soundInstance->SetAudioClip(audioClip);
        soundInstance->SetMixerGroupInstance(mixerGroupInstance);   // Note: I currently allow a null group instance.
        
        if (parameters.volumeRandomRange != Vector2::Zero_f)
        {
            assert(parameters.volumeRandomRange.x <= 0.f && parameters.volumeRandomRange.y >= 0.f);

            float volume = parameters.volume * (1.f + GetRandomf(parameters.volumeRandomRange.x, parameters.volumeRandomRange.y));
            soundInstance->SetVolume(volume);
        }
        else
        {
            soundInstance->SetVolume(parameters.volume);
        }

        if (parameters.pitchRandomRange != Vector2::Zero_f)
        {
            assert(parameters.pitchRandomRange.x <= 0.f && parameters.pitchRandomRange.y >= 0.f);

            float pitch = 1.f + GetRandomf(parameters.pitchRandomRange.x, parameters.pitchRandomRange.y);
            soundInstance->SetPitch(pitch);
        }

        if (parameters.spatialized)
        {
            SpatializationParameters spatializationParameters = m_spatializationParameters;

            if (parameters.spatializationParameters.override)
            {
                spatializationParameters = parameters.spatializationParameters;
            }
            else if (mixerGroupInstance)
            {
                mixerGroupInstance->GetSpatializationParameters(spatializationParameters);
            }

            soundInstance->SetSpatialization(true, spatializationParameters);
            soundInstance->SetPosition(parameters.position);
        }

        soundInstance->Play(m_elapsedTimeReference);

#if GUGU_AUDIO_AUDIOCLIP_RESTRICTION_BY_COOLDOWN
        // Add cooldown on the clip to limit superposition.
        m_audioClipCooldowns.insert(std::make_pair(audioClip, GUGU_AUDIO_AUDIOCLIP_RESTRICTION_COOLDOWN_VALUE));
#endif
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
