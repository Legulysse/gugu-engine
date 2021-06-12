////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include <imgui.h>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
, m_textToggle(nullptr)
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
    ManagerInputs* inputs = GetInputs();
    inputs->RegisterInput("ToggleImGui", inputs->BuildKeyboardEvent(sf::Keyboard::F1));
    inputs->RegisterInput("UseMouse", inputs->BuildKeyboardEvent(sf::Keyboard::F2));
    inputs->RegisterInput("UseSystemMouse", inputs->BuildKeyboardEvent(sf::Keyboard::F3));
    inputs->RegisterInput("CloseGame", inputs->BuildKeyboardEvent(sf::Keyboard::Escape));

    // Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    // Instructions
    ElementText* textInstructions = m_root->AddChild<ElementText>();
    textInstructions->SetText("F1 : Toggle ImGui Demo.\nF2 : Use Game Mouse.\nF3 : Use System Mouse.\n\nEscape : Close Demo.");
    textInstructions->SetPosition(15, 15);

    // Toggle text and Test Buttons
    ElementButton* pButton;
    float fPosX = 30.f;
    float fPosY = 190.f;
    float fGapTextBigY = 56.f;
    float fGapButtonY = 56.f;

    ElementText* textHelp = m_root->AddChild<ElementText>();
    textHelp->SetText("Use the components below to test focus and event propagation below ImGui panels.");
    textHelp->SetMultiline(true);
    textHelp->SetSize(400, 200);
    textHelp->SetPosition(fPosX, fPosY);

    fPosY += fGapTextBigY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Toggle Text A");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::ToggleTextA));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapButtonY;
    pButton = m_root->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Toggle Text B");
    pButton->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::ToggleTextB));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapButtonY;
    m_textToggle = m_root->AddChild<ElementText>();
    m_textToggle->SetText("No text selected.");
    m_textToggle->SetPosition(fPosX, fPosY);

    fPosY += fGapTextBigY;
    ElementText* textEdit = m_root->AddChild<ElementText>();
    textEdit->SetText("Edit me...");
    textEdit->SetDebugBoundsVisible(true);
    textEdit->SetEditable(true);
    textEdit->SetPosition(fPosX, fPosY);
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

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputPressed("ToggleImGui", event))
    {
        m_showImGuiDemo = !m_showImGuiDemo;
        return false;
    }
    else if (inputs->IsInputPressed("UseMouse", event))
    {
        GetGameWindow()->SetMouseVisible(true);
        GetGameWindow()->SetSystemMouseVisible(false);
        return false;
    }
    else if (inputs->IsInputPressed("UseSystemMouse", event))
    {
        GetGameWindow()->SetMouseVisible(false);
        GetGameWindow()->SetSystemMouseVisible(true);
        return false;
    }
    else if (inputs->IsInputReleased("CloseGame", event))
    {
        GetEngine()->StopMainLoop();
        return false;
    }

    return true;
}

void Demo::OnButtonClick(EButton button)
{
    if (button == EButton::ToggleTextA)
    {
        m_textToggle->SetText("Text A A A A A.");
    }
    else if (button == EButton::ToggleTextB)
    {
        m_textToggle->SetText("Text B B B B B.");
    }
}

}   //namespace demoproject
