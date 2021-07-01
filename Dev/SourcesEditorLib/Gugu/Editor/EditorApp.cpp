////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/EditorApp.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorCore.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
EditorApp::EditorApp()
{
}

EditorApp::~EditorApp()
{
}

void EditorApp::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Register Inputs.
    ManagerInputs* inputs = GetInputs();
    inputs->RegisterInput("CloseApp", inputs->BuildKeyboardEvent(sf::Keyboard::Escape));
    inputs->RegisterInput("ResetPanels", inputs->BuildKeyboardEvent(sf::Keyboard::F1));

    // Setup EditorCore.
    GetEditor()->Init();
}

void EditorApp::AppStop()
{
    GetEditor()->Release();
}

void EditorApp::AppUpdate(const DeltaTime& dt)
{
    GetEditor()->Update(dt);
}

bool EditorApp::OnSFEvent(const sf::Event& event)
{
    if (!EventListener::OnSFEvent(event))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("CloseApp", event))
    {
        GetEngine()->StopMainLoop();
        return false;
    }
    else if (inputs->IsInputReleased("ResetPanels", event))
    {
        GetEditor()->ResetPanels();
        return false;
    }

    return true;
}

}   //namespace gugu
