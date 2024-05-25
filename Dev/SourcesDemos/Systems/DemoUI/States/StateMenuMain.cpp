////////////////////////////////////////////////////////////////
// Header

#include "States/StateMenuMain.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateDemoTest.h"
#include "States/StateDemoLayoutGroup.h"
#include "States/StateDemoWidget.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/Memory.h"
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
    : m_root(nullptr)
    , m_menu(nullptr)
{
}

StateMenuMain::~StateMenuMain()
{
}

void StateMenuMain::Init()
{
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Background
    ElementSprite* background = m_root->AddChild<ElementSprite>();
    background->SetTexture("Background.jpg");
    background->SetRepeatTexture(true);
    background->SetUnifiedSize(UDim2::SIZE_FULL);

    // Logo
    ElementSprite* sfmlLogo = m_root->AddChild<ElementSprite>();
    sfmlLogo->SetTexture("sfml-logo-small.png");
    sfmlLogo->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    sfmlLogo->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + Vector2f(-30.f, -10.f));

    // Box Menu
    Element* box = m_root->AddChildWidget("Box9_Default_01.widget.xml");

    ElementLayoutGroup* verticalLayout = box->AddChild<ElementLayoutGroup>();
    verticalLayout->SetMainDirection(ELayoutDirection::TopToBottom);
    verticalLayout->SetItemSpacing(10.f);
    verticalLayout->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(20.f, 20.f));
    verticalLayout->SetSize(300.f, 350.f);

    m_menu = box;
    m_menu->GetEvents()->AddCallback(EInteractionEvent::MouseDragMoved, [](const InteractionInfos&) {});
    m_menu->SetPosition(Vector2f(50.f, 50.f));
    m_menu->SetUnifiedSize(UDim2::SIZE_FULL + Vector2f(40.f, 40.f), verticalLayout);

    // Buttons
    ElementButton* button;
    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Gui Test");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 1));
    verticalLayout->AddItem(button);

    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Demo Layout Group");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 2));
    verticalLayout->AddItem(button);

    button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Demo Widget");
    button->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 3));
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
    else if (_iButtonID == 1)
    {
        GetOwner()->ChangeState(new StateDemoTest);   //TODO: rename as ChangeState and ChangeStateDelayed, instead of ChangeStateNow and ChangeState.
    }
    else if (_iButtonID == 2)
    {
        GetOwner()->ChangeState(new StateDemoLayoutGroup);
    }
    else if (_iButtonID == 3)
    {
        GetOwner()->ChangeState(new StateDemoWidget);
    }
}

}   //namespace demoproject
