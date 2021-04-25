////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/Character.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Projectiles/Projectile.h"
#include "UI/ElementBar.h"
#include "Level/Grid.h"

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

    m_maxStamina = 90.f;
    m_staminaRecovery = 50.f;
    m_staminaRecoveryDelay = 1.f;

    m_attackSpeed = 150;
    m_attackStaminaCost = 0.1f;

    m_sprite = nullptr;
    m_lifeBar = nullptr;

    m_currentLife = m_maxLife;
    m_currentStamina = m_maxStamina;
    m_attackCooldown = 0.f;
    m_staminaRecoveryCooldown = 0.f;

    m_experience = 0;
    m_totalPoints = 0;
    m_spentPoints = 0;

    m_isDead = false;
}

Character::~Character()
{
    SafeDelete(m_sprite);

    m_lifeBar = nullptr;
    m_grid = nullptr;
}

void Character::InitCharacter(bool bPlayer, float _fSpeed, Grid* grid)
{
    m_grid = grid;
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

        m_lifeBar->SetVisible(false);
    }
}

void Character::Move(sf::Vector2f _kDirection, const DeltaTime& dt)
{
    if (_kDirection == sf::Vector2f())  //TODO: Constant for Zero vector
    {
        //if(!m_sprite->IsAnimationPlaying("IdleDown") && m_attackCooldown <= 0.f)  //TODO: param on StartAnimation to check current anim
        //    m_sprite->StartAnimation("IdleDown");
    }
    else
    {
        _kDirection = Normalize(_kDirection);

        sf::Vector2f position = Character::GetPosition();
        position += _kDirection * dt.s() * m_walkSpeed;

        m_grid->ClampPositionInsideBounds(position);
        m_sprite->SetPosition(position);

        float fAngleDegrees = ToDegreesf(atan2f(_kDirection.y, _kDirection.x));     //TODO: Atan2 dans Math.h + AngleVector en version degrees + radians + rename ToDegrees

        std::string strAnim = "IdleDown";
        if (fAngleDegrees >= -45.f && fAngleDegrees <= 45.f)
            strAnim = "WalkRight";
        else if (fAngleDegrees <= -135.f || fAngleDegrees >= 135.f)
            strAnim = "WalkLeft";
        else if (fAngleDegrees >= 45.f && fAngleDegrees <= 135.f)
            strAnim = "WalkDown";
        else if (fAngleDegrees <= -45.f && fAngleDegrees >= -135.f)
            strAnim = "WalkUp";

        if (!m_sprite->IsAnimationPlaying(strAnim) && m_attackCooldown <= 0.f)
            m_sprite->StartAnimation(strAnim);
    }
}

void Character::Attack(const sf::Vector2f& _kCoords, const DeltaTime& dt)
{
    if (m_attackCooldown <= 0.f && m_currentStamina > 0.f)   // Allow action when we have at least 1 stamina point (allow negative values)
    {
        float attackDelay = (m_attackSpeed > 0.f) ? 1.f / m_attackSpeed : 1.f;
        int nbAttacks = Max(1, (int)(dt.s() / attackDelay));

        m_attackCooldown = attackDelay * nbAttacks;
        m_staminaRecoveryCooldown = m_staminaRecoveryDelay;

        for (int i = 0; i < nbAttacks; ++i)
        {
            if (m_currentStamina <= 0.f)   // Allow action when we have at least 1 stamina point (allow negative values)
            {
                break;
            }

            m_currentStamina -= m_attackStaminaCost;

            Projectile* newProjectile = new Projectile();
            m_level->AddActor(newProjectile);

            newProjectile->InitProjectile(this, GetPosition(), _kCoords);
        }

        sf::Vector2f direction;
        if (_kCoords != GetPosition())
        {
            direction = Normalize(_kCoords - GetPosition());
        }
        else
        {
            direction = sf::Vector2f(1.f, 0.f);
        }

        float fAngleDegrees = ToDegreesf(atan2f(direction.y, direction.x));

        std::string strAnim = "";
        if (fAngleDegrees >= -45.f && fAngleDegrees <= 45.f)
            strAnim = "AttackRight";
        else if (fAngleDegrees <= -135.f || fAngleDegrees >= 135.f)
            strAnim = "AttackLeft";
        else if (fAngleDegrees >= 45.f && fAngleDegrees <= 135.f)
            strAnim = "AttackDown";
        else if (fAngleDegrees <= -45.f && fAngleDegrees >= -135.f)
            strAnim = "AttackUp";

        if (strAnim != "" && !m_sprite->IsAnimationPlaying(strAnim))
            m_sprite->StartAnimation(strAnim);
    }
}

bool Character::TestCollision(Projectile* _pProjectile)
{
    if (!_pProjectile)
        return false;

    if (LengthSquare(m_sprite->GetPosition() - _pProjectile->m_sprite->GetPosition()) > Power(16.f, 2))
        return false;

    m_lifeBar->SetVisible(true);

    m_currentLife -= 20.f;
    if (m_currentLife <= 0.f)
    {
        m_isDead = true;
        m_currentLife = 0.f;

        m_sprite->SetVisible(false);

        _pProjectile->m_characterSource->GainExperience(1);
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

void Character::GainExperience(int value)
{
    m_experience += value;
    m_totalPoints = m_experience / 25;
}

sf::Vector2f Character::GetPosition() const
{
    return m_sprite->GetPosition();
}

}   //namespace demoproject
