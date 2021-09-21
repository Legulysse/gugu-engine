#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"
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

class Character : public gugu::Actor
{
public:

    Character();
    virtual ~Character();
    
    void            Move        (gugu::Vector2f _kDirection, const gugu::DeltaTime& dt);
    gugu::Vector2f    GetPosition () const;

    bool TestCollision(class Projectile* _pProjectile);

    virtual void ReceiveDamage(Character* source, float value);
    virtual void ReceiveHeal(float value);
    virtual void NotifyOpponentKilled(int value);

    virtual bool CanUseSkill(class DS_Skill* skill) const;

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

public:

    float m_walkSpeed;              // Distance walked per second
    float m_maxLife;                // Max Life

public:

    class Grid* m_grid;

    gugu::ElementSprite* m_sprite;
    gugu::SpriteAnimation* m_animation;
    class ElementBar* m_lifeBar;

    float m_currentLife;
    bool m_isDead;

    bool m_steppedThisFrame;
    bool m_isMovingThisFrame;
    bool m_isActingThisFrame;
};

}   //namespace demoproject
