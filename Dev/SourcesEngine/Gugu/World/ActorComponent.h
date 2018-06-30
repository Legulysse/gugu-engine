#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/DeltaTime.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Actor;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ActorComponent
{
    friend class Actor;

public:

	ActorComponent();
	virtual ~ActorComponent();

	virtual void Init		();
	virtual void Release	();

    Actor* GetActor() const;

    //Events called from Actor
	virtual void Step		(const DeltaTime& dt);
	virtual void Update		(const DeltaTime& dt);

    virtual void OnAddedToActor         () {}
    virtual void OnRemovedFromActor     () {}
    
    virtual void OnComponentAdded       (ActorComponent* _pComponent) {}
    virtual void OnComponentRemoved     (ActorComponent* _pComponent) {}

private:

    Actor* m_actor;
};

}   // namespace gugu
