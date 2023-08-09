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
#include "Gugu/Element/UI/ElementLayoutGroup.h"

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
    box->SetSize(240, 220);

    m_menu = box;
    m_menu->GetEvents()->AddCallback(EInteractionEvent::MouseDragMoved, [](const InteractionInfos&) {});

    ElementLayoutGroup* verticalLayout = box->AddChild<ElementLayoutGroup>();
    verticalLayout->SetLayoutDirection(ELayoutDirection::Vertical);
    verticalLayout->SetItemSpacing(10.f);
    verticalLayout->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(20.f, 20.f));

    ElementButton* button;
    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Gui Test");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 6));
    verticalLayout->AddItem(button);

    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Demo Layout");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 4));
    verticalLayout->AddItem(button);

    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Disabled");
    button->SetDisabled(true);
    verticalLayout->AddItem(button);

    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Quit");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 0));
    verticalLayout->AddItem(button);
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
