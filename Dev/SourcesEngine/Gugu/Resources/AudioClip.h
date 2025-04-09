#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <SFML/System/Time.hpp>

namespace sf
{
    class SoundBuffer;
    class Music;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AudioClip : public Resource
{
public:

    AudioClip();
    virtual ~AudioClip();

    sf::SoundBuffer* GetOrLoadSFSoundBuffer();
    sf::Music* OpenSFMusicStream();

    sf::Time GetOrReadDuration();
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

protected:

    virtual void Unload() override;

protected:

    sf::SoundBuffer* m_sfSoundBuffer;
    sf::Time m_duration;
};

}   // namespace gugu
