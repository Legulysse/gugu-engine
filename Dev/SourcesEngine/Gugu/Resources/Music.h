#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Core/DeltaTime.h"

#include <SFML/Audio/Music.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Music : public Resource
{
public:

    Music();
    virtual ~Music();

    bool    LoadSFMusic (sf::Music* _pMusic);
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;

    DeltaTime GetDuration() const;
};

}   // namespace gugu
