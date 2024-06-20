////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "EditorApp.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Window.h"

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
    RegisterEventHandler(GetGameWindow());
}

void EditorApp::AppStop()
{
    GetEditor()->Release();
}

void EditorApp::AppUpdateImGui(const DeltaTime& dt)
{
    GetEditor()->Update(dt);
}

bool EditorApp::OnMainWindowCloseEvent()
{
    // The Editor may decide to not close if some documents are dirty.
    GetEditor()->CloseEditor();
    return false;
}

bool EditorApp::OnSFEvent(const sf::Event& event)
{
    if (!EventListener::OnSFEvent(event))
        return false;

    return GetEditor()->OnSFEvent(event);
}

}   //namespace gugu
