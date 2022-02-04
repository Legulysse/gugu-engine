////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Characters/Character.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Projectiles/Projectile.h"
#include "UI/ElementBar.h"
#include "Scene/Grid.h"

#include "Gugu/Scene/Scene.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Character::Character()
{
    m_sprite = nullptr;
    m_lifeBar = nullptr;

    m_isDead = false;

    m_steppedThisFrame = false;
    m_isMovingThisFrame = false;
    m_isActingThisFrame = false;
}

Character::~Character()
{
    SafeDelete(m_sprite);

    m_lifeBar = nullptr;
    m_grid = nullptr;
}

void Character::Move(Vector2f _kDirection, const DeltaTime& dt)
{
    if (_kDirection == Vector2::Zero_f)
    {
        //if(!m_sprite->IsAnimationPlaying("IdleDown") && m_attackCooldown <= 0.f)  //TODO: param on StartAnimation to check current anim
        //    m_sprite->StartAnimation("IdleDown");

        m_isMovingThisFrame = false;
    }
    else
    {
        _kDirection = Normalize(_kDirection);

        Vector2f position = Character::GetPosition();
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

        if (!m_animation->IsAnimationPlaying(strAnim) && !m_isActingThisFrame)
            m_animation->StartAnimation(strAnim);

        m_isMovingThisFrame = true;
    }
}

void Character::TestCollision(Projectile* _pProjectile)
{
    if (!_pProjectile || m_isDead || _pProjectile->m_isDead)
        return;

    if (LengthSquare(m_sprite->GetPosition() - _pProjectile->m_sprite->GetPosition()) > Power(32.f, 2))
        return;

    _pProjectile->OnHit(this);
}

void Character::ReceiveDamage(Character* source, float value)
{
    if (m_isDead)
    {
        return;
    }

    if (m_lifeBar)
    {
        m_lifeBar->SetVisible(true);
    }

    m_currentLife -= value;
    if (m_currentLife <= 0.f)
    {
        m_isDead = true;
        m_currentLife = 0.f;

        m_sprite->SetVisible(false);

        source->NotifyOpponentKilled(1);
    }
}

void Character::ReceiveHeal(float value)
{
    if (m_isDead)
    {
        return;
    }

    m_currentLife = Clamp(m_currentLife + value, m_currentLife, m_maxLife);
}

void Character::NotifyOpponentKilled(int value)
{
}

bool Character::CanUseSkill(const DS_Skill* skill) const
{
    return true;
}

void Character::Step(const DeltaTime& dt)
{
    SceneActor::Step(dt);

    m_steppedThisFrame = true;
}

void Character::Update(const DeltaTime& dt)
{
    SceneActor::Update(dt);

    // Animation
    if ((m_steppedThisFrame && !m_isMovingThisFrame && !m_isActingThisFrame) || !m_animation->IsAnimationPlaying())
    {
        m_animation->StartAnimation("IdleDown");
    }

    m_steppedThisFrame = false;
    m_isMovingThisFrame = false;
    m_isActingThisFrame = false;

    // UI
    if (m_lifeBar)
    {
        m_lifeBar->SetValue(m_currentLife, m_maxLife);
    }

    m_sprite->SetZIndex((int32)m_sprite->GetPosition().y);
}

Vector2f Character::GetPosition() const
{
    return m_sprite->GetPosition();
}

}   //namespace demoproject
