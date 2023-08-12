////////////////////////////////////////////////////////////////
// Header

#include "States/StateDemoLayoutGroup.h"

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
#include "Gugu/Element/UI/ElementLayoutGroup.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
StateDemoLayoutGroup::StateDemoLayoutGroup()
{
}

StateDemoLayoutGroup::~StateDemoLayoutGroup()
{
}

void StateDemoLayoutGroup::Init()
{
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);
    
    // Layouts
    std::vector<ElementLayoutGroup*> layoutGroups;
    ElementLayoutGroup* layout;

    layout = m_root->AddChild<ElementLayoutGroup>();
    layout->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(10.f, 10.f));
    //layout->SetSize(100.f, 200.f);
    layout->SetLayoutDirection(ELayoutDirection::Vertical);
    layout->SetWrapItems(false);
    layout->SetAutoResize(true);
    layoutGroups.push_back(layout);

    layout = m_root->AddChild<ElementLayoutGroup>();
    layout->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + Vector2f(60.f, 10.f));
    layout->SetSize(100.f, 200.f);
    layout->SetLayoutDirection(ELayoutDirection::Vertical);
    layout->SetWrapItems(true);
    layout->SetAutoResize(false);
    layoutGroups.push_back(layout);

    layout = m_root->AddChild<ElementLayoutGroup>();
    layout->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER + Vector2f(0.f, 10.f));
    //layout->SetSize(200.f, 100.f);
    layout->SetLayoutDirection(ELayoutDirection::Horizontal);
    layout->SetWrapItems(false);
    layout->SetAutoResize(true);
    layoutGroups.push_back(layout);

    layout = m_root->AddChild<ElementLayoutGroup>();
    layout->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER + Vector2f(0.f, 60.f));
    layout->SetSize(200.f, 100.f);
    layout->SetLayoutDirection(ELayoutDirection::Horizontal);
    layout->SetWrapItems(true);
    layout->SetAutoResize(false);
    layoutGroups.push_back(layout);

    for (size_t i = 0; i < layoutGroups.size(); ++i)
    {
        layoutGroups[i]->SetItemSpacing(Vector2f(2.f, 2.f));
        layoutGroups[i]->SetDebugBoundsVisible(true);
    }

    // Buttons
    for (size_t i = 0; i < layoutGroups.size(); ++i)
    {
        for (size_t ii = 0; ii < 8; ++ii)
        {
            ElementButton* button;
            button = new ElementButton;
            button->LoadFromWidget("Button_01.widget.xml");
            button->SetText(StringFormat("{0}", ii));
            button->SetSize(40.f, 40.f);

            layoutGroups[i]->AddItem(button);
        }
    }
}

void StateDemoLayoutGroup::Release()
{
    SafeDelete(m_root);
}

bool StateDemoLayoutGroup::OnSFEvent(const sf::Event& _oSFEvent)
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
