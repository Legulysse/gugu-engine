////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"

#include <imgui.h>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
, m_showImGuiDemo(false)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Register Inputs
    ManagerConfig* pConfig = GetConfig();
    pConfig->RegisterInput("ToggleImGui", pConfig->BuildKeyboardEvent(sf::Keyboard::F1));
    pConfig->RegisterInput("CloseGame", pConfig->BuildKeyboardEvent(sf::Keyboard::Escape));

    // Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    if (m_showImGuiDemo)
    {
        ImGui::ShowDemoWindow();
    }
}

bool Demo::OnSFEvent(const sf::Event& event)
{
    if (!EventListener::OnSFEvent(event))
        return false;

    ManagerConfig* config = GetConfig();

    if (config->IsInputPressed("ToggleImGui", event))
    {
        m_showImGuiDemo = !m_showImGuiDemo;
        return false;
    }
    else if (config->IsInputReleased("CloseGame", event))
    {
        GetEngine()->StopLooping();
        return false;
    }

    return true;
}

}   //namespace demoproject
