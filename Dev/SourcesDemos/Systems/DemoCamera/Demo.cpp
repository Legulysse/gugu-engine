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
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/System/Platform.h"

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
    RegisterEventHandler(GetGameWindow());

    //Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");

    ManagerInputs* inputs = GetInputs();

    //Method 1 : use a config file
    inputs->LoadInputFile("Bindings.xml");
    
    //Method 2 : register keys manually
    inputs->RegisterInput("Player_1_Up", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Z));
    inputs->RegisterInput("Player_1_Down", inputs->BuildKeyboardEvent(sf::Keyboard::Key::S));
    inputs->RegisterInput("Player_1_Left", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Q));
    inputs->RegisterInput("Player_1_Right", inputs->BuildKeyboardEvent(sf::Keyboard::Key::D));

    inputs->RegisterInput("Player_2_Up", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Up));
    inputs->RegisterInput("Player_2_Down", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Down));
    inputs->RegisterInput("Player_2_Left", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Left));
    inputs->RegisterInput("Player_2_Right", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Right));

    inputs->RegisterInput("Exit", inputs->BuildKeyboardEvent(sf::Keyboard::Key::Escape));

    // Run demo
    CreateScenario();
}

void Demo::AppStep(const DeltaTime& dt)
{
}

void Demo::AppUpdate(const DeltaTime& dt)
{
}

void Demo::CreateScenario()
{
    //Root UI
    m_menu = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_menu->SetUnifiedSize(UDim2::SIZE_FULL);

    ElementSprite* pSeparator = m_menu->AddChild<ElementSprite>();
    pSeparator->SetTexture("Separator.png");
    pSeparator->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedPosition(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedSize(UDim2(Vector2f(0.f, 1.f), Vector2f(6.f, 0.f)));

    //Init Scene and Cameras
    Camera* pCameraA = new Camera;     //TODO: Simplify (auto AddCamera)
    pCameraA->SetViewport(sf::FloatRect(Vector2f(0.f, 0.f), Vector2f(0.5f, 1.f)));
    pCameraA->SetCenterOnTarget(true);
    GetGameWindow()->AddCamera(pCameraA);
    
    Camera* pCameraB = new Camera;
    pCameraB->SetViewport(sf::FloatRect(Vector2f(0.5f, 0.f), Vector2f(0.5f, 1.f)));
    pCameraB->SetCenterOnTarget(true);
    GetGameWindow()->AddCamera(pCameraB);

    Scene* sceneA = GetScenes()->GetRootScene()->AddChildScene();
    Scene* sceneB = GetScenes()->GetRootScene()->AddChildScene();

    //GetGameWindow()->BindScene(sceneA);
    GetGameWindow()->BindScene(sceneA, pCameraA);
    GetGameWindow()->BindScene(sceneB, pCameraB);
    
    //Fill Scene
    ElementSprite* pGroundA = sceneA->GetRootNode()->AddChild<ElementSprite>();
    pGroundA->SetTexture("Background.jpg");
    pGroundA->SetRepeatTexture(true);
    pGroundA->SetSize(1024.f, 1024.f);
    
    ElementSprite* pGroundB = sceneB->GetRootNode()->AddChild<ElementSprite>();
    pGroundB->SetTexture("BraidBackground.jpg");

    //Init Player A
    m_characterA = new Character;
    sceneA->AddActor(m_characterA);
    m_characterA->InitCharacter(0);

    m_controllerA = new ControllerPlayer;
    sceneA->AddActor(m_controllerA);
    m_controllerA->InitController(m_characterA, 0);
    
    //Init Player B
    m_characterB = new Character;
    sceneB->AddActor(m_characterB);
    m_characterB->InitCharacter(1);

    m_controllerB = new ControllerPlayer;
    sceneB->AddActor(m_controllerB);
    m_controllerB->InitController(m_characterB, 1);
}

void Demo::ClearScenario()
{
    GetGameWindow()->DeleteAllCameras();

    GetScenes()->ResetRootScene();
}

bool Demo::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputEventReleased("Exit", _oSFEvent))
    {
        GetEngine()->StopMainLoop();
        return false;
    }

    if (const auto buttonPressedEvent = _oSFEvent.getIf<sf::Event::MouseButtonPressed>())
    {
        if (buttonPressedEvent->button == sf::Mouse::Button::Left)
        {
            Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
            Camera* pCameraA = GetGameWindow()->GetCamera(0);
            Camera* pCameraB = GetGameWindow()->GetCamera(1);
            Camera* pCamera = nullptr;

            if (pCameraA && pCameraA->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraA;
                WriteInIDEConsoleEndline("Picked Camera A");
            }
            if (pCameraB && pCameraB->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraB;
                WriteInIDEConsoleEndline("Picked Camera B");
            }

            if (pCamera)
            {
                if (pCamera->IsMouseOverElement(kMouseCoords, m_characterA->m_sprite))
                {
                    WriteInIDEConsoleEndline("Picked Character A");
                }
                if (pCamera->IsMouseOverElement(kMouseCoords, m_characterB->m_sprite))
                {
                    WriteInIDEConsoleEndline("Picked Character B");
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
