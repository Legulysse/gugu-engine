#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Audio/SoundInstance.h"
#include "Gugu/Audio/MusicInstance.h"
#include "Gugu/Audio/MusicLayer.h"

#include <map>
#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    struct EngineStats;
    class SoundCue;
    class AudioClip;
    class AudioMixerGroup;
    class AudioMixerGroupInstance;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerAudio
{
public:

    ManagerAudio();
    ~ManagerAudio();

    void Init(const EngineConfig& config);
    void Release();
    
    void Update(const DeltaTime& dt, EngineStats& stats);

    void SetListenerMuted(bool muted);
    bool IsListenerMuted() const;
    void SetListenerVolume(float volume);     // Volume range [0, 1]
    float GetListenerVolume() const;

    void SetListenerPosition(const Vector2f& position);

    const SpatializationParameters& GetDefaultSpatializationParameters() const;

    void SetRootAudioMixerGroup(AudioMixerGroup* rootMixerGroup);
    AudioMixerGroupInstance* GetMixerGroupInstance(const std::string& mixerGroupId) const;
    AudioMixerGroupInstance* GetMixerGroupInstance(AudioMixerGroup* mixerGroup) const;
    void RecomputeAllMixedVolumes();

    bool PlaySoundCue(const std::string& soundCueId, const Vector2f& position = Vector2::Zero_f);
    bool PlaySoundCue(SoundCue* soundCue, const Vector2f& position = Vector2::Zero_f);
    bool PlaySound(const std::string& audioClipId, float volume = 1.f);         // Volume range [0, 1]
    bool PlaySound(const SoundParameters& parameters);

    bool PlayMusic(const std::string& audioClipId, float volume = 1.f, float fade = 2.f);      // Volume range [0, 1]
    bool PlayMusic(const MusicParameters& parameters);
    bool PlayMusicList(const std::vector<MusicParameters>& playlist, bool loopPlaylist, int layer = 0);
    bool StopMusic(float fade = 2.f, int layer = 0);

    MusicInstance* GetCurrentMusicInstance(int layer) const;

private:

    AudioMixerGroupInstance* m_rootMixerGroupInstance;
    std::map<AudioMixerGroup*, AudioMixerGroupInstance*> m_mixerGroupInstances;

    std::vector<SoundInstance> m_soundInstances;
    std::vector<MusicInstance> m_musicInstances;
    std::vector<MusicLayer> m_musicLayers;

    size_t m_soundIndex;
    std::map<AudioClip*, float> m_audioClipCooldowns;

    bool m_listenerMuted;
    float m_listenerVolume;
    SpatializationParameters m_spatializationParameters;
};


ManagerAudio* GetAudio();


}   // namespace gugu
