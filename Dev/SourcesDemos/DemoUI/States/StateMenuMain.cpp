////////////////////////////////////////////////////////////////
// Header

#include "States/StateMenuMain.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateGuiTest.h"
#include "States/StateDemoLayout.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Action.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementTile.h"
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
	ElementTile* pTile = m_root->AddChild<ElementTile>();
    pTile->SetTexture("Background.jpg");
    pTile->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

	//Logo
	ElementSprite* pLogoSfml = m_root->AddChild<ElementSprite>();
    pLogoSfml->SetTexture("SfmlLogo.png");
	pLogoSfml->SetUnifiedPosition(UDim2(0.65f, 0.f, 0.65f, 0.f));
	pLogoSfml->SetUnifiedSize(UDim2(0.35f, 0.f, 0.35f, 0.f));

	//Box Menu
    ElementSpriteGroup* pBox = m_root->AddChild<ElementSpriteGroup>();
    pBox->LoadFromXml("Box9_02.xml");
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
	pButton->SetOnMouseReleased( new ActionClass1P<StateMenuMain,int>(this, &StateMenuMain::OnButtonClick, 6) );
    pButton->SetPosition(fPosX, fPosY);

	fPosY += fGapY;
	pButton = pBox->AddChild<ElementButton>();
	pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("DemoLayout");
	pButton->SetOnMouseReleased( new ActionClass1P<StateMenuMain,int>(this, &StateMenuMain::OnButtonClick, 4) );
    pButton->SetPosition(fPosX, fPosY);

	fPosY += fGapY;
	pButton = pBox->AddChild<ElementButton>();
	pButton->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    pButton->SetText("Quit");
	pButton->SetOnMouseReleased( new ActionClass1P<StateMenuMain,int>(this, &StateMenuMain::OnButtonClick, 0) );
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

	ManagerConfig* pConfig = GetConfig();

	if (pConfig->IsInputReleased("CloseGame", _oSFEvent))
	{
		GetEngine()->Stop();
        return false;
	}

	return true;
}

void StateMenuMain::OnButtonClick(int _iButtonID)
{
	if (_iButtonID == 0)
	{
		GetEngine()->Stop();
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
