////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Projectiles/Projectile.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"

#include "DatasheetBinding.h"

#include "Gugu/World/Level.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Utility/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Projectile::Projectile()
{
    m_sprite = nullptr;
    m_speed = 650;
    m_lifetime = 2.f;
}

Projectile::~Projectile()
{
    SafeDelete(m_sprite);
}

void Projectile::InitProjectile(const SkillContext& skillContext, DS_EffectProjectile* effectSource, const sf::Vector2f& _kFrom, const sf::Vector2f& _kTo)
{
    m_skillContext = skillContext;
    m_effectSource = effectSource;

    m_characterSource = m_skillContext.caster;
    m_speed = effectSource->speed;

    m_lifetime = effectSource->lifetime;
    m_destination = _kTo;

    m_hitCount = 0;
    m_hasLifetime = m_lifetime > 0.f;
    m_hasDestination = m_effectSource->aim == EProjectileAim::Cursor;

    float targetDistance = 1.f;
    if (_kTo != _kFrom)
    {
        targetDistance = Length(_kTo - _kFrom);
        m_direction = Normalize(_kTo - _kFrom);
    }
    else
    {
        m_direction = sf::Vector2f(1.f, 0.f);
    }

    sf::Vector2f kOffset = sf::Vector2f();
    if (!m_hasDestination)
    {
        // Add random amplitude
        float spreadAmplitude = InverseLerp(50.f, 400.f, Clamp(targetDistance, 50.f, 400.f));
        spreadAmplitude = 1.f - spreadAmplitude;
        spreadAmplitude = Lerp(0.05f, 1.25f, spreadAmplitude);
        m_direction = Rotate(m_direction, GetRandomf(-spreadAmplitude, spreadAmplitude));

        // Add random start offset
        kOffset = m_direction * GetRandomf(20.f, 25.f);

        // Add random speed
        m_speed += GetRandomf(-50, 50);
    }

    float fAngleDegrees = ToDegreesf(atan2f(m_direction.y, m_direction.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

    m_sprite = m_level->GetRootNode()->AddChild<ElementSprite>();
    m_sprite->SetTexture("Arrow.png");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    m_sprite->SetRotation(fAngleDegrees + 90.f);
    m_sprite->SetPosition(_kFrom + kOffset);

    GetGame()->m_projectiles.push_back(this);
}

bool Projectile::OnHit(Character* character)
{
    // TODO: handle only one hit per character.
    SkillUtility::ApplySkillEffectList(m_skillContext, m_effectSource->effectsOnHit, character, m_sprite->GetPosition());

    ++m_hitCount;
    if (m_effectSource->maximumHits >= 0)
    {
        return m_hitCount >= m_effectSource->maximumHits;    // true = Destroy projectile
    }

    return false;    // true = Destroy projectile
}

void Projectile::Step(const DeltaTime& dt)
{
    Actor::Step(dt);

    bool reachedDestination = false;
    sf::Vector2f moveStep = m_direction * dt.s() * m_speed;

    if (m_hasDestination)
    {
        if (LengthSquare(m_sprite->GetPosition() - m_destination) <= LengthSquare(moveStep) + Math::Epsilon)
        {
            reachedDestination = true;
        }
    }

    if (m_hasLifetime)
    {
        m_lifetime -= dt.s();
        if (m_lifetime < 0.f)
        {
            reachedDestination = true;
        }
    }

    m_sprite->Move(moveStep);

    if (reachedDestination)
    {
        SkillUtility::ApplySkillEffectList(m_skillContext, m_effectSource->effectsOnDestination, nullptr, m_sprite->GetPosition());

        StdVectorRemove(GetGame()->m_projectiles, this);
        m_level->DeleteActor(this);
    }
}

}   //namespace demoproject
