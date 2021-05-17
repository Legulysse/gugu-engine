#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"
#include "Gugu/Utility/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSpriteAnimated;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Character : public gugu::Actor
{
public:

    Character();
    virtual ~Character();
    
    void    InitCharacter       (int index);

    //
    void            Move        (sf::Vector2f _kDirection, const gugu::DeltaTime& dt);
    sf::Vector2f    GetPosition () const;

public:

    gugu::ElementSpriteAnimated* m_sprite;
};

}   //namespace demoproject
