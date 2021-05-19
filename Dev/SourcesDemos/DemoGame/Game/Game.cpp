////////////////////////////////////////////////////////////////
// Header

#include "Game/Game.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/StateMenuMain.h"
#include "Game/StateScenario.h"
#include "Actors/Controllers/ControllerPlayer.h"
#include "Actors/Controllers/ControllerAI.h"
#include "Actors/Characters/Character.h"
#include "Actors/Characters/CharacterHero.h"
#include "Actors/Characters/CharacterEnemy.h"
#include "Actors/Projectiles/Projectile.h"
#include "Level/Grid.h"

#include "DatasheetBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/World/World.h"
#include "Gugu/World/Level.h"
#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Game::Game()
{
    m_character = nullptr;
    m_controllerPlayer = nullptr;

    m_floor = 0;
    m_delayReset = 0;
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

    ManagerInputs* inputs = GetInputs();

    //Method 1 : use a config file
    inputs->LoadInputFile("Bindings.xml");
    
    //Method 2 : register keys manually
    inputs->RegisterInput("Player_1_Up", inputs->BuildKeyboardEvent(sf::Keyboard::Z));
    inputs->RegisterInput("Player_1_Down", inputs->BuildKeyboardEvent(sf::Keyboard::S));
    inputs->RegisterInput("Player_1_Left", inputs->BuildKeyboardEvent(sf::Keyboard::Q));
    inputs->RegisterInput("Player_1_Right", inputs->BuildKeyboardEvent(sf::Keyboard::D));
    inputs->RegisterInput("Player_1_Skill_1", inputs->BuildKeyboardEvent(sf::Keyboard::Num1));
    inputs->RegisterInput("Player_1_Skill_2", inputs->BuildKeyboardEvent(sf::Keyboard::Num2));
    inputs->RegisterInput("Player_1_Skill_3", inputs->BuildKeyboardEvent(sf::Keyboard::Num3));

    inputs->RegisterInput("Exit", inputs->BuildKeyboardEvent(sf::Keyboard::Escape));
    inputs->RegisterInput("Screenshot", inputs->BuildKeyboardEvent(sf::Keyboard::F10));

    inputs->RegisterInput("ToggleCharacterSheet", inputs->BuildKeyboardEvent(sf::Keyboard::I));

    // Push the entry-point State.
    PushStateNow(new StateScenario);
}

void Game::AppStop()
{
}

void Game::AppStep(const DeltaTime& dt)
{
    StateMachine::Step(dt);
}

void Game::AppUpdate(const DeltaTime& dt)
{
    StateMachine::Update(dt);
}

void Game::StepScenario(const DeltaTime& dt)
{
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

    // Reset level
    if (m_delayReset > 0)
    {
        m_delayReset -= dt.ms();
        if (m_delayReset <= 0)
        {
            m_delayReset = 0;

            SpawnNextFloor();
        }
    }
    else if (m_controllersAI.empty())
    {
        m_delayReset = 2000;
    }

    //m_level->GetRootNode()->SortOnZIndex();  //TODO: Should this be automatic even for Level nodes ?
}

bool Game::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputPressed("Screenshot", _oSFEvent))
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
    
    m_level = GetWorld()->GetMainLevel()->CreateSubLevel();
    GetGameWindow()->BindLevel(m_level, pCamera);

    //Disable interactions on level nodes (optimisation)
    m_level->GetRootNode()->AddInteractionFlag(EInteraction::Absorb);
    
    //Fill Level
    //ElementSprite* pGround = pLevel->GetLayer(0)->GetRoot()->AddChild<ElementSprite>();
    //pGround->SetImage("BraidBackground.jpg");

    m_grid = new Grid();
    m_grid->InitGrid(m_level, 50, 50, 32.f, 32.f);

    //Init Player
    DS_Hero* sheetHero = GetResources()->GetDatasheet<DS_Hero>("Hero.hero");

    m_character = new CharacterHero;
    m_level->AddActor(m_character);

    m_character->InitHero(sheetHero, 600.f, m_grid);

    m_controllerPlayer = new ControllerPlayer;
    m_level->AddActor(m_controllerPlayer);

    m_controllerPlayer->InitControllerPlayer(m_character);

    // Spawn first level
    m_floor = 1;

    SpawnFloor();
}

void Game::SpawnNextFloor()
{
    m_floor += 1;

    SpawnFloor();
}

void Game::SpawnFloor()
{
    //Init Enemies
    size_t enemiesCount = m_floor * 50;
    for (size_t i = 0; i < enemiesCount; ++i)
    {
        DS_Enemy* sheetEnemy = GetResources()->GetDatasheet<DS_Enemy>("DefaultEnemy.enemy");

        ControllerAI* pControllerAI = new ControllerAI;
        m_level->AddActor(pControllerAI);

        m_controllersAI.push_back(pControllerAI);

        CharacterEnemy* pEnemy = new CharacterEnemy;
        m_level->AddActor(pEnemy);

        pEnemy->InitEnemy(sheetEnemy, 100.f, m_grid);
        pControllerAI->m_character = pEnemy;
    }

    m_level->GetRootNode()->SortOnZIndex();  //TODO: Should this be automatic even for Level nodes ?
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

void Game::GetStatus(int& floor, int& enemies) const
{
    floor = m_floor;
    enemies = m_controllersAI.size();
}

void Game::GetCharactersInRange(std::vector<Character*>& characters, const sf::Vector2f& center, float radius) const
{
    for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
    {
        if (LengthSquare(m_controllersAI[iController]->m_character->GetPosition() - center) <= Power(radius, 2))
        {
            characters.push_back(m_controllersAI[iController]->m_character);
        }
    }
}


Game* GetGame()
{
    return (Game*)GetEngine()->GetApplication();
}

}   //namespace demoproject
