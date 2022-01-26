#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Misc/Pattern/StateMachine.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
    class ControllerPlayer;
}

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Demo : public gugu::Application, public gugu::StateMachine, public gugu::EventListener
{
public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStep(const gugu::DeltaTime& dt) override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;
    
    void CreateScenario();
    void ClearScenario();

private:

    gugu::Element* m_menu;

    Character* m_characterA;
    ControllerPlayer* m_controllerA;

    Character* m_characterB;
    ControllerPlayer* m_controllerB;
};

Demo* GetGame();

}   //namespace demoproject
