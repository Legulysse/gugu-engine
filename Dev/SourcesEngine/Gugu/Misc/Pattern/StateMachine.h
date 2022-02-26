#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <stack>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class State;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class StateMachine
{
private:

    enum EStateAction {
        None,
        Push,
        Change,
        Pop,
        PopAll,
    };

public:

    StateMachine();
    virtual ~StateMachine();

    template<typename T>
    T* PushState()
    {
        T* pNewState = new T;
        PushState(pNewState);
        return pNewState;
    }

    template<typename T>
    T* ChangeState()
    {
        T* pNewState = new T;
        ChangeState(pNewState);
        return pNewState;
    }

    template<typename T>
    T* PushStateNow()
    {
        T* pNewState = new T;
        PushStateNow(pNewState);
        return pNewState;
    }

    template<typename T>
    T* ChangeStateNow()
    {
        T* pNewState = new T;
        ChangeStateNow(pNewState);
        return pNewState;
    }

    void    PushState       (State* _pNextState);
    void    ChangeState     (State* _pNextState);
    void    PopState        ();
    void    PopAllStates    ();

    void    PushStateNow    (State* _pNextState);
    void    ChangeStateNow  (State* _pNextState);
    void    PopStateNow     ();
    void    PopAllStatesNow ();

    void    ProcessNextState();

    virtual void    Step    (const DeltaTime& dt);
    virtual void    Update  (const DeltaTime& dt);

    State*  GetState() const;

private:

    void PushStateImpl      (State* pNewState);
    void ChangeStateImpl    (State* pNewState);
    void PopStateImpl       ();
    void PopAllStatesImpl   ();

private:

    std::stack<State*> m_states;

    EStateAction m_nextStateAction;
    State* m_nextState;
};

}   // namespace gugu
