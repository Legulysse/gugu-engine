////////////////////////////////////////////////////////////////
// Header

#include "Game/Game.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/ControllerPlayer.h"
#include "Actors/Character.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementTile.h"
#include "Gugu/World/World.h"
#include "Gugu/World/Level.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Utility/System.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Game::Game()
{
    m_characterA = nullptr;
    m_controllerA = nullptr;

    m_characterB = nullptr;
    m_controllerB = nullptr;
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
    pConfig->RegisterInput("Player_1_Up",       pConfig->BuildEventKey(sf::Keyboard::Z));
    pConfig->RegisterInput("Player_1_Down",     pConfig->BuildEventKey(sf::Keyboard::S));
    pConfig->RegisterInput("Player_1_Left",     pConfig->BuildEventKey(sf::Keyboard::Q));
    pConfig->RegisterInput("Player_1_Right",    pConfig->BuildEventKey(sf::Keyboard::D));

    pConfig->RegisterInput("Player_2_Up",       pConfig->BuildEventKey(sf::Keyboard::Up));
    pConfig->RegisterInput("Player_2_Down",     pConfig->BuildEventKey(sf::Keyboard::Down));
    pConfig->RegisterInput("Player_2_Left",     pConfig->BuildEventKey(sf::Keyboard::Left));
    pConfig->RegisterInput("Player_2_Right",    pConfig->BuildEventKey(sf::Keyboard::Right));

    pConfig->RegisterInput("Exit",              pConfig->BuildEventKey(sf::Keyboard::Escape));
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

bool Game::OnSFEvent(const sf::Event& _oSFEvent)
{
    if(!EventListener::OnSFEvent(_oSFEvent))
        return false;

    return true;
}

void Game::CreateScenario()
{
    //Init Level and Cameras
    Camera* pCameraA = new Camera;     //TODO: Simplify (auto AddCamera)
    pCameraA->SetViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
    pCameraA->SetCenterOnTarget(true);
    GetGameWindow()->AddCamera(pCameraA);
    
    Camera* pCameraB = new Camera;
    pCameraB->SetViewport(sf::FloatRect(0.5f, 0.f, 0.5f, 1.f));
    pCameraB->SetCenterOnTarget(true);
    GetGameWindow()->AddCamera(pCameraB);

    Level* pLevelA = GetWorld()->GetMainLevel()->AddSubLevel(new Level);
    Level* pLevelB = GetWorld()->GetMainLevel()->AddSubLevel(new Level);

    //GetGameWindow()->BindLevel(pLevelA);
    GetGameWindow()->BindLevel(pLevelA, pCameraA);
    GetGameWindow()->BindLevel(pLevelB, pCameraB);
    
    //Fill Level
    ElementTile* pGroundA = pLevelA->GetRootNode()->AddChild<ElementTile>();
    pGroundA->SetTexture("Background.jpg");
    pGroundA->SetSize(1024.f, 1024.f);
    
    ElementSprite* pGroundB = pLevelB->GetRootNode()->AddChild<ElementSprite>();
    pGroundB->SetTexture("BraidBackground.jpg");

    //Init Player A
    m_characterA = new Character;
    pLevelA->AddActor(m_characterA);
    m_characterA->InitCharacter(0);

    m_controllerA = new ControllerPlayer;
    pLevelA->AddActor(m_controllerA);
    m_controllerA->InitController(m_characterA, 0);
    
    //Init Player B
    m_characterB = new Character;
    pLevelB->AddActor(m_characterB);
    m_characterB->InitCharacter(1);

    m_controllerB = new ControllerPlayer;
    pLevelB->AddActor(m_controllerB);
    m_controllerB->InitController(m_characterB, 1);
}

void Game::ClearScenario()
{
    GetGameWindow()->DeleteAllCameras();

    GetWorld()->ResetWorld();
}


Game* GetGame()
{
    return (Game*)GetEngine()->GetApplication();
}

}   //namespace demoproject
