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
#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
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
    ElementSprite* background = m_root->AddChild<ElementSprite>();
    background->SetTexture("Background.jpg");
    background->SetRepeatTexture(true);
    background->SetUnifiedSize(UDim2::SIZE_FULL);

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
    ElementList* list = m_root->AddChild<ElementList>();
    list->SetSize(200.f, 150.f);
    list->SetImageSet("ScrollBar_01.imageset.xml");
    list->SetDebugBoundsVisible(true);
    layout->AddItem(list);

    for (int i = 0; i < 51; ++i)
    {
        Element* itemRoot = new Element;
        itemRoot->SetSizeY(30);

        ElementText* itemText = itemRoot->AddChild<ElementText>();
        itemText->SetText("Item " + ToString(i));
        itemText->SetDebugBoundsVisible(true);

        ElementButton* itemButton = itemText->AddChild<ElementButton>();
        itemButton->LoadFromWidget("Button_01.widget.xml");
        itemButton->SetPosition(90, 0);
        itemButton->SetSize(60, 30);
        itemButton->SetText("Btn");

        ElementListItem* item = new ElementListItem();
        item->SetElement(itemRoot);
        //item->SetSizeY(30);

        list->AddItem(item);
    }

    // List (rotated)
    ElementList* listB = m_root->AddChild<ElementList>();
    listB->SetPosition(80.f, 350.f);
    listB->SetSize(200.f, 200.f);
    //listB->SetScaleY(0.7f);
    listB->Rotate(22.5f);
    listB->SetImageSet("ScrollBar_01.imageset.xml");
    listB->SetDebugBoundsVisible(true);

    for (int i=0; i<51; ++i)
    {
        ElementText* itemText = new ElementText;
        itemText->SetText("Element " + ToString(i));
        itemText->SetDebugBoundsVisible(true);

        ElementListItem* item = new ElementListItem();
        item->SetElement(itemText);

        listB->AddItem(item);
    }

    // utf8 (french) static multiline text
    ElementText* multilineText = m_root->AddChild<ElementText>();
    multilineText->SetResizeRule(ETextResizeRule::FitHeight);
    multilineText->SetSize(600.f, 800.f);
    multilineText->SetMultiline(true);
    multilineText->SetText("L'intérêt de ce texte est de contenir des caractères spéciaux.\nComme une arobase @, un dollar $, ou un dièse #.");
    multilineText->SetPosition(300.f, 10.f);
    multilineText->SetDebugBoundsVisible(true);

    // Editable single line text
    ElementEditableText* editLine = m_root->AddChild<ElementEditableText>();
    //editLine->SetResizeRule(TextResizeRule::FitScale);
    editLine->SetText("Single Line - Edit Me");
    editLine->SetMultiline(false);
    editLine->SetEditable(true);
    editLine->SetPosition(300.f, 80.f);
    //editLine->SetSize(300.f, 200.f);
    editLine->SetDebugBoundsVisible(true);

    // Editable multiline text
    ElementEditableText* editMultilineText = m_root->AddChild<ElementEditableText>();
    editMultilineText->SetResizeRule(ETextResizeRule::FitHeight);
    editMultilineText->SetSize(600.f, 800.f);
    editMultilineText->SetMultiline(true);
    editMultilineText->SetEditable(true);
    editMultilineText->SetText("Multiple Lines - Edit Me\n\nGraeci recteque molestiae vel ex, no est dico dissentiet, an dolor legere habemus eos. Regione perfecto delicatissimi ei vis, eu commodo pertinacia reprimique per. Dolore dissentiunt quo eu. Mea harum probatus efficiendi cu. Animal aliquid id qui, tibique probatus at duo, ne mea summo laudem definitionem. Amet stet dicta ei per. Ius errem luptatum instructior cu, eu integre facilisis adipiscing duo, ex ubique eripuit debitis qui.\n Ut sit everti electram theophrastus, sumo expetendis pri ad, has officiis pertinax ex. Quod fabellas cu vim, ei qui impedit mnesarchum, eum veniam admodum at. Est elit oratio eu, vim ferri possim denique an. Eu clita dolores sed, vis mnesarchum percipitur dissentiet ex, sea rebum scripta deterruisset te. No praesent imperdiet rationibus his.");
    editMultilineText->SetPosition(300.f, 120.f);
    editMultilineText->SetDebugBoundsVisible(true);
}

void StateDemoTest::Release()
{
    SafeDelete(m_root);
}

bool StateDemoTest::OnSFEvent(const sf::Event& event)
{
    if (!EventListener::OnSFEvent(event))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputEventReleased("CloseGame", event))
    {
        GetOwner()->ChangeState(new StateMenuMain);
        return false;
    }

    return true;
}

}   //namespace demoproject
