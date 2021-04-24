////////////////////////////////////////////////////////////////
// Header

#include "Game/StateScenario.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"
#include "Game/StateMenuMain.h"
#include "Actors/Characters/Character.h"
#include "UI/ElementBar.h"

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
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    //LifeBar test
    float fSizeX = 200.f;
    float fSizeY = 16.f;
    float fPositionX = 8.f;
    float fPositionY = -8.f;
    float fOffsetX = 0.f;
    float fOffsetY = -20.f;

    ElementBar* pLifeBar = m_root->AddChild<ElementBar>();
    pLifeBar->InitBar(ElementBar::BarColor::Red, 8.f);
    pLifeBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    pLifeBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
    pLifeBar->SetSize(fSizeX, fSizeY);

    fPositionX += fOffsetX;
    fPositionY += fOffsetY;

    m_staminaBar = m_root->AddChild<ElementBar>();
    m_staminaBar->InitBar(ElementBar::BarColor::Green, 8.f);
    m_staminaBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    m_staminaBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
    m_staminaBar->SetSize(fSizeX, fSizeY);

    fPositionX += fOffsetX;
    fPositionY += fOffsetY;

    ElementBar* pManaBar = m_root->AddChild<ElementBar>();
    pManaBar->InitBar(ElementBar::BarColor::Blue, 8.f);
    pManaBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    pManaBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
    pManaBar->SetSize(fSizeX, fSizeY);

    fPositionX += fOffsetX;
    fPositionY += fOffsetY;

    ElementBar* pFaithBar = m_root->AddChild<ElementBar>();
    pFaithBar->InitBar(ElementBar::BarColor::Yellow, 8.f);
    pFaithBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    pFaithBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
    pFaithBar->SetSize(fSizeX, fSizeY);

    // Status panel
    m_textStatus = m_root->AddChild<ElementText>();
    m_textStatus->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    m_textStatus->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(10, 10));

    //Init Level
    GetGame()->CreateScenario();
}

void StateScenario::Release()
{
    GetGame()->ClearScenario();

    SafeDelete(m_root);
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
    Character* pCharacter = GetGame()->m_character;

    if (pCharacter->m_maxStamina > 0.f)
    {
        m_staminaBar->SetValue(pCharacter->m_currentStamina, pCharacter->m_maxStamina);
    }

    int level = 0;
    int enemies = 0;
    GetGame()->GetStatus(level, enemies);
    m_textStatus->SetText(StringFormat("Level : {0}\nEnemies : {1}", level, enemies));
}

bool StateScenario::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerConfig* pConfig = GetConfig();

    if (pConfig->IsInputReleased("Exit", _oSFEvent))
    {
        GetOwner()->ChangeState(new StateMenuMain);
        return false;
    }

    return true;
}

}   //namespace demoproject
