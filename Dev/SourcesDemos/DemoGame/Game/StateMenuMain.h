#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Patterns/State.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class StateMenuMain : public gugu::State, public gugu::EventListener
{
public:

    StateMenuMain();
    virtual ~StateMenuMain();

    virtual void Init() override;
    virtual void Release() override;

    virtual void Enter(State* _pStateFrom) override;
    virtual void Exit(State* _pStateTo) override;

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

    void OnButtonClick(int _iButtonID);

private:

    gugu::Element* m_menu;
};

}   //namespace demoproject
