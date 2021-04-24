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
    class ControllerAI;
    class Projectile;
    class Grid;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Game : public gugu::Application, public gugu::StateMachine, public gugu::EventListener
{
public:

    Game();
    virtual ~Game();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppStep(const gugu::DeltaTime& dt) override;
    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;
    
    void CreateScenario();
    void ClearScenario();

    void GetStatus(int& level, int& enemies) const;

public:

    Character* m_character;
    ControllerPlayer* m_controllerPlayer;
    std::vector<ControllerAI*> m_controllersAI;

    std::vector<Projectile*> m_projectiles;

    Grid* m_grid;
};

Game* GetGame();

}   //namespace demoproject
