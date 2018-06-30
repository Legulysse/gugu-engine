////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Actor.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/ActorComponent.h"
#include "Gugu/World/Level.h"
#include "Gugu/Utility/System.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Actor::Actor()
: m_level(nullptr)
{
}

Actor::~Actor()
{
    //Inform the Level if released from gameplay (If released from Level, this reference is already null).
    if (m_level)
    {
        m_level->OnActorReleased(this);
        m_level = nullptr;
    }

    while (!m_components.empty())
    {
        DeleteComponent(m_components[0]);
    }
}

void Actor::Step(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        m_components[i]->Step(dt);
    }
}

void Actor::Update(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        m_components[i]->Update(dt);
    }
}

ActorComponent* Actor::AddComponent(ActorComponent* _pComponent)
{
    if (!_pComponent)
        return nullptr;

    if (!StdVectorContains(m_components, _pComponent))
    {
        m_components.push_back(_pComponent);

        _pComponent->m_actor = this;

        OnComponentAdded(_pComponent);
        _pComponent->OnAddedToActor();
        
        for (size_t i = 0; i < m_components.size(); ++i)
        {
            if (m_components[i] != _pComponent)
                m_components[i]->OnComponentAdded(_pComponent);
        }

        return _pComponent;
    }

    return nullptr;
}

void Actor::RemoveComponent(ActorComponent* _pComponent)
{
    if (!_pComponent)
        return;

    auto iteComponent = StdVectorFind(m_components, _pComponent);
    if (iteComponent != m_components.end())
    {
        for (size_t i = 0; i < m_components.size(); ++i)
        {
            if (m_components[i] != _pComponent)
                m_components[i]->OnComponentRemoved(_pComponent);
        }

        _pComponent->OnRemovedFromActor();
        OnComponentRemoved(_pComponent);

        _pComponent->m_actor = nullptr;

        m_components.erase(iteComponent);
    }
}

void Actor::DeleteComponent(ActorComponent* _pComponent)
{
    if (!_pComponent)
        return;

    RemoveComponent(_pComponent);
    SafeDelete(_pComponent);
}

bool Actor::HasComponent(ActorComponent* _pComponent) const
{
    if (!_pComponent)
        return false;

    return StdVectorContains(m_components, _pComponent);
}

void Actor::OnComponentReleased(ActorComponent* _pComponent)
{
    if (!_pComponent)
        return;

    auto iteComponent = StdVectorFind(m_components, _pComponent);
    if (iteComponent != m_components.end())
    {
        //ActorComponent has already been released, no need to process anything on it.
        m_components.erase(iteComponent);
    }
}

}   // namespace gugu
