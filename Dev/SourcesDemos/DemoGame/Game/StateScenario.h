#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/State.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class StateScenario : public gugu::State, public gugu::EventListener
{
public:

    StateScenario();
    virtual ~StateScenario();

    virtual void Init() override;
    virtual void Release() override;

    virtual void Enter(State* _pStateFrom) override;
    virtual void Exit(State* _pStateTo) override;

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

private:

    gugu::Element* m_menu;

    class ElementBar* m_staminaBar;
};

}   //namespace demoproject
