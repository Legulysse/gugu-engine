#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

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

    sf::Time GetDuration() const;
    
    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
};

}   // namespace gugu
