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

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Projectile::Projectile()
{
    m_sprite = nullptr;
    m_speed = 400.f;
    m_lifetime = 1.f;
}

Projectile::~Projectile()
{
    SafeDelete(m_sprite);
}

void Projectile::InitProjectile(Character* _pSource, const sf::Vector2f& _kFrom, const sf::Vector2f& _kTo)
{
    m_characterSource = _pSource;
    if (_kTo != _kFrom)
        m_direction = Normalize(_kTo - _kFrom);
    else
        m_direction = sf::Vector2f(1.f, 0.f);

    sf::Vector2f kOffset = m_direction * 20.f;
    float fAngleDegrees = ToDegreesf(atan2f(m_direction.y, m_direction.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

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
