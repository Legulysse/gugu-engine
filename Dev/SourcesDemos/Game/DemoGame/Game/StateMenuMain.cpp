////////////////////////////////////////////////////////////////
// Header

#include "Game/StateMenuMain.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/StateScenario.h"

#include "Gugu/Engine.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
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

    //Root UI
    m_menu = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_menu->SetUnifiedSize(UDim2::SIZE_FULL);

    //Box Menu
    ElementSpriteGroup* pBox = m_menu->AddChild<ElementSpriteGroup>();
    pBox->LoadFromWidget("Box9_Default_02.widget.xml");
    pBox->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(50.f, 50.f));

    ElementButton* pButton;
    float padding = 20.f;
    float fGapY = 10.f;
    float fPosX = padding;
    float fPosY = padding;

    pButton = pBox->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("New Game", "Roboto-Regular.ttf");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 1));
    pButton->SetPosition(fPosX, fPosY);

    fPosY += fGapY + pButton->GetSize().y;
    pButton = pBox->AddChild<ElementButton>();
    pButton->LoadFromWidget("Button_01.widget.xml");
    pButton->SetText("Quit", "Roboto-Regular.ttf");
    pButton->SetOnMouseReleased(std::bind(&StateMenuMain::OnButtonClick, this, 0));
    pButton->SetPosition(fPosX, fPosY);

    pBox->SetSize(pButton->GetSize().x + padding * 2.f, fPosY + pButton->GetSize().y + padding);
}

void StateMenuMain::Release()
{
    SafeDelete(m_menu);
}

void StateMenuMain::Enter(State* _pStateFrom)
{
}

void StateMenuMain::Exit(State* _pStateTo)
{
}

void StateMenuMain::Step(const DeltaTime& dt)
{
}

void StateMenuMain::Update(const DeltaTime& dt)
{
}

bool StateMenuMain::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputEventReleased("Exit", _oSFEvent))
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
        GetOwner()->ChangeState(new StateScenario);
    }
}

}   //namespace demoproject
