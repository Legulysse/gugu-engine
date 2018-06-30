#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"
#include "Gugu/Utility/StateMachine.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
    class ControllerPlayer;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Game : public gugu::Application, public gugu::StateMachine, public gugu::EventListener
{
public:

	Game();
	virtual ~Game();

    virtual	void AppStart() override;
    virtual	void AppStop() override;

    virtual void AppStep(const gugu::DeltaTime& dt) override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;
    
    void CreateScenario();
    void ClearScenario();

public:

    Character* m_characterA;
    ControllerPlayer* m_controllerA;

    Character* m_characterB;
    ControllerPlayer* m_controllerB;
};

Game* GetGame();

}   //namespace demoproject
