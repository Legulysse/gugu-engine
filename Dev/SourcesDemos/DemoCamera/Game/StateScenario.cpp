////////////////////////////////////////////////////////////////
// Header

#include "Game/StateScenario.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"
#include "Game/StateMenuMain.h"
#include "Actors/Character.h"

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Manager/ManagerConfig.h"
#include "Gugu/Manager/ManagerAudio.h"
#include "Gugu/Manager/ManagerNetwork.h"

#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
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

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

StateScenario::StateScenario()
{
}

StateScenario::~StateScenario()
{
}

void StateScenario::Init()
{
    RegisterHandlerEvents(GetGameWindow());

    //Root UI
    m_menu = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_menu->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));
    
    ElementSprite* pSeparator = m_menu->AddChild<ElementSprite>();
    pSeparator->SetTexture("Separator.png");
    pSeparator->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedPosition(UDim2::POSITION_CENTER);
    pSeparator->SetUnifiedSize(UDim2(UDim(0.f, 6.f), UDim(1.f, 0.f)));

    //Init Level
    GetGame()->CreateScenario();
}

void StateScenario::Release()
{
    GetGame()->ClearScenario();

    SafeDelete(m_menu);
}

void StateScenario::Enter(State* _pStateFrom)
{
}

void StateScenario::Exit(State* _pStateTo)
{
}

void StateScenario::Step(const DeltaTime& dt)
{
}

void StateScenario::Update(const DeltaTime& dt)
{
}

bool StateScenario::OnSFEvent(const sf::Event& _oSFEvent)
{
    if(!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerConfig* pConfig = GetConfig();

    if (pConfig->IsInputReleased("Exit", _oSFEvent))
    {
        GetOwner()->ChangeState(new StateMenuMain);
        return false;
    }

    if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i kMouseCoords = GetGameWindow()->GetMousePixelCoords();
            Camera* pCameraA = GetGameWindow()->GetCamera(0);
            Camera* pCameraB = GetGameWindow()->GetCamera(1);
            Camera* pCamera = nullptr;
            
            if (pCameraA && pCameraA->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraA;
                WriteInConsole("Picked Camera A");
            }
            if (pCameraB && pCameraB->IsMouseOverCamera(kMouseCoords))
            {
                pCamera = pCameraB;
                WriteInConsole("Picked Camera B");
            }

            if (pCamera)
            {
                if (pCamera->IsMouseOverElement(kMouseCoords, GetGame()->m_characterA->m_sprite))
                {
                    WriteInConsole("Picked Character A");
                }
                if (pCamera->IsMouseOverElement(kMouseCoords, GetGame()->m_characterB->m_sprite))
                {
                    WriteInConsole("Picked Character B");
                }
            }
        }
    }

    return true;
}

}   //namespace demoproject
