#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Skills/SkillUtility.h"

#include "Gugu/Scene/SceneActor.h"
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
    class ElementParticles;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Projectile : public gugu::SceneActor
{
public:

    Projectile();
    virtual ~Projectile();

    void InitProjectile(const SkillContext& skillContext, const DS_EffectProjectile* effectSource, const gugu::Vector2f& _kFrom, const gugu::Vector2f& _kTo);
    void OnHit(Character* character);

    virtual void Step(const gugu::DeltaTime& dt) override;

public:

    SkillContext m_skillContext;
    const DS_EffectProjectile* m_effectSource;
    Character* m_characterSource;
    gugu::ElementSprite* m_sprite;
    gugu::Vector2f m_direction;
    gugu::Vector2f m_destination;
    gugu::ElementParticles* m_deathParticles;
    bool m_isFireball;
    bool m_isBowAttack;
    float m_speed;
    float m_lifetime;
    bool m_hasLifetime;
    bool m_hasDestination;
    int m_hitCount;
    std::set<Character*> m_characterHits;
    bool m_isDead;
    bool m_pendingDestroy;
};

}   //namespace demoproject
