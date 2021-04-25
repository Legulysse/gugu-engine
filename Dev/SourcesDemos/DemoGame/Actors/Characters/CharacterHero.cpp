////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/CharacterHero.h"

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

CharacterHero::CharacterHero()
{
    m_maxStamina = 90.f;
    m_staminaRecovery = 50.f;
    m_staminaRecoveryDelay = 1.f;

    m_attackSpeed = 150;
    m_attackStaminaCost = 0.1f;

    m_currentStamina = m_maxStamina;
    m_attackCooldown = 0.f;
    m_staminaRecoveryCooldown = 0.f;

    m_experience = 0;
    m_totalPoints = 0;
    m_spentPoints = 0;
}

CharacterHero::~CharacterHero()
{
}

void CharacterHero::Attack(const sf::Vector2f& _kCoords, const DeltaTime& dt)
{
    m_isActingThisFrame = true;

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

void CharacterHero::Step(const DeltaTime& dt)
{
    Character::Step(dt);

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
}

void CharacterHero::Update(const DeltaTime& dt)
{
    Character::Update(dt);

    // UI
    if (m_lifeBar)
    {
        m_lifeBar->SetValue(m_currentLife, m_maxLife);
    }
}

void CharacterHero::NotifyOpponentKilled(int value)
{
    m_experience += value;
    m_totalPoints = m_experience / 25;
}

}   //namespace demoproject
