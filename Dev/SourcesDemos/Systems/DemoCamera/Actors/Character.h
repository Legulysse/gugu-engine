#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/SceneActor.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementSprite;
    class SpriteAnimation;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Character : public gugu::SceneActor
{
public:

    Character();
    virtual ~Character();
    
    void InitCharacter(int index);

    void Move(gugu::Vector2f _kDirection, const gugu::DeltaTime& dt);
    gugu::Vector2f GetPosition() const;

public:

    gugu::ElementSprite* m_sprite;
    gugu::SpriteAnimation* m_spriteAnimation;
};

}   //namespace demoproject
