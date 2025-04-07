#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

namespace sf
{
    class SoundBuffer;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class AudioClip : public Resource
{
public:

    AudioClip();
    virtual ~AudioClip();

    // TODO: Get as Buffer and/or Stream.
    sf::SoundBuffer* GetSFSoundBuffer() const;
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

protected:

    virtual void Unload() override;

protected:

    sf::SoundBuffer* m_sfSoundBuffer;
};

}   // namespace gugu
