////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "EditorApp.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/Engine.h"
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

    return GetEditor()->OnSFEvent(event);
}

}   //namespace gugu
