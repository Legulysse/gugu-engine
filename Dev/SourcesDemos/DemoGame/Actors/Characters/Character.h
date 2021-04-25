#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"

#include <SFML/System/Vector2.hpp>

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
    
    void            Move        (sf::Vector2f _kDirection, const gugu::DeltaTime& dt);
    sf::Vector2f    GetPosition () const;

    bool TestCollision(class Projectile* _pProjectile);

    virtual void NotifyOpponentKilled(int value);

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

public:

    float m_walkSpeed;              // Distance walked per second
    float m_maxLife;                // Max Life

public:

    class Grid* m_grid;

    gugu::ElementSpriteAnimated* m_sprite;
    class ElementBar* m_lifeBar;

    float m_currentLife;
    bool m_isDead;

    bool m_steppedThisFrame;
    bool m_isMovingThisFrame;
    bool m_isActingThisFrame;
};

}   //namespace demoproject
