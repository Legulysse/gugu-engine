#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ActorComponent;
    class Level;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Actor
{
    friend class Level;
    friend class ActorComponent;

public:

    Actor();
    virtual ~Actor();

    virtual void Step(const DeltaTime& dt);
    virtual void Update(const DeltaTime& dt);

    //Components
    ActorComponent* AddComponent(ActorComponent* _pComponent);
    void RemoveComponent(ActorComponent* _pComponent);
    void DeleteComponent(ActorComponent* _pComponent);

    bool HasComponent(ActorComponent* _pComponent) const;

protected:

    virtual void OnComponentAdded(ActorComponent* _pComponent) {}
    virtual void OnComponentRemoved(ActorComponent* _pComponent) {}

    void OnComponentReleased(ActorComponent* _pComponent);   //Called by an ActorComponent just released, to ensure the Actor is not referencing it.

protected:

    Level* m_level;

    std::vector<ActorComponent*> m_components;
};

}   // namespace gugu
