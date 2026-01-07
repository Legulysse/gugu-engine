////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Misc/Pattern/State.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
State::State()
    : m_owner(nullptr)
{
}

State::~State()
{
}

void State::Init()
{
}

void State::Release()
{
}

void State::Enter(State* _pStateFrom)
{
}

void State::Exit(State* _pStateTo)
{
}

void State::Step(const DeltaTime& dt)
{
}

void State::Update(const DeltaTime& dt)
{
}

void State::LateUpdate(const DeltaTime& dt)
{
}

void State::SetOwner(StateMachine* _pOwner)
{
    m_owner = _pOwner;
}

StateMachine* State::GetOwner() const
{
    return m_owner;
}

bool State::IsTopState() const
{
    return (m_owner->GetState() == this);
}

}   // namespace gugu
