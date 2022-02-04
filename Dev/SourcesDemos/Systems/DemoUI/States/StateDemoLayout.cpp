////////////////////////////////////////////////////////////////
// Header

#include "States/StateDemoLayout.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateMenuMain.h"

#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Element/UI/ElementUILayout.h"

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
    RegisterHandlerEvents(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));
    
    //Layout
    ElementUILayout* pLayout = m_root->AddChild<ElementUILayout>();
    pLayout->LoadFromFile("LayoutDemo.xml");
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
