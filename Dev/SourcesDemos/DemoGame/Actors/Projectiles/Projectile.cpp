////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Projectiles/Projectile.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"

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

void Projectile::InitProjectile(Character* _pSource, const sf::Vector2f& _kFrom, const sf::Vector2f& _kTo)
{
    m_characterSource = _pSource;

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

    // Add random amplitude
    float spreadAmplitude = InverseLerp(50.f, 400.f, Clamp(targetDistance, 50.f, 400.f));
    spreadAmplitude = 1.f - spreadAmplitude;
    spreadAmplitude = Lerp(0.05f, 1.25f, spreadAmplitude);
    m_direction = Rotate(m_direction, GetRandomf(-spreadAmplitude, spreadAmplitude));
    float fAngleDegrees = ToDegreesf(atan2f(m_direction.y, m_direction.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

    // Add random start offset
    sf::Vector2f kOffset = m_direction * GetRandomf(20.f, 25.f);

    // Add random speed
    m_speed += GetRandomf(-50, 50);

    m_sprite = m_level->GetRootNode()->AddChild<ElementSprite>();
    m_sprite->SetTexture("Arrow.png");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    m_sprite->SetRotation(fAngleDegrees + 90.f);
    m_sprite->SetPosition(_kFrom + kOffset);

    GetGame()->m_projectiles.push_back(this);
}

void Projectile::Step(const DeltaTime& dt)
{
    Actor::Step(dt);

    m_sprite->Move(m_direction * dt.s() * m_speed);

    m_lifetime -= dt.s();
    if (m_lifetime < 0.f)
    {
        StdVectorRemove(GetGame()->m_projectiles, this);
        m_level->DeleteActor(this);
    }
}

}   //namespace demoproject
