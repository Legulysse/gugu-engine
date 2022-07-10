#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Music;
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

    void    SetVolume       (float _fVolume);

    void    SetFadeCoeff    (float _fFadeCoeff);
    float   GetFadeCoeff    () const;

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
    sf::Music*  m_sfMusic;
    float       m_volume;

    float       m_fadeCoeff;
};

}   // namespace gugu
