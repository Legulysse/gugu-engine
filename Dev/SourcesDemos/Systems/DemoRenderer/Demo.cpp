////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "RendererDemo.h"

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
, m_renderer(nullptr)
, m_twirl(nullptr)
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
    m_twirl->SetUnifiedOrigin(UDim2(0.5, 0.0, 0.5, 0.0));

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //Background
    ElementSprite* pTile = m_root->AddChild<ElementSprite>();
    pTile->SetRepeatTexture(true);
    pTile->SetTexture("BraidBackground.jpg");
    pTile->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));
}

void Demo::AppStop()
{
    SafeDelete(m_renderer);
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    m_twirl->Rotate(dt.s() * 0.1f * -360.f);     //360° in 10s
}

}   //namespace demoproject
