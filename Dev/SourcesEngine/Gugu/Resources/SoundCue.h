#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Audio/SoundInstance.h"

#include <SFML/Audio/SoundBuffer.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Sound;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SoundCue : public Resource
{
public:

    SoundCue();
    virtual ~SoundCue();
    
    int     GetSoundCount   () const;
    bool    GetSound        (int _iIndex, SoundParameters& _kParameters) const;
    bool    GetRandomSound  (SoundParameters& _kParameters) const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool    LoadFromFile() override;

protected:

    std::vector<SoundParameters> m_audioFiles;
};

}   // namespace gugu
