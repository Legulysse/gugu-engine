////////////////////////////////////////////////////////////////
// Header

#include "States/StateDemoLayout.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateMenuMain.h"

#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
StateDemoLayout::StateDemoLayout()
{
}

StateDemoLayout::~StateDemoLayout()
{
}

void StateDemoLayout::Init()
{
    RegisterEventHandler(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);
    
    //Layout
    m_root->AddChild(GetResources()->GetElementWidget("WidgetTest.widget.xml")->InstanciateWidget());
}

void StateDemoLayout::Release()
{
    SafeDelete(m_root);
}

bool StateDemoLayout::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputEventReleased("CloseGame", _oSFEvent))
    {
        GetOwner()->ChangeState(new StateMenuMain);
        return false;
    }

    return true;
}

}   //namespace demoproject
