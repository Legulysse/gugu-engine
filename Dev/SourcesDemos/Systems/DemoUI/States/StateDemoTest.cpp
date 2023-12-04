////////////////////////////////////////////////////////////////
// Header

#include "States/StateDemoTest.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateMenuMain.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/UI/ElementEditableText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Element/UI/ElementCheckbox.h"
#include "Gugu/Element/UI/ElementSlider.h"
#include "Gugu/Element/UI/ElementLayoutGroup.h"
#include "Gugu/Element/UI/ElementList.h"
#include "Gugu/Element/UI/ElementListItem.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
StateDemoTest::StateDemoTest()
{
}

StateDemoTest::~StateDemoTest()
{
}

void StateDemoTest::Init()
{
    RegisterEventHandler(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2::SIZE_FULL);

    // Background
    ElementSprite* pTile = m_root->AddChild<ElementSprite>();
    pTile->SetTexture("Background.jpg");
    pTile->SetRepeatTexture(true);
    pTile->SetUnifiedSize(UDim2::SIZE_FULL);

    // Vertical layout
    ElementLayoutGroup* layout = m_root->AddChild<ElementLayoutGroup>();
    layout->SetPosition(10.f, 10.f);
    layout->SetItemSpacing(4.f);
    layout->SetMainDirection(ELayoutDirection::TopToBottom);

    // Button
    ElementButton* button = new ElementButton;
    button->LoadFromWidget("Button_01.widget.xml");
    button->SetText("Press Me");
    button->SetDebugBoundsVisible(true);
    layout->AddItem(button);

    // Checkbox
    ElementCheckbox* checkbox = new ElementCheckbox;
    checkbox->LoadFromWidget("Checkbox_01.widget.xml");
    checkbox->SetText("Toggle Me");
    checkbox->SetChecked(true);
    checkbox->SetDebugBoundsVisible(true);
    layout->AddItem(checkbox);

    // Slider
    ElementSlider* slider = new ElementSlider;
    slider->LoadFromWidget("Slider_01.widget.xml");
    slider->SetValueLimits(-100, 100);
    slider->SetValue(50);
    slider->SetDebugBoundsVisible(true);
    layout->AddItem(slider);

    ElementText* sliderText = slider->AddChild<ElementText>();
    sliderText->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT);
    sliderText->SetText(ToString(slider->GetValue()));

    slider->SetOnValueChanged([=]()
    {
        sliderText->SetText(ToString(slider->GetValue()));
    });

    // List
    ElementList* pList = m_root->AddChild<ElementList>();
    pList->SetPosition(40.f, 250.f);
    pList->SetSize(200.f, 200.f);
    //pList->SetScaleY(0.7f);
    pList->Rotate(22.5f);
    pList->SetImageSet("ScrollBar_01.imageset.xml");
    pList->SetDebugBoundsVisible(true);

    for (int i=0; i<51; ++i)
    {
        ElementText* pText = new ElementText;
        pText->SetText("Element " + ToString(i));
        pText->SetDebugBoundsVisible(true);

        ElementListItem* pItem = new ElementListItem();
        pItem->SetElement(pText);

        pList->AddItem(pItem);
    }

    // Editable single line text
    ElementEditableText* pEditLine = m_root->AddChild<ElementEditableText>();
    //pEditLine->SetResizeRule(TextResizeRule::FitScale);
    pEditLine->SetText("Single Line - Edit Me");
    pEditLine->SetMultiline(false);
    pEditLine->SetEditable(true);
    pEditLine->SetPosition(300.f, 10.f);
    //pEditLine->SetSize(300.f, 200.f);
    pEditLine->SetDebugBoundsVisible(true);

    // Editable multiline text
    ElementEditableText* pText = m_root->AddChild<ElementEditableText>();
    pText->SetResizeRule(ETextResizeRule::FitHeight);
    pText->SetSize(600.f, 800.f);
    pText->SetMultiline(true);
    pText->SetEditable(true);
    pText->SetText("Multiple Lines - Edit Me\n\nGraeci recteque molestiae vel ex, no est dico dissentiet, an dolor legere habemus eos. Regione perfecto delicatissimi ei vis, eu commodo pertinacia reprimique per. Dolore dissentiunt quo eu. Mea harum probatus efficiendi cu. Animal aliquid id qui, tibique probatus at duo, ne mea summo laudem definitionem. Amet stet dicta ei per. Ius errem luptatum instructior cu, eu integre facilisis adipiscing duo, ex ubique eripuit debitis qui.\n Ut sit everti electram theophrastus, sumo expetendis pri ad, has officiis pertinax ex. Quod fabellas cu vim, ei qui impedit mnesarchum, eum veniam admodum at. Est elit oratio eu, vim ferri possim denique an. Eu clita dolores sed, vis mnesarchum percipitur dissentiet ex, sea rebum scripta deterruisset te. No praesent imperdiet rationibus his.");
    pText->SetPosition(300.f, 50.f);
    pText->SetDebugBoundsVisible(true);
}

void StateDemoTest::Release()
{
    SafeDelete( m_root );
}

bool StateDemoTest::OnSFEvent(const sf::Event& _oSFEvent)
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
