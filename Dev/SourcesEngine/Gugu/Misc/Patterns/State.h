#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/StateMachine.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class State
{
public:

    State();
    virtual ~State();

    virtual void Init();
    virtual void Release();

    virtual void Enter  (State* _pStateFrom);
    virtual void Exit   (State* _pStateTo);

    virtual void Step(const DeltaTime& dt);
    virtual void Update(const DeltaTime& dt);

    void            SetOwner(StateMachine* _pOwner);
    StateMachine*   GetOwner() const;

    bool            IsTopState() const;

private:

    StateMachine* m_owner;
};

}   // namespace gugu
