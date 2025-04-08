#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Music;
    class AudioMixerGroupInstance;
}

namespace sf
{
    class Music;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class MusicInstance
{
public:

    MusicInstance   ();
    ~MusicInstance  ();

    void    Reset           ();

    void    SetMusic        (Music* _pMusic, bool _bLoop);
    bool    IsSet() const;
    Music*  GetMusic() const;

    void    SetMixerGroupInstance(AudioMixerGroupInstance* mixerGroupInstance);
    void    SetVolume       (float _fVolume);

    void    SetFadeCoeff    (float _fFadeCoeff);
    float   GetFadeCoeff    () const;

    void RecomputeMixedVolume();

    void    Play            ();
    void    Pause           ();
    void    Stop            ();

    sf::Time GetDuration() const;
    sf::Time GetPlayOffset() const;
    void SetPlayOffset(const sf::Time& offset);
    //TODO: Investigate loop points feature

    bool    IsPaused() const;
    bool    IsFinished() const;

private:

    Music*      m_music;
    AudioMixerGroupInstance* m_mixerGroupInstance;
    sf::Music*  m_sfMusic;
    float       m_volume;

    float       m_fadeCoeff;
};

}   // namespace gugu
