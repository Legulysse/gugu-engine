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
    class MusicInstance;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct MusicParameters
{
    Music*      music;      // Music resource to play
    std::string musicID;    // If music is null, the system will try to load the resource specified by musicID
    float       volume;     // Volume range [0, 1]
    float       fadeIn;
    float       fadeOut;
	int			layer;

    MusicParameters();
};

class MusicLayer
{
public:

    MusicLayer();
    //MusicLayer(const MusicLayer&);
    ~MusicLayer();

    void SetInstances(MusicInstance* _pInstanceA, MusicInstance* _pInstanceB);

    void PurgeFade();

    void SetNext(const MusicParameters& _kParameters);
    void FadeToNext();

    void SetPlayList(const std::vector<MusicParameters>& _vecPlaylist, bool loopPlaylist);
    void StopPlaylist();

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
