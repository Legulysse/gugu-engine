#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"
#include "Gugu/Utility/StateMachine.h"
#include "Gugu/Utility/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class CharacterHero;
    class ControllerPlayer;
    class ControllerAI;
    class Projectile;
    class Grid;
}

namespace gugu
{
    class Level;
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
    void StepScenario(const gugu::DeltaTime& dt);

    void SpawnNextFloor();
    void SpawnFloor();

    void GetStatus(int& floor, int& enemies) const;

    void GetCharactersInRange(std::vector<class Character*>& characters, const sf::Vector2f& center, float radius) const;

public:

    gugu::Level* m_level;

    CharacterHero* m_character;
    ControllerPlayer* m_controllerPlayer;
    std::vector<ControllerAI*> m_controllersAI;

    std::vector<Projectile*> m_projectiles;

    Grid* m_grid;

    int m_floor;
    int m_delayReset;
};

Game* GetGame();

}   //namespace demoproject
