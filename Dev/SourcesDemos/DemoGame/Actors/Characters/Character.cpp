////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/Character.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Projectiles/Projectile.h"
#include "UI/ElementBar.h"

#include "Gugu/World/Level.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Utility/Random.h"
#include "Gugu/Utility/System.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Character::Character()
{
	m_walkSpeed = 100.f;

    m_maxLife = 100.f;

    m_maxStamina = 100.f;
    m_staminaRecovery = 50.f;
    m_staminaRecoveryDelay = 1.f;

    m_attackSpeed = 5.f;
    m_attackStaminaCost = 2.f;

    m_sprite = nullptr;
    m_lifeBar = nullptr;

    m_currentLife = m_maxLife;
    m_currentStamina = m_maxStamina;
	m_attackCooldown = 0.f;
    m_staminaRecoveryCooldown = 0.f;

    m_isDead = false;
}

Character::~Character()
{
	SafeDelete(m_sprite);
}

void Character::InitCharacter(bool bPlayer, float _fSpeed)
{
	m_walkSpeed = _fSpeed;

	m_sprite = m_level->GetRootNode()->AddChild<ElementSpriteAnimated>();

    if (bPlayer)
    {
        m_sprite->ChangeAnimSet("Human.animset.xml");
    }
    else
    {
        if (GetRandom(0, 1) == 0)
            m_sprite->ChangeAnimSet("Orc.animset.xml");
        else
            m_sprite->ChangeAnimSet("Lady.animset.xml");
    }

	m_sprite->StartAnimation("IdleDown");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    if (bPlayer)
    {
        //m_pSprite->SetColor(sf::Color::Cyan);
        m_sprite->SetZIndex(1000);
    }
    else
    {
        m_lifeBar = m_sprite->AddChild<ElementBar>();
        m_lifeBar->InitBar(ElementBar::BarColor::Red, 2.f);
        m_lifeBar->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        m_lifeBar->SetUnifiedPosition(UDim2(0.5f, 0.f, 0.f, -5.f));
        m_lifeBar->SetSize(32.f, 4.f);
    }
}

void Character::Move(sf::Vector2f _kDirection, const DeltaTime& dt)
{
    if (_kDirection == sf::Vector2f())  //TODO: Constant for Zero vector
    {
        if(!m_sprite->IsAnimationPlaying("Idle"))  //TODO: param on StartAnimation to check current anim
            m_sprite->StartAnimation("Idle");
    }
    else
    {
        _kDirection = Normalize(_kDirection);
        m_sprite->Move(_kDirection * dt.s() * m_walkSpeed);

        float fAngleDegrees = ToDegreesf(atan2f(_kDirection.y, _kDirection.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

        std::string strAnim = "Idle";

        if (fAngleDegrees >= -45.f && fAngleDegrees <= 45.f)
            strAnim = "WalkRight";
        else if (fAngleDegrees <= -135.f || fAngleDegrees >= 135.f)
            strAnim = "WalkLeft";
        else if (fAngleDegrees >= 45.f && fAngleDegrees <= 135.f)
            strAnim = "WalkDown";
        else if (fAngleDegrees <= -45.f && fAngleDegrees >= -135.f)
            strAnim = "WalkUp";

        if (!m_sprite->IsAnimationPlaying(strAnim))
            m_sprite->StartAnimation(strAnim);
    }
}

void Character::Attack(const sf::Vector2f& _kCoords)
{
    if (m_attackCooldown <= 0.f && m_currentStamina > 0.f)   // Allow action when we have at least 1 stamina point (allow negative values)
	{
        m_currentStamina -= m_attackStaminaCost;
        m_staminaRecoveryCooldown = m_staminaRecoveryDelay;

        m_attackCooldown = (m_attackSpeed > 0.f) ? 1.f / m_attackSpeed : 1.f;

		Projectile* newProjectile = new Projectile();
		m_level->AddActor(newProjectile);

		newProjectile->InitProjectile(this, GetPosition(), _kCoords);
	}
}

bool Character::TestCollision(class Projectile* _pProjectile)
{
    if (!_pProjectile)
        return false;

    if (LengthSquare(m_sprite->GetPosition() - _pProjectile->m_sprite->GetPosition()) > Power(16.f, 2))
        return false;

    m_currentLife -= 20.f;
    if (m_currentLife <= 0.f)
    {
        m_isDead = true;
        m_currentLife = 0.f;

        m_sprite->SetVisible(false);
    }

    return true;
}

void Character::Step(const DeltaTime& dt)
{
	Actor::Step(dt);

    // Stamina
    if (m_staminaRecoveryCooldown > 0.f)
    {
        m_staminaRecoveryCooldown -= dt.s();
    }

    if (m_staminaRecoveryCooldown <= 0.f)
    {
        m_currentStamina += dt.s() * m_staminaRecovery;
        m_currentStamina = Min(m_currentStamina, m_maxStamina);
    }

    // Attack
    if (m_attackCooldown > 0.f)
	{
        m_attackCooldown -= dt.s();
	}

    // UI
    if (m_lifeBar)
    {
        m_lifeBar->SetValue(m_currentLife, m_maxLife);
    }
}

sf::Vector2f Character::GetPosition() const
{
    return m_sprite->GetPosition();
}

}   //namespace demoproject
