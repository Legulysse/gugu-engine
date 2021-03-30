////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/EditorApp.h"

////////////////////////////////////////////////////////////////
// Includes

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
    RegisterHandlerEvents(GetGameWindow());
}

void EditorApp::AppStop()
{
}

void EditorApp::AppUpdate(const DeltaTime& dt)
{
    ImGui::ShowDemoWindow();
}

}   //namespace demoproject
