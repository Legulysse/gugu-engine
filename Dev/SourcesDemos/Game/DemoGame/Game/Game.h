#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Misc/Pattern/StateMachine.h"
#include "Gugu/Math/Vector2.h"

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
    class Scene;
    class Element;
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
    
    // TODO: split into a dedicated Scenario class.
    void CreateScenario();
    void ClearScenario();
    void StepScenario(const gugu::DeltaTime& dt);

    void SpawnNextFloor();
    void SpawnFloor();

    void GetStatus(int& floor, int& enemies) const;

    void GetCharactersInRange(std::vector<class Character*>& characters, const gugu::Vector2f& center, float radius) const;

public:

    gugu::Scene* m_scene;
    gugu::Element* m_charactersNode;

    CharacterHero* m_character;
    ControllerPlayer* m_controllerPlayer;
    std::vector<ControllerAI*> m_controllersAI;

    std::vector<Projectile*> m_projectiles;

    Grid* m_grid;

    int m_floor;
    float m_delayReset;
};

Game* GetGame();

}   //namespace demoproject
