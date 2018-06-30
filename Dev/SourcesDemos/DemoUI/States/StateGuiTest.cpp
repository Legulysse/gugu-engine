////////////////////////////////////////////////////////////////
// Header

#include "States/StateGuiTest.h"

////////////////////////////////////////////////////////////////
// Includes

#include "States/StateMenuMain.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Manager/ManagerAudio.h"
#include "Gugu/Manager/ManagerNetwork.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Action.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/2D/ElementTile.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Element/UI/ElementList.h"
#include "Gugu/Element/UI/ElementListItem.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
StateGuiTest::StateGuiTest()
{
}

StateGuiTest::~StateGuiTest()
{
}

void StateGuiTest::Init()
{
    RegisterHandlerEvents(GetGameWindow());

    //Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
	m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

	//Background
	ElementTile* pTile = m_root->AddChild<ElementTile>();
	pTile->SetTexture("Background.jpg");
	pTile->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //Test EditLine
	ElementText* pEditLine = m_root->AddChild<ElementText>();
    //pEditLine->SetResizeRule(TextResizeRule::FitScale);
    pEditLine->SetText("Edit Me !");
	pEditLine->SetMultiline(false);
	pEditLine->SetEditable(true);
	pEditLine->SetPosition(10.f, 10.f);
	//pEditLine->SetSize(300.f, 200.f);
    pEditLine->SetDebugBoundsVisible(true);

	//List
	ElementList* pList = m_root->AddChild<ElementList>();
	pList->SetPosition(40.f, 250.f);
	pList->SetSize(200.f, 200.f);
	//pList->SetScaleY(0.7f);
	pList->Rotate(22.5f);
    pList->SetImageSet("ScrollBar01.imageset.xml");

	for (int i=0; i<51; ++i)
	{
		ElementText* pText = new ElementText;
        pText->SetText("Element " + ToString(i));
        pText->SetDebugBoundsVisible(true);

        ElementListItem* pItem = new ElementListItem();
        pItem->SetElement(pText);

		pList->AddItem(pItem);
	}

	//Test Multilined text
	ElementText* pText = m_root->AddChild<ElementText>();
    pText->SetResizeRule(ETextResizeRule::FitHeight);
	pText->SetSize(600.f, 800.f);
	pText->SetMultiline(true);
	pText->SetEditable(true);
    pText->SetText("Graeci recteque\n\n molestiae   vel ex, no est dico dissentiet, an dolor legere habemus eos. Regione perfecto delicatissimi ei vis, eu commodo pertinacia reprimique per. Dolore dissentiunt quo eu. Mea harum probatus efficiendi cu. Animal aliquid id qui, tibique probatus at duo, ne mea summo laudem definitionem. Amet stet dicta ei per. Ius errem luptatum instructior cu, eu integre facilisis adipiscing duo, ex ubique eripuit debitis qui.\n Ut sit everti electram theophrastus, sumo expetendis pri ad, has officiis pertinax ex. Quod fabellas cu vim, ei qui impedit mnesarchum, eum veniam admodum at. Est elit oratio eu, vim ferri possim denique an. Eu clita dolores sed, vis mnesarchum percipitur dissentiet ex, sea rebum scripta deterruisset te. No praesent imperdiet rationibus his.");
	pText->SetPosition(300.f, 0.f);
    pText->SetDebugBoundsVisible(true);
}

void StateGuiTest::Release()
{
	SafeDelete( m_root );
}

bool StateGuiTest::OnSFEvent(const sf::Event& _oSFEvent)
{
	if (!EventListener::OnSFEvent(_oSFEvent))
		return false;

	ManagerConfig* pConfig = GetConfig();

	if (pConfig->IsInputReleased("CloseGame", _oSFEvent))
	{
		GetOwner()->ChangeState(new StateMenuMain);
        return false;
	}

	return true;
}

}   //namespace demoproject
