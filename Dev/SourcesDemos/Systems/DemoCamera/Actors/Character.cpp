////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Character.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/Scene.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Character::Character()
{
    m_sprite = nullptr;
}

Character::~Character()
{
}

void Character::InitCharacter(int index)
{
    m_sprite = m_scene->GetRootNode()->AddChild<ElementSprite>();
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    m_spriteAnimation = GetAnimations()->AddAnimation(m_sprite);

    if (index == 0)
        m_spriteAnimation->ChangeAnimSet("Human.animset.xml");
    else
        m_spriteAnimation->ChangeAnimSet("Lady.animset.xml");

    m_spriteAnimation->StartAnimation("WalkDown");
}

void Character::Move(Vector2f _kDirection, const DeltaTime& dt)
{
    _kDirection = Normalize(_kDirection);
    m_sprite->Move(_kDirection * dt.s() * 200.f);
}

Vector2f Character::GetPosition() const
{
    return m_sprite->GetPosition();
}

}   //namespace demoproject
