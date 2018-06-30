////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Utility/StateMachine.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/System.h"
#include "Gugu/Utility/State.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

StateMachine::StateMachine()
{
	m_nextStateAction = None;
    m_nextState = nullptr;
}

StateMachine::~StateMachine()
{
    PopAllStatesNow();
}

void StateMachine::PushState(State* _pNextState)
{
    SafeDelete(m_nextState);

    m_nextState = _pNextState;
    m_nextStateAction = Push;
}

void StateMachine::ChangeState(State* _pNextState)
{
    SafeDelete(m_nextState);

    m_nextState = _pNextState;
    m_nextStateAction = Change;
}

void StateMachine::PopState()
{
    SafeDelete(m_nextState);

    m_nextStateAction = Pop;
}

void StateMachine::PopAllStates()
{
    SafeDelete(m_nextState);

    m_nextStateAction = PopAll;
}

void StateMachine::PushStateNow(State* _pNextState)
{
    SafeDelete(m_nextState);
    m_nextStateAction = None;

	PushStateImpl(_pNextState);
}

void StateMachine::ChangeStateNow(State* _pNextState)
{
    SafeDelete(m_nextState);
    m_nextStateAction = None;

	ChangeStateImpl(_pNextState);
}

void StateMachine::PopStateNow()
{
    SafeDelete(m_nextState);
    m_nextStateAction = None;

	PopStateImpl();
}

void StateMachine::PopAllStatesNow()
{
    SafeDelete(m_nextState);
    m_nextStateAction = None;

    PopAllStatesImpl();
}

void StateMachine::PushStateImpl(State* pNewState)
{
	//Init new state
	pNewState->SetOwner(this);
	pNewState->Init();

	//Exit actual state
    if(!m_states.empty())
    {
		GetState()->Exit( pNewState );
    }

	//Enter new state
    pNewState->Enter( GetState() );
    m_states.push( pNewState );
}

void StateMachine::ChangeStateImpl(State* pNewState)
{
	if(m_states.empty())
	{
		PushStateImpl( pNewState );
	}
	else
	{
		//Init new state
		pNewState->SetOwner(this);
		pNewState->Init();

		//Exit actual state
		State* pOldState = GetState();
		m_states.pop();
		pOldState->Exit( pNewState );

		//Enter new state
		pNewState->Enter( pOldState );
		m_states.push( pNewState );

		//Release old state
		pOldState->Release();
		SafeDelete(pOldState);
	}
}

void StateMachine::PopStateImpl()
{
    if(!m_states.empty())
    {
		//Exit actual state
		State* pOldState = GetState();
        m_states.pop();
		pOldState->Exit( GetState() );

		//Enter new state if any
		if(!m_states.empty())
		{
			GetState()->Enter( pOldState );
		}

		//Release old state
		pOldState->Release();
		SafeDelete(pOldState);
    }
}

void StateMachine::PopAllStatesImpl()
{
    while(!m_states.empty())
    {
        PopStateImpl();
    }
}

State* StateMachine::GetState() const
{
	if( !m_states.empty() )
	{
	    return m_states.top();
	}

	return nullptr;
}

void StateMachine::ProcessNextState()
{
    if(m_nextStateAction != None)
    {
        if(m_nextStateAction == Push)
        {
            PushStateImpl(m_nextState);
        }
        else if(m_nextStateAction == Change)
        {
            ChangeStateImpl(m_nextState);
        }
        else if(m_nextStateAction == Pop)
        {
            PopStateImpl();
        }
        else if(m_nextStateAction == PopAll)
        {
            PopAllStatesImpl();
        }

		m_nextStateAction = None;
        m_nextState = nullptr;
    }
}

void StateMachine::Step(const DeltaTime& dt)
{
	//Process eventual State update asked by a PushState, ChangeState or PopState
	ProcessNextState(); //TODO: remove this, let the user decide

	//Process State step
	if (GetState())
        GetState()->Step(dt);

	//Process eventual State update asked by a PushState, ChangeState or PopState
	ProcessNextState(); //TODO: remove this, let the user decide
}

void StateMachine::Update(const DeltaTime& dt)
{
    //Skip Update if we are waiting a step to process a state change
    if(m_nextStateAction == None)
    {
	    //Process State update
	    if (GetState())
            GetState()->Update(dt);
    }
}

}   // namespace gugu
