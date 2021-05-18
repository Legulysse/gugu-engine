////////////////////////////////////////////////////////////////
// Header

#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/ControllerPlayer.h"
#include "Actors/Character.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementTile.h"
#include "Gugu/World/World.h"
#include "Gugu/World/Level.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

Demo::Demo()
{
    m_characterA = nullptr;
    m_controllerA = nullptr;

    m_characterB = nullptr;
    m_controllerB = nullptr;
}

Demo::~Demo()
{
}

void Demo::AppStart()
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

    inputs->RegisterInput("Player_2_Up", inputs->BuildKeyboardEvent(sf::Keyboard::Up));
    inputs->RegisterInput("Player_2_Down", inputs->BuildKeyboardEvent(sf::Keyboard::Down));
    inputs->RegisterInput("Player_2_Left", inputs->BuildKeyboardEvent(sf::Keyboard::Left));
    inputs->RegisterInput("Player_2_Right", inputs->BuildKeyboardEvent(sf::Keyboard::Right));

    inputs->RegisterInput("Exit", inputs->BuildKeyboardEvent(sf::Keyboard::Escape));

    // Run demo
    CreateScenario();
}

void Demo::AppStep(const DeltaTime& dt)
{
    StateMachine::Step(dt);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    StateMachine::Update(dt);
}

void Demo::CreateScenario()
{
    //Root UI
    m_menu = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_menu->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    ElementSprite* pSeparator = m_menu->AddChild<ElementSprite>();
    pSeparator->SetTexture("Separator.png");
    pSeparator->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedPosition(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedSize(UDim2(UDim(0.f, 6.f), UDim(1.f, 0.f)));

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

void Demo::ClearScenario()
{
    GetGameWindow()->DeleteAllCameras();

    GetWorld()->ResetWorld();
}

bool Demo::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("Exit", _oSFEvent))
    {
        GetEngine()->StopLooping();
        return false;
    }

    if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
            Camera* pCameraA = GetGameWindow()->GetCamera(0);
            Camera* pCameraB = GetGameWindow()->GetCamera(1);
            Camera* pCamera = nullptr;

            if (pCameraA && pCameraA->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraA;
                WriteInConsole("Picked Camera A");
            }
            if (pCameraB && pCameraB->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraB;
                WriteInConsole("Picked Camera B");
            }

            if (pCamera)
            {
                if (pCamera->IsMouseOverElement(kMouseCoords, m_characterA->m_sprite))
                {
                    WriteInConsole("Picked Character A");
                }
                if (pCamera->IsMouseOverElement(kMouseCoords, m_characterB->m_sprite))
                {
                    WriteInConsole("Picked Character B");
                }
            }
        }
    }

    return true;
}

Demo* GetGame()
{
    return (Demo*)GetEngine()->GetApplication();
}

}   //namespace demoproject
