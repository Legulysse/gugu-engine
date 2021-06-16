#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/Character.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class CharacterHero : public Character
{
public:

    CharacterHero();
    virtual ~CharacterHero();

    void InitHero(class DS_Hero* sheetHero, float _fSpeed, class Grid* grid, gugu::Element* parentNode);

    void UseSkillByIndex(int index, const gugu::Vector2f& _kCoords, const gugu::DeltaTime& dt);
    void UseSkill(DS_Skill* skill, const gugu::Vector2f& _kCoords, const gugu::DeltaTime& dt);

    virtual bool CanUseSkill(class DS_Skill* skill) const override;

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual void NotifyOpponentKilled(int value) override;

public:

    float m_maxStamina;             // Max Stamina
    float m_staminaRecovery;        // Stamina recovery per second
    float m_staminaRecoveryDelay;   // Delay in seconds before stamina recovery

    float m_maxMana;                // Max Mana
    float m_manaRecovery;           // Mana recovery per second

    float m_attackSpeed;            // Nb attacks per second

public:

    float m_currentStamina;
    float m_staminaRecoveryCooldown;

    float m_currentMana;

    float m_attackCooldown;
    float m_skillCooldown;

    int m_experience;
    int m_totalPoints;
    int m_spentPoints;
};

}   //namespace demoproject
