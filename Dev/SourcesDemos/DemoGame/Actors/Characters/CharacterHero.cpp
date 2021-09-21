////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/CharacterHero.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Projectiles/Projectile.h"
#include "UI/ElementBar.h"
#include "Level/Grid.h"
#include "Skills/SkillUtility.h"

#include "DatasheetBinding.h"

#include "Gugu/World/Level.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

CharacterHero::CharacterHero()
{
    m_attackSpeed = 10;

    m_attackCooldown = 0.f;
    m_skillCooldown = 0.f;
    m_staminaRecoveryCooldown = 0.f;

    m_experience = 0;
    m_totalPoints = 0;
    m_spentPoints = 0;
}

CharacterHero::~CharacterHero()
{
}

void CharacterHero::InitHero(DS_Hero* sheetHero, float _fSpeed, Grid* grid, Element* parentNode)
{
    m_grid = grid;

    // Init stats from datasheet
    m_maxLife = sheetHero->health;
    m_walkSpeed = sheetHero->speed;

    m_maxStamina = sheetHero->stamina;
    m_staminaRecovery = sheetHero->staminaRecovery;
    m_staminaRecoveryDelay = sheetHero->staminaRecoveryDelay;

    m_maxMana = sheetHero->mana;
    m_manaRecovery = sheetHero->manaRecovery;

    m_currentLife = m_maxLife;
    m_currentStamina = m_maxStamina;
    m_currentMana = m_maxMana;

    // Sprite
    m_sprite = parentNode->AddChild<ElementSprite>();
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    m_animation = GetAnimations()->AddAnimation(m_sprite);
    m_animation->ChangeAnimSet(sheetHero->sprite->animSet);
    m_animation->StartAnimation("IdleDown");

    //m_pSprite->SetColor(sf::Color::Cyan);
    //m_sprite->SetZIndex(1000);
}

void CharacterHero::UseSkillByIndex(int index, const Vector2f& _kCoords, const gugu::DeltaTime& dt)
{
    // Index 0 is the base attack skill.
    if (index == 0)
    {
        DS_Skill* skill = GetResources()->GetDatasheet<DS_Skill>("BowAttack.skill");
        UseSkill(skill, _kCoords, dt);
    }
    else
    {
        DS_Skill* skill = GetResources()->GetDatasheet<DS_Skill>("Fireball.skill");
        UseSkill(skill, _kCoords, dt);
    }
}

void CharacterHero::UseSkill(DS_Skill* skill, const Vector2f& _kCoords, const DeltaTime& dt)
{
    m_isActingThisFrame = true;

    if (CanUseSkill(skill))
    {
        int nbUse = 1;
        if (skill->useAttackSpeed)
        {
            // If the attack speed allows for several hits in the same frame, we need some additional computations.
            float attackDelay = (m_attackSpeed > 0.f) ? 1.f / m_attackSpeed : 1.f;
            nbUse = Max(1, (int)(dt.s() / attackDelay));

            if (skill->staminaCost > 0.f)
            {
                int nbUseStamina = (int)(m_currentStamina / skill->staminaCost);
                if (nbUseStamina * skill->staminaCost < m_currentStamina - Math::Epsilon)
                {
                    // Allow action when we have at least 1 stamina point (allow negative values).
                    nbUseStamina += 1;
                }

                if (nbUseStamina < nbUse)
                {
                    nbUse = Max(1, nbUseStamina);
                }
            }

            m_attackCooldown = attackDelay * nbUse;
        }
        else
        {
            m_skillCooldown = skill->cooldown;
        }

        if (skill->staminaCost > 0.f)
        {
            m_currentStamina -= skill->staminaCost * nbUse;
            m_staminaRecoveryCooldown = m_staminaRecoveryDelay;
        }

        if (skill->manaCost > 0.f)
        {
            m_currentMana -= skill->manaCost * nbUse;
        }

        for (int i = 0; i < nbUse; ++i)
        {
            // Use skill.
            SkillContext context;
            context.skill = skill;
            context.caster = this;
            context.target = nullptr;
            context.aim = _kCoords;
            context.level = m_level;

            SkillUtility::UseSkill(context);
        }

        Vector2f direction;
        if (_kCoords != GetPosition())
        {
            direction = Normalize(_kCoords - GetPosition());
        }
        else
        {
            direction = Vector2f(1.f, 0.f);
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

        if (strAnim != "" && !m_animation->IsAnimationPlaying(strAnim))
            m_animation->StartAnimation(strAnim);
    }
}

bool CharacterHero::CanUseSkill(DS_Skill* skill) const
{
    if (Character::CanUseSkill(skill))
    {
        // Allow action when we have at least 1 stamina point (allow negative values).
        if (skill->staminaCost > 0.f && m_currentStamina <= 0.f)
        {
            return false;
        }

        if (skill->manaCost > 0.f && m_currentMana < skill->manaCost - Math::Epsilon)
        {
            return false;
        }

        if (skill->useAttackSpeed && m_attackCooldown > 0.f)
        {
            return false;
        }
        else if (skill->cooldown > 0.f && m_skillCooldown > 0.f)
        {
            return false;
        }

        return true;
    }

    return false;
}

void CharacterHero::Step(const DeltaTime& dt)
{
    Character::Step(dt);

    // Stamina recovery
    if (m_staminaRecoveryCooldown > 0.f)
    {
        m_staminaRecoveryCooldown -= dt.s();
    }

    if (m_staminaRecoveryCooldown <= 0.f)
    {
        m_currentStamina += dt.s() * m_staminaRecovery;
        m_currentStamina = Min(m_currentStamina, m_maxStamina);
    }

    // Mana recovery
    m_currentMana += dt.s() * m_manaRecovery;
    m_currentMana = Min(m_currentMana, m_maxMana);

    // Attack cooldown
    if (m_attackCooldown > 0.f)
    {
        m_attackCooldown -= dt.s();
    }

    // Skill cooldown
    if (m_skillCooldown > 0.f)
    {
        m_skillCooldown -= dt.s();
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
