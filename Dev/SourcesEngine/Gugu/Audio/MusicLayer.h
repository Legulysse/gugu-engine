#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class Music;
    class AudioMixerGroupInstance;
    class MusicInstance;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct MusicParameters
{
    Music*      music;      // Music resource to play
    std::string musicID;    // If music is null, the system will try to load the resource specified by musicID
    AudioMixerGroupInstance* mixerGroupInstance;
    std::string mixerGroupID;
    float       volume;     // Volume range [0, 1]
    float       fadeIn;
    float       fadeOut;
    int         layer;
    bool        loop;

    MusicParameters();
};

class MusicLayer
{
public:

    MusicLayer();
    ~MusicLayer();

    void SetInstances(MusicInstance* instanceA, MusicInstance* instanceB);
    void Reset();

    void SetNext(const MusicParameters& parameters);
    void SetPlayList(const std::vector<MusicParameters>& playlist, bool loopPlaylist);

    void FadeToNext();
    void PurgeFade();

    void Update(const DeltaTime& dt);

    MusicInstance* GetCurrentMusicInstance() const;

private:

    std::vector<MusicParameters> m_playlist;
    int m_playlistIndex;
    bool m_loopPlaylist;

    MusicInstance* m_currentInstance;
    MusicInstance* m_nextInstance;

    float m_fadeIn;
    float m_fadeOut;
    float m_currentFadeTime;
    bool m_isFading;
};

}   // namespace gugu
