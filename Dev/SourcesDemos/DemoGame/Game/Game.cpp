////////////////////////////////////////////////////////////////
// Header

#include "Game/Game.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Controllers/ControllerPlayer.h"
#include "Actors/Controllers/ControllerAI.h"
#include "Actors/Characters/Character.h"
#include "Actors/Projectiles/Projectile.h"
#include "Level/Grid.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/World/World.h"
#include "Gugu/World/Level.h"
#include "Gugu/World/Grid/SquareGrid.h"
#include "Gugu/World/Grid/GridCell.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Utility/System.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Game::Game()
{
    m_character = nullptr;
    m_controllerPlayer = nullptr;
}

Game::~Game()
{
}

void Game::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    //Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");

    ManagerConfig* pConfig = GetConfig();

    //Method 1 : use a config file
    pConfig->LoadInputFile("Bindings.xml");
    
    //Method 2 : register keys manually
    /*pConfig->RegisterInput("Player_1_Up",     pConfig->BuildEventKey(sf::Keyboard::Z));
    pConfig->RegisterInput("Player_1_Down",     pConfig->BuildEventKey(sf::Keyboard::S));
    pConfig->RegisterInput("Player_1_Left",     pConfig->BuildEventKey(sf::Keyboard::Q));
    pConfig->RegisterInput("Player_1_Right",    pConfig->BuildEventKey(sf::Keyboard::D));

    pConfig->RegisterInput("Player_2_Up",       pConfig->BuildEventKey(sf::Keyboard::Up));
    pConfig->RegisterInput("Player_2_Down",     pConfig->BuildEventKey(sf::Keyboard::Down));
    pConfig->RegisterInput("Player_2_Left",     pConfig->BuildEventKey(sf::Keyboard::Left));
    pConfig->RegisterInput("Player_2_Right", pConfig->BuildEventKey(sf::Keyboard::Right));
    */

    pConfig->RegisterInput("Player_1_Up", pConfig->BuildKeyboardEvent(sf::Keyboard::Z));
    pConfig->RegisterInput("Player_1_Down", pConfig->BuildKeyboardEvent(sf::Keyboard::S));
    pConfig->RegisterInput("Player_1_Left", pConfig->BuildKeyboardEvent(sf::Keyboard::Q));
    pConfig->RegisterInput("Player_1_Right", pConfig->BuildKeyboardEvent(sf::Keyboard::D));

    pConfig->RegisterInput("Exit", pConfig->BuildKeyboardEvent(sf::Keyboard::Escape));
    pConfig->RegisterInput("Screenshot", pConfig->BuildKeyboardEvent(sf::Keyboard::F10));
}

void Game::AppStop()
{
}

void Game::AppStep(const DeltaTime& dt)
{
    StateMachine::Step(dt);

    // Handle basic collisions for projectiles
    for (size_t iProjectile = 0; iProjectile < m_projectiles.size(); ++iProjectile)
    {
        for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
        {
            if (m_controllersAI[iController]->m_character->m_isDead)
                continue;

            if (m_controllersAI[iController]->m_character->TestCollision(m_projectiles[iProjectile]))
            {
                SafeDelete(m_projectiles[iProjectile]);  //TODO: Use a DeleteActor method ?
            }
        }
    }
    StdVectorRemove<Projectile*>(m_projectiles, nullptr);

    // Purge dead Actors
    for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
    {
        if (m_controllersAI[iController]->m_character->m_isDead)
        {
            SafeDelete(m_controllersAI[iController]->m_character);   //TODO: Use a DeleteActor method ?
            SafeDelete(m_controllersAI[iController]);     //TODO: Use a DeleteActor method ?
        }
    }
    StdVectorRemove<ControllerAI*>(m_controllersAI, nullptr);
}

void Game::AppUpdate(const DeltaTime& dt)
{
    StateMachine::Update(dt);
}

bool Game::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerConfig* pConfig = GetConfig();
    if (pConfig->IsInputPressed("Screenshot", _oSFEvent))
    {
        GetGameWindow()->Screenshot();
    }

    return true;
}

void Game::CreateScenario()
{
    //Init Level and Camera
    Camera* pCamera = GetGameWindow()->CreateCamera();
    pCamera->SetCenterOnTarget(true);
    
    Level* pLevel = GetWorld()->GetMainLevel()->CreateSubLevel();

    GetGameWindow()->BindLevel(pLevel, pCamera);
    
    //Fill Level
    //ElementSprite* pGround = pLevel->GetLayer(0)->GetRoot()->AddChild<ElementSprite>();
    //pGround->SetImage("BraidBackground.jpg");

    m_grid = new Grid();
    m_grid->InitGrid(pLevel, 50, 50, 32.f, 32.f);

    //Init Player
    m_character = new Character;
    pLevel->AddActor(m_character);

    m_character->InitCharacter(true, 1000.f);

    m_controllerPlayer = new ControllerPlayer;
    pLevel->AddActor(m_controllerPlayer);

    m_controllerPlayer->InitControllerPlayer(m_character);

    //Init Enemies
    for (size_t i = 0; i < 500; ++i)
    {
        ControllerAI* pControllerAI = new ControllerAI;
        pLevel->AddActor(pControllerAI);

        m_controllersAI.push_back(pControllerAI);

        Character* pEnemy = new Character;
        pLevel->AddActor(pEnemy);

        pEnemy->InitCharacter(false, 100.f);
        pControllerAI->m_character = pEnemy;
    }

    pLevel->GetRootNode()->SortOnZIndex();  //TODO: Should this be automatic even for Level nodes ?
}

void Game::ClearScenario()
{
    // Reset references to Level Actors
    m_projectiles.clear();
    m_controllersAI.clear();
    m_character = nullptr;
    m_controllerPlayer = nullptr;

    SafeDelete(m_grid);

    //Delete the Levels and their Actors
    GetWorld()->ResetWorld();

    GetGameWindow()->DeleteAllCameras();
}


Game* GetGame()
{
    return (Game*)GetEngine()->GetApplication();
}

}   //namespace demoproject
