////////////////////////////////////////////////////////////////
// Header

#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Inputs/ManagerInputs.h"
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
    RegisterEventHandler(GetGameWindow());

    // Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");
    
    // Register Inputs
    ManagerInputs* inputs = GetInputs();
    inputs->RegisterInput("CloseGame", inputs->BuildKeyboardEvent(sf::Keyboard::Scan::Escape));
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
