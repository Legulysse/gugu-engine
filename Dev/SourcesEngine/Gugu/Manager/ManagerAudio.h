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
    class Sound;
    class Music;
    class AudioCue;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerAudio
{
public:

    ManagerAudio();
    ~ManagerAudio();

    void    Init    (const EngineConfig& config);
    void    Release ();
    
    void    Update  (const DeltaTime& dt);

    void    SetVolumeMaster (float _fVolume);   // Volume range [0, 1]
    float   GetVolumeMaster () const;

    //void    SetVolumeGroup  (int _iGroup, float _fVolume);    //TODO
    //float   GetVolumeGroup  (int _iGroup) const;              //TODO

    bool    PlaySoundCue    (const std::string& _strFile);
    bool    PlaySound       (const std::string& _strFile, float _fVolume = 1.f, int _iGroup = 0);     // Volume range [0, 1]
    bool    PlaySound       (const SoundParameters& _kParameters);

    bool    PlayMusic       (const std::string& _strFile, float _fVolume = 1.f, float _fFade = 2.f);   // Volume range [0, 1]
    bool    PlayMusic       (const MusicParameters& _kParameters);
    bool    PlayMusicList   (const std::vector<MusicParameters>& _vecPlaylist);
    bool    StopMusic       (float _fFade = 2.f);

    //TODO: Provide layer parameter (currently only index 0 is valid)
    MusicInstance* GetCurrentMusicInstance() const;

private:

    std::vector<SoundInstance>  m_soundInstances;
    std::vector<MusicInstance>  m_musicInstances;
    std::vector<MusicLayer>     m_musicLayers;

    size_t                      m_soundIndex;
};


ManagerAudio* GetAudio();


}   // namespace gugu
