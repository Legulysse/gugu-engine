////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Animation/ManagerAnimations.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAnimations::ManagerAnimations()
{
}

ManagerAnimations::~ManagerAnimations()
{
}

void ManagerAnimations::Init(const EngineConfig& config)
{
}

void ManagerAnimations::Release()
{
    DeleteAllAnimations();
}

void ManagerAnimations::Step(const DeltaTime& dt)
{
    // TODO: handle step vs update animations.
    for (SpriteAnimation* animation : m_spriteAnimations)
    {
        animation->StepAnimation(dt);
    }
}

void ManagerAnimations::Update(const DeltaTime& dt)
{
    // TODO: handle step vs update animations.
    //for (SpriteAnimation* animation : m_spriteAnimations)
    //{
    //    animation->StepAnimation(dt);
    //}
}

SpriteAnimation* ManagerAnimations::AddAnimation(ElementSprite* sprite)
{
    if (sprite == nullptr)
        return nullptr;

    SpriteAnimation* animation = new SpriteAnimation;
    animation->SetSprite(sprite);

    sprite->InitInteractions();
    sprite->GetInteractions()->AddCallback(EInteraction::Destroyed, [sprite]() { GetAnimations()->DeleteAnimation(sprite); });

    m_spriteAnimations.push_back(animation);
    return animation;
}

void ManagerAnimations::RemoveAnimation(SpriteAnimation* animation)
{
    StdVectorRemove(m_spriteAnimations, animation);
}

void ManagerAnimations::DeleteAnimation(SpriteAnimation* animation)
{
    // Remove will be called in the animation destructor.
    SafeDelete(animation);
}

void ManagerAnimations::DeleteAnimation(ElementSprite* sprite)
{
    if (sprite == nullptr)
        return;

    size_t i = 0;
    while (i < m_spriteAnimations.size())
    {
        if (m_spriteAnimations[i]->GetSprite() == sprite)
        {
            SpriteAnimation* animation = m_spriteAnimations[i];
            StdVectorRemoveAt(m_spriteAnimations, i);
            SafeDelete(animation);
        }
        else
        {
            ++i;
        }
    }
}

void ManagerAnimations::DeleteAllAnimations()
{
    //TODO: I should use some flag to tell if an animation is being destroyed, and avoid going back and forth between the manager and the destructors.
    for (size_t i = 0; i < m_spriteAnimations.size(); ++i)
    {
        SpriteAnimation* animation = m_spriteAnimations[i];
        m_spriteAnimations[i] = nullptr;  //Set to null before calling the delete, to avoid the child to call a remove

        SafeDelete(animation);
    }

    m_spriteAnimations.clear();
}

ManagerAnimations* GetAnimations()
{
    return GetEngine()->GetManagerAnimations();
}

}   // namespace gugu
