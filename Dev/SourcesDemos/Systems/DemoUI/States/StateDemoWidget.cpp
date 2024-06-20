////////////////////////////////////////////////////////////////
// Header

#include "States/StateDemoWidget.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateMenuMain.h"

#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Element/UI/ElementCheckbox.h"
#include "Gugu/System/Memory.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
StateDemoWidget::StateDemoWidget()
{
}

StateDemoWidget::~StateDemoWidget()
{
}

void StateDemoWidget::Init()
{
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);
    
    // Widget
    ElementPathBindings pathBindings;
    m_root->AddChild(GetResources()->GetElementWidget("WidgetTest.widget.xml")->InstanciateWidget(pathBindings));

    // Customize widget elements
    if (ElementButton* button = pathBindings.GetElementAs<ElementButton>("test button"))
    {
        button->SetText("Button Text");
    }

    if (ElementCheckbox* checkbox = pathBindings.GetElementAs<ElementCheckbox>("test checkbox"))
    {
        checkbox->SetText("Checkbox Text");
    }
}

void StateDemoWidget::Release()
{
    SafeDelete(m_root);
}

bool StateDemoWidget::OnSFEvent(const sf::Event& _oSFEvent)
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
