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
    class SoundCue;
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
    
    void Update(const DeltaTime& dt);

    void SetMasterMuted(bool muted);
    bool IsMasterMuted() const;
    void SetMasterVolume(float volume);     // Volume range [0, 1]
    void SetMasterVolume100(int volume);    // Volume range [0, 100]
    float GetMasterVolume() const;
    int GetMasterVolume100() const;

    void SetAudioMixer(AudioMixerGroup* rootMixerGroup);
    AudioMixerGroupInstance* GetMixerGroupInstance(const std::string& mixerGroupId) const;
    AudioMixerGroupInstance* GetMixerGroupInstance(AudioMixerGroup* mixerGroup) const;

    bool PlaySoundCue(const std::string& soundCueID);
    bool PlaySoundCue(SoundCue* soundCue);
    bool PlaySound(const std::string& soundID, float volume = 1.f, int group = 0);         // Volume range [0, 1]
    bool PlaySound(const SoundParameters& parameters);

    bool PlayMusic(const std::string& musicID, float volume = 1.f, float fade = 2.f);      // Volume range [0, 1]
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

    bool m_masterMuted;
    float m_masterVolume;
};


ManagerAudio* GetAudio();


}   // namespace gugu
