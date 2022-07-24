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
#include "Scene/Grid.h"

#include "DatasheetBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Debug/Trace.h"

#include <thread>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Game::Game()
{
    m_charactersNode = nullptr;
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
    //PushStateNow(new StateMenuMain);
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

    if (m_charactersNode)
    {
        m_charactersNode->SortOnZIndex();  //TODO: Should this be automatic even for Scene nodes ?
    }
}

void Game::StepScenario(const DeltaTime& dt)
{
#if 1
    {
        GUGU_SCOPE_TRACE_MAIN("Collisions");

        size_t nbProjectiles = m_projectiles.size();
        size_t nbThreads = std::thread::hardware_concurrency();

        if (nbThreads >= 2 && nbProjectiles > nbThreads * 2)
        {
            std::vector<std::thread> threads(nbThreads);

            auto func = [&](const size_t from, const size_t to)
            {
                for (size_t i = from; i < to; ++i)
                {
                    for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
                    {
                        m_controllersAI[iController]->m_character->TestCollision(m_projectiles[i]);
                    }
                }
            };

            size_t nbLoops = nbProjectiles;
            for (size_t t = 0; t < nbThreads; ++t)
            {
                size_t from = t * nbLoops / nbThreads;
                size_t to = (t + 1) == nbThreads ? nbLoops : (t + 1) * nbLoops / nbThreads;
                threads[t] = std::thread(std::bind(func, from, to));
            }
            std::for_each(threads.begin(), threads.end(), [](std::thread& x) {x.join(); });

            for (size_t iProjectile = 0; iProjectile < m_projectiles.size(); ++iProjectile)
            {
                // Purge dead Projectiles
                if (m_projectiles[iProjectile]->m_pendingDestroy)
                {
                    SafeDelete(m_projectiles[iProjectile]);  //TODO: Use a DeleteActor method ?
                }
            }
        }
        else
        {
            // Handle basic collisions for projectiles
            for (size_t iProjectile = 0; iProjectile < m_projectiles.size(); ++iProjectile)
            {
                for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
                {
                    m_controllersAI[iController]->m_character->TestCollision(m_projectiles[iProjectile]);
                }

                // Purge dead Projectiles
                if (m_projectiles[iProjectile]->m_pendingDestroy)
                {
                    SafeDelete(m_projectiles[iProjectile]);  //TODO: Use a DeleteActor method ?
                }
            }
        }
    }
#else
    {
        GUGU_SCOPE_TRACE_MAIN("Collisions");

        // Handle basic collisions for projectiles
        for (size_t iProjectile = 0; iProjectile < m_projectiles.size(); ++iProjectile)
        {
            for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
            {
                m_controllersAI[iController]->m_character->TestCollision(m_projectiles[iProjectile]);
            }

            // Purge dead Projectiles
            if (m_projectiles[iProjectile]->m_pendingDestroy)
            {
                SafeDelete(m_projectiles[iProjectile]);  //TODO: Use a DeleteActor method ?
            }
        }
    }
#endif
    {
        GUGU_SCOPE_TRACE_MAIN("Dead Actors");

        // Purge dead Actors
        for (size_t iController = 0; iController < m_controllersAI.size(); ++iController)
        {
            if (m_controllersAI[iController]->m_character->m_isDead)
            {
                SafeDelete(m_controllersAI[iController]->m_character);   //TODO: Use a DeleteActor method ?
                SafeDelete(m_controllersAI[iController]);     //TODO: Use a DeleteActor method ?
            }
        }
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Clear");

        StdVectorRemove<Projectile*>(m_projectiles, nullptr);
        StdVectorRemove<ControllerAI*>(m_controllersAI, nullptr);
    }

    // Reset Floor
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
}

bool Game::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputEventPressed("Screenshot", _oSFEvent))
    {
        GetGameWindow()->Screenshot();
    }

    return true;
}

void Game::CreateScenario()
{
    // Init Scene and Camera
    Camera* pCamera = GetGameWindow()->CreateCamera();
    pCamera->SetCenterOnTarget(true);
    
    m_scene = GetScenes()->GetRootScene()->AddChildScene();
    GetGameWindow()->BindScene(m_scene, pCamera);

    // Fill Scene
    m_grid = new Grid();
    m_grid->InitGrid(m_scene, 50, 50, 32.f, 32.f);

    m_charactersNode = m_scene->GetRootNode()->AddChild<Element>();

    // Init Player
    const DS_Hero* sheetHero = GetResources()->GetDatasheetObject<DS_Hero>("Hero.hero");

    m_character = new CharacterHero;
    m_scene->AddActor(m_character);

    m_character->InitHero(sheetHero, 600.f, m_grid, m_charactersNode);

    m_controllerPlayer = new ControllerPlayer;
    m_scene->AddActor(m_controllerPlayer);

    m_controllerPlayer->InitControllerPlayer(m_character);

    // Spawn first floor
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
    const DS_Enemy* sheetEnemy = GetResources()->GetDatasheetObject<DS_Enemy>("DefaultEnemy.enemy");

    size_t enemiesCount = m_floor * 50;
    for (size_t i = 0; i < enemiesCount; ++i)
    {
        ControllerAI* pControllerAI = new ControllerAI;
        m_scene->AddActor(pControllerAI);

        m_controllersAI.push_back(pControllerAI);

        CharacterEnemy* pEnemy = new CharacterEnemy;
        m_scene->AddActor(pEnemy);

        sf::Vector2f position = GetRandomPointInCircle(Vector2::Zero_f, 150.f);

        pEnemy->InitEnemy(sheetEnemy, position, 100.f, m_grid, m_charactersNode);
        pControllerAI->m_character = pEnemy;
    }
}

void Game::ClearScenario()
{
    // Reset references to Scene Actors
    m_projectiles.clear();
    m_controllersAI.clear();
    m_character = nullptr;
    m_controllerPlayer = nullptr;

    m_charactersNode = nullptr;

    SafeDelete(m_grid);

    //Delete the Scenes and their Actors
    GetScenes()->ResetRootScene();

    GetGameWindow()->DeleteAllCameras();
}

void Game::GetStatus(int& floor, int& enemies) const
{
    floor = m_floor;
    enemies = m_controllersAI.size();
}

void Game::GetCharactersInRange(std::vector<Character*>& characters, const Vector2f& center, float radius) const
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
