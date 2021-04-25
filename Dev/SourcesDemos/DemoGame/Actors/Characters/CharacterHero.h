#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/Character.h"

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class CharacterHero : public Character
{
public:

    CharacterHero();
    virtual ~CharacterHero();

    void InitHero(class DS_Hero* sheetHero, float _fSpeed, class Grid* grid);

    void Attack(const sf::Vector2f& _kCoords, const gugu::DeltaTime& dt);

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual void NotifyOpponentKilled(int value) override;

public:

    float m_maxStamina;             // Max Stamina
    float m_staminaRecovery;        // Stamina recovery per second
    float m_staminaRecoveryDelay;   // Delay in seconds before stamina recovery

    float m_attackSpeed;            // Nb attacks per second
    float m_attackStaminaCost;      // Stamina cost per attack

public:

    float m_currentStamina;

    float m_attackCooldown;
    float m_staminaRecoveryCooldown;

    int m_experience;
    int m_totalPoints;
    int m_spentPoints;
};

}   //namespace demoproject
