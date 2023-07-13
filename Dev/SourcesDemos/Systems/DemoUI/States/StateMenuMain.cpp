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
#include "Gugu/Events/ElementEventHandler.h"
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
    RegisterEventHandler(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    //Background
    ElementSprite* pTile = m_root->AddChild<ElementSprite>();
    pTile->SetTexture("Background.jpg");
    pTile->SetRepeatTexture(true);
    pTile->SetUnifiedSize(UDim2::SIZE_FULL);

    //Logo
    ElementSprite* pLogoSfml = m_root->AddChild<ElementSprite>();
    pLogoSfml->SetTexture("sfml-logo-small.png");
    pLogoSfml->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    pLogoSfml->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + Vector2f(-30.f, -10.f));

    //Box Menu
    Element* box = m_root->AddChildWidget("Box9_Default_01.widget.xml");
    box->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(50.f, 50.f));

    m_menu = box;
    m_menu->GetEvents()->AddCallback(EInteractionEvent::MouseDragMoved, [](const InteractionInfos&) {});

    ElementButton* pButton;
    float padding = 20.f;
    float fGapY = 10.f;
    float fPosX = padding;
    float fPosY = padding;

    pButton = dynamic_cast<ElementButton*>(box->AddChildWidget("Button_01.widget.xml")); // Alternative 1 : instanciate widget and cast as button.
    pButton->SetText("Gui Test");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 6));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY + pButton->GetSize().y;
    pButton = box->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml"); // Alternative 2 : instanciate a button and load a widget data inside.
    pButton->SetText("Demo Layout");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 4));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY + pButton->GetSize().y;
    pButton = box->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Disabled");
    pButton->SetDisabled(true);
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY + pButton->GetSize().y;
    pButton = box->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Quit");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 0));
    pButton->SetPosition(fPosX, fPosY);

    box->SetSize(pButton->GetSize().x + padding * 2.f, fPosY + pButton->GetSize().y + padding);
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

    if (inputs->IsInputEventReleased("CloseGame", _oSFEvent))
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
