#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Audio/Sound.hpp>

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Sound;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct SoundParameters
{
    gugu::Sound* sound;
    std::string soundID;
    float volume;
    int group;

    SoundParameters();
};

class SoundInstance
{
public:

    SoundInstance();
    //SoundInstance(const SoundInstance&);
    ~SoundInstance();

    void    Reset       ();

    void    SetGroup    (int _iGroup);
    int     GetGroup    () const;

    void    SetSound    (gugu::Sound* _pSound);
    void    SetVolume   (float _fVolume);

    void    Play        ();

protected:

    sf::Sound m_sfSound;
    int m_group;
};

}	// namespace gugu
