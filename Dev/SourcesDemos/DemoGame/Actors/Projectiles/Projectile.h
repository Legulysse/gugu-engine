#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Skills/SkillUtility.h"

#include "Gugu/World/Actor.h"
#include "Gugu/Math/Vector2.h"

#include <set>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class DS_EffectProjectile;
    class Character;
}

namespace gugu
{
    class Element;
    class ElementSprite;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Projectile : public gugu::Actor
{
public:

    Projectile();
    virtual ~Projectile();

    void InitProjectile(const SkillContext& skillContext, DS_EffectProjectile* effectSource, const gugu::Vector2f& _kFrom, const gugu::Vector2f& _kTo);
    bool OnHit(Character* character);

    virtual void Step(const gugu::DeltaTime& dt) override;

public:

    SkillContext m_skillContext;
    DS_EffectProjectile* m_effectSource;
    Character* m_characterSource;
    gugu::ElementSprite* m_sprite;
    gugu::Vector2f m_direction;
    gugu::Vector2f m_destination;
    float m_speed;
    float m_lifetime;
    bool m_hasLifetime;
    bool m_hasDestination;
    int m_hitCount;
    std::set<Character*> m_characterHits;
};

}   //namespace demoproject
