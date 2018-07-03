////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Character.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Level.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Utility/Math.h"

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
    m_sprite = m_level->GetRootNode()->AddChild<ElementSpriteAnimated>();

    if (index == 0)
        m_sprite->ChangeAnimSet("Human.animset.xml");
    else
        m_sprite->ChangeAnimSet("Lady.animset.xml");

    m_sprite->StartAnimation("WalkDown");
    m_sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
}

void Character::Move(sf::Vector2f _kDirection, const DeltaTime& dt)
{
    _kDirection = Normalize(_kDirection);
    m_sprite->Move(_kDirection * dt.s() * 200.f);
}

sf::Vector2f Character::GetPosition() const
{
    return m_sprite->GetPosition();
}

}   //namespace demoproject
