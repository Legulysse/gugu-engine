////////////////////////////////////////////////////////////////
// Header

#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Window.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");
    
    // Register Inputs
    ManagerConfig* pConfig = GetConfig();
    pConfig->RegisterInput("CloseGame", pConfig->BuildEventKey(sf::Keyboard::Escape));
}

void Demo::AppStop()
{
}

void Demo::AppStep(const DeltaTime& dt)
{
    // Ensure the StateMachine Step is called (there is no automated call built-in)
    StateMachine::Step(dt);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    // Ensure the StateMachine Update is called (there is no automated call built-in)
    StateMachine::Update(dt);
}

}   //namespace demoproject
