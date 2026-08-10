////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "RendererDemo.h"

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/System/Memory.h"
#include "Gugu/External/ImGuiUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
, m_renderer(nullptr)
, m_twirl(nullptr)
, m_sprite1(nullptr)
, m_sprite2(nullptr)
, m_blendModeIndex(0)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterEventHandler(GetGameWindow());

    //Create a custom Renderer
    m_renderer = new RendererDemo;
    GetGameWindow()->SetRenderer(m_renderer);   //TODO: Ability to store Renderers on the Engine

    //Set Mouse aspect
    GetGameWindow()->SetSystemMouseVisible(false);
    GetGameWindow()->SetMouseVisible(true);
    GetGameWindow()->SetMouseTexture("Mouse.png");

    //Create the Twirling effect attached to the mouse
    m_twirl = GetGameWindow()->GetMouseNode()->AddChild<ElementSprite>();
    m_twirl->SetTexture("Twirl.png");
    m_twirl->SetSize(160.0f, 160.0f);
    m_twirl->SetRenderPass(DEMO_RENDERPASS_REFRACTION);
    m_twirl->SetUnifiedOrigin(UDim2::POSITION_CENTER);

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    //Background
    ElementSprite* pTile = m_root->AddChild<ElementSprite>();
    pTile->SetRepeatTexture(true);
    pTile->SetTexture("BraidBackground-Holed.png");
    pTile->SetUnifiedSize(UDim2::SIZE_FULL);

    // Sprites.
    m_sprite1 = m_root->AddChild<ElementSprite>();
    m_sprite1->SetPosition(50, 50);
    m_sprite1->SetTexture("gugu-engine-logo-128.png");

    m_sprite2 = m_root->AddChild<ElementSprite>();
    m_sprite2->SetPosition(100, 100);
    m_sprite2->SetTexture("gugu-engine-logo-128.png");
}

void Demo::AppStop()
{
    SafeDelete(m_renderer);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    m_twirl->Rotate(dt.s() * 0.1f * -360.f);     //360° in 10s
}

void Demo::AppUpdateImGui(const DeltaTime& dt)
{
    if (ImGui::Begin("Renderer Demo"))
    {
        bool twirl = m_twirl->IsVisible();
        ImGui::Checkbox("Twirl on cursor", &twirl);
        m_twirl->SetVisible(twirl);

        static const std::vector<std::string> blendModeNames
        {
            "Alpha",        // default
            "Add",
            "Multiply",
            "MultiplyBis",  // custom
            "MultiplyBis2", // custom
            "Min",
            "Max",
            "None",
        };

        static const std::vector<sf::BlendMode> blendModes
        {
            // Notes:
            // - Helpful tool to test blendmodes : https://www.andersriggelsen.dk/glblendfunc.php

            // Sfml Alpha, Default.
            sf::BlendAlpha,
            sf::BlendAdd,
            // Sfml Multiply : Very strange behaviour with transparent pixels.
            sf::BlendMultiply,
            // MultiplyBis proposal : Seems better but not able to draw above transparent pixels.
            sf::BlendMode(
                sf::BlendMode::Factor::DstColor,
                sf::BlendMode::Factor::OneMinusSrcAlpha,
                sf::BlendMode::Equation::Add),
            // MultiplyBis2 proposal : Seems more aligned with standard Multiply.
            sf::BlendMode(
                sf::BlendMode::Factor::DstColor,
                sf::BlendMode::Factor::OneMinusSrcAlpha,
                sf::BlendMode::Equation::Add,
                sf::BlendMode::Factor::One,
                sf::BlendMode::Factor::OneMinusSrcAlpha,
                sf::BlendMode::Equation::Add),
            sf::BlendMin,
            sf::BlendMax,
            sf::BlendNone,
        };

        assert(blendModeNames.size() == blendModes.size());

        if (ImGui::Combo("Blend mode", blendModeNames, &m_blendModeIndex))
        {
            m_sprite1->SetBlendMode(blendModes[m_blendModeIndex]);
            m_sprite2->SetBlendMode(blendModes[m_blendModeIndex]);
        }
    }
    ImGui::End();
}

}   //namespace demoproject
