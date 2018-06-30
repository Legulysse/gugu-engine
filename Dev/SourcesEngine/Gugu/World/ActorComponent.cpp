////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/ActorComponent.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ActorComponent::ActorComponent()
: m_actor(nullptr)
{
}

ActorComponent::~ActorComponent()
{
    Release();
}

void ActorComponent::Init()
{
}

void ActorComponent::Release()
{
    //Inform the Actor if released from gameplay (If released from Actor, this reference is already null).
    if (m_actor)
    {
        m_actor->OnComponentReleased(this);
        m_actor = nullptr;
    }
}

Actor* ActorComponent::GetActor() const
{
    return m_actor;
}

void ActorComponent::Step(const DeltaTime& dt)
{
}

void ActorComponent::Update(const DeltaTime& dt)
{
}

}   // namespace gugu
