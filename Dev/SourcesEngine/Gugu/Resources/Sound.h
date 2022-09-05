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
    
class Sound : public Resource
{
public:

    Sound();
    virtual ~Sound();

    sf::SoundBuffer* GetSFSoundBuffer() const;
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

protected:

    virtual void Unload() override;

protected:

    sf::SoundBuffer* m_sfSoundBuffer;
};

}   // namespace gugu
