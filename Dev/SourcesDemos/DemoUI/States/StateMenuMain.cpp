////////////////////////////////////////////////////////////////
// Header

#include "States/StateMenuMain.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateGuiTest.h"
#include "States/StateDemoLayout.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

StateMenuMain::StateMenuMain()
{
}

StateMenuMain::~StateMenuMain()
{
}

void StateMenuMain::Init()
{
    RegisterHandlerEvents(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //Background
    ElementSprite* pTile = m_root->AddChild<ElementSprite>();
    pTile->SetTexture("Background.jpg");
    pTile->SetRepeatTexture(true);
    pTile->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //Logo
    ElementSprite* pLogoSfml = m_root->AddChild<ElementSprite>();
    pLogoSfml->SetTexture("sfml-logo-small.png");
    pLogoSfml->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    pLogoSfml->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + Vector2f(-30.f, -10.f));

    //Box Menu
    ElementSpriteGroup* pBox = m_root->AddChild<ElementSpriteGroup>();
    pBox->LoadFromFile("Box9_02.xml");
    pBox->SetUnifiedPosition(UDim2(0.f, 50.f, 0.f, 50.f));

    m_menu = pBox;
    m_menu->AddInteractionFlag(EInteraction::Drag);

    ElementButton* pButton;
    float fPosX = 20.f;
    float fPosY = 20.f;
    float fGapY = 64.f;

    pButton = pBox->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("GuiTest");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 6));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = pBox->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("DemoLayout");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 4));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY;
    pButton = pBox->AddChild<ElementButton>();
    pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Quit");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 0));
    pButton->SetPosition(fPosX, fPosY);

    pBox->SetSize(pButton->GetSize().x + 40.f, fPosY + pButton->GetSize().y + 20.f);
}

void StateMenuMain::Release()
{
    SafeDelete(m_root);
}

bool StateMenuMain::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("CloseGame", _oSFEvent))
    {
        GetEngine()->StopMainLoop();
        return false;
    }

    return true;
}

void StateMenuMain::OnButtonClick(int _iButtonID)
{
    if (_iButtonID == 0)
    {
        GetEngine()->StopMainLoop();
    }
    else if (_iButtonID == 4)
    {
        GetOwner()->ChangeState(new StateDemoLayout);   //TODO: rename as ChangeState and ChangeStateDelayed, instead of ChangeStateNow and ChangeState.
    }
    else if (_iButtonID == 6)
    {
        GetOwner()->ChangeState(new StateGuiTest);
    }
}

}   //namespace demoproject
