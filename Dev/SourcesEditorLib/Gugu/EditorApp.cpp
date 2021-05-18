////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/EditorApp.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/EditorCore.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
EditorApp::EditorApp()
    : m_editorCore(nullptr)
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
    m_editorCore = new EditorCore;
    m_editorCore->Start();
}

void EditorApp::AppStop()
{
    m_editorCore->Stop();
    SafeDelete(m_editorCore);
}

void EditorApp::AppUpdate(const DeltaTime& dt)
{
    m_editorCore->Update(dt);
}

bool EditorApp::OnSFEvent(const sf::Event& event)
{
    if (!EventListener::OnSFEvent(event))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("CloseApp", event))
    {
        GetEngine()->StopLooping();
        return false;
    }
    else if (inputs->IsInputReleased("ResetPanels", event))
    {
        m_editorCore->ResetPanels();
        return false;
    }

    return true;
}

}   //namespace gugu
