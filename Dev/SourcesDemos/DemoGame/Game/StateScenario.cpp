////////////////////////////////////////////////////////////////
// Header

#include "Game/StateScenario.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"
#include "Game/StateMenuMain.h"
#include "Actors/Characters/Character.h"
#include "Actors/Characters/CharacterHero.h"
#include "UI/ElementBar.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"
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

    //Init Level
    GetGame()->CreateScenario();

    //Root UI
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    // Character skills
    {
        ElementSprite* panelCharacterSkills = m_root->AddChild<ElementSprite>();
        panelCharacterSkills->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        panelCharacterSkills->SetUnifiedPosition(UDim2::POSITION_BOTTOM_LEFT + sf::Vector2f(210, 0.f));
        panelCharacterSkills->SetSubImage("uipack_rpg.imageset.xml", "panel_blue");
        panelCharacterSkills->SetSize(300, 100);

        m_skillSprite1 = panelCharacterSkills->AddChild<ElementSprite>();
        m_skillSprite1->SetTexture("fireball-red-2.png");
        m_skillSprite1->SetColor(sf::Color(110, 110, 110));
        m_skillSprite1->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        m_skillSprite1->SetPosition(64, 28);
        m_skillSprite1->SetSize(64, 64);

        ElementText* skillText1 = panelCharacterSkills->AddChild<ElementText>();
        skillText1->SetText("1");
        skillText1->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        skillText1->SetPosition(64, 2);

        m_skillCooldownText1 = m_skillSprite1->AddChild<ElementText>();
        m_skillCooldownText1->SetText("--");
        m_skillCooldownText1->SetFontColor(sf::Color::White);
        m_skillCooldownText1->SetUnifiedOrigin(UDim2::POSITION_CENTER);
        m_skillCooldownText1->SetUnifiedPosition(UDim2::POSITION_CENTER);
    }

    // Character bars
    {
        ElementSprite* panelCharacterBars = m_root->AddChild<ElementSprite>();
        panelCharacterBars->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        panelCharacterBars->SetUnifiedPosition(UDim2::POSITION_BOTTOM_LEFT);
        panelCharacterBars->SetSubImage("uipack_rpg.imageset.xml", "panel_blue");
        panelCharacterBars->SetSize(230, 110);

        float fSizeX = 200.f;
        float fSizeY = 16.f;
        float fPositionX = 16;
        float fPositionY = -16.f;
        float fOffsetX = 0.f;
        float fOffsetY = -20.f;

        m_healthBar = panelCharacterBars->AddChild<ElementBar>();
        m_healthBar->InitBar(ElementBar::BarColor::Red, 8.f);
        m_healthBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        m_healthBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
        m_healthBar->SetSize(fSizeX, fSizeY);

        fPositionX += fOffsetX;
        fPositionY += fOffsetY;

        m_staminaBar = panelCharacterBars->AddChild<ElementBar>();
        m_staminaBar->InitBar(ElementBar::BarColor::Green, 8.f);
        m_staminaBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        m_staminaBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
        m_staminaBar->SetSize(fSizeX, fSizeY);

        fPositionX += fOffsetX;
        fPositionY += fOffsetY;

        m_manaBar = panelCharacterBars->AddChild<ElementBar>();
        m_manaBar->InitBar(ElementBar::BarColor::Blue, 8.f);
        m_manaBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        m_manaBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
        m_manaBar->SetSize(fSizeX, fSizeY);

        fPositionX += fOffsetX;
        fPositionY += fOffsetY;

        ElementBar* pFaithBar = panelCharacterBars->AddChild<ElementBar>();
        pFaithBar->InitBar(ElementBar::BarColor::Yellow, 8.f);
        pFaithBar->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
        pFaithBar->SetUnifiedPosition(UDim2(0.f, fPositionX, 1.f, fPositionY));
        pFaithBar->SetSize(fSizeX, fSizeY);
    }

    // Status panel
    {
        ElementSprite* panelStatus = m_root->AddChild<ElementSprite>();
        panelStatus->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
        panelStatus->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT);
        panelStatus->SetSubImage("uipack_rpg.imageset.xml", "panel_blue");
        panelStatus->SetSize(180, 60);

        m_textStatus = panelStatus->AddChild<ElementText>();
        m_textStatus->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
        m_textStatus->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(15, 5));
    }

    // Character sheet
    {
        ElementSprite* characterSheetBackground = m_root->AddChild<ElementSprite>();
        characterSheetBackground->SetUnifiedOrigin(UDim2::POSITION_TOP_RIGHT);
        characterSheetBackground->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT);
        characterSheetBackground->SetSubImage("uipack_rpg.imageset.xml", "panel_beige");
        characterSheetBackground->SetSize(400, 600);

        m_characterSheet = characterSheetBackground;

        float positionX = 20.f;
        float positionY = 50.f;
        float positionOffsetYSmall = 25.f;
        float positionOffsetYBig = 45.f;
        float positionColumnX = 300.f;

        AddCharacterSheetAttribute("Experience", m_textExperienceValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYSmall;
        AddCharacterSheetAttribute("Available Points", m_textAvailablePointsValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYBig;
        AddCharacterSheetAttribute("Maximum Health", m_textHealthValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYBig;
        AddCharacterSheetAttribute("Maximum Stamina", m_textStaminaValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYSmall;
        AddCharacterSheetAttribute("Stamina Recovery", m_textStaminaRecoveryValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYSmall;
        AddCharacterSheetAttribute("Stamina Reco. Delay", m_textStaminaRecoveryDelayValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYBig;
        AddCharacterSheetAttribute("Maximum Mana", m_textManaValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYSmall;
        AddCharacterSheetAttribute("Mana Recovery", m_textManaRecoveryValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYBig;
        AddCharacterSheetAttribute("Walk Speed", m_textWalkSpeedValue, positionX, positionY, positionColumnX, nullptr);

        positionY += positionOffsetYBig;
        AddCharacterSheetAttribute("Attack Speed", m_textAttackSpeedValue, positionX, positionY, positionColumnX,
            std::bind(&StateScenario::OnButtonUpdateStat, this, EButtonUpdateStat::AttackSpeed, 1));

        positionY += positionOffsetYSmall;
        AddCharacterSheetAttribute("Stamina Cost", m_textAttackStaminaCostValue, positionX, positionY, positionColumnX, nullptr);
    }

    RefreshCharacterSheet();
}

void StateScenario::AddCharacterSheetAttribute(const std::string& label, gugu::ElementText*& textValue, float positionX, float positionY, float positionColumnX, Callback onClick)
{
    float positionColumnOffsetXLeftButton = -40.f;
    float positionColumnOffsetXRightButton = 40.f;

    ElementText* textLabel = m_characterSheet->AddChild<ElementText>();
    textLabel->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(positionX, positionY));
    textLabel->SetText(label);

    textValue = m_characterSheet->AddChild<ElementText>();
    textValue->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(positionColumnX, positionY));
    textValue->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
    textValue->SetText("---");

    if (onClick)
    {
        ElementSprite* buttonValueLeft = m_characterSheet->AddChild<ElementSprite>();
        buttonValueLeft->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(positionColumnX + positionColumnOffsetXLeftButton, positionY));
        buttonValueLeft->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        buttonValueLeft->SetSubImage("uipack_rpg.imageset.xml", "arrowBrown_left");

        ElementSprite* buttonValueRight = m_characterSheet->AddChild<ElementSprite>();
        buttonValueRight->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + sf::Vector2f(positionColumnX + positionColumnOffsetXRightButton, positionY));
        buttonValueRight->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
        buttonValueRight->SetSubImage("uipack_rpg.imageset.xml", "arrowBrown_right");
        buttonValueRight->AddInteractionFlag(EInteraction::Click);
        buttonValueRight->GetInteractions()->AddCallback(EInteraction::Click, onClick);
    }
}

void StateScenario::RefreshCharacterSheet()
{
    m_textExperienceValue->SetText(StringFormat("{0}", GetGame()->m_character->m_experience));
    m_textAvailablePointsValue->SetText(StringFormat("{0}", GetGame()->m_character->m_totalPoints - GetGame()->m_character->m_spentPoints));
    m_textHealthValue->SetText(StringFormat("{0}", GetGame()->m_character->m_maxLife));
    m_textStaminaValue->SetText(StringFormat("{0}", GetGame()->m_character->m_maxStamina));
    m_textStaminaRecoveryValue->SetText(StringFormat("{0}", GetGame()->m_character->m_staminaRecovery));
    m_textStaminaRecoveryDelayValue->SetText(StringFormat("{0}", GetGame()->m_character->m_staminaRecoveryDelay));
    m_textManaValue->SetText(StringFormat("{0}", GetGame()->m_character->m_maxMana));
    m_textManaRecoveryValue->SetText(StringFormat("{0}", GetGame()->m_character->m_manaRecovery));
    m_textAttackSpeedValue->SetText(StringFormat("{0}", GetGame()->m_character->m_attackSpeed));
    m_textAttackStaminaCostValue->SetText(StringFormat("---"));
    m_textWalkSpeedValue->SetText(StringFormat("{0}", GetGame()->m_character->m_walkSpeed));
}

void StateScenario::OnButtonUpdateStat(EButtonUpdateStat button, int value)
{
    if (button == EButtonUpdateStat::AttackSpeed)
    {
        if (value > 0)
        {
            GetGame()->m_character->m_attackSpeed += 10;
        }
        else if (value < 0)
        {
            GetGame()->m_character->m_attackSpeed -= 10;
        }
    }
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
    GetGame()->StepScenario(dt);
}

void StateScenario::Update(const DeltaTime& dt)
{
    CharacterHero* pCharacter = GetGame()->m_character;

    RefreshCharacterSheet();

    if (pCharacter->m_maxLife > 0.f)
    {
        m_healthBar->SetValue(pCharacter->m_currentLife, pCharacter->m_maxLife);
    }

    if (pCharacter->m_maxStamina > 0.f)
    {
        m_staminaBar->SetValue(pCharacter->m_currentStamina, pCharacter->m_maxStamina);
    }

    if (pCharacter->m_maxMana > 0.f)
    {
        m_manaBar->SetValue(pCharacter->m_currentMana, pCharacter->m_maxMana);
    }

    // SKills status
    if (pCharacter->m_skillCooldown > 0.f)
    {
        m_skillCooldownText1->SetVisible(true);
        m_skillCooldownText1->SetText(StringFormat("{0}s", ToString(pCharacter->m_skillCooldown, 1)));
        m_skillSprite1->SetColor(sf::Color(110, 110, 110));
    }
    else
    {
        if (m_skillCooldownText1->IsVisible())
        {
            m_skillCooldownText1->SetVisible(false);
            m_skillSprite1->SetColor(sf::Color::White);
        }
    }

    int floor = 0;
    int enemies = 0;
    GetGame()->GetStatus(floor, enemies);
    m_textStatus->SetText(StringFormat("Floor : {0}\nEnemies : {1}", floor, enemies));
}

bool StateScenario::OnSFEvent(const sf::Event& _oSFEvent)
{
    if (!EventListener::OnSFEvent(_oSFEvent))
        return false;

    ManagerInputs* inputs = GetInputs();

    if (inputs->IsInputReleased("Exit", _oSFEvent))
    {
        GetOwner()->ChangeState(new StateMenuMain);
        return false;
    }
    else if (inputs->IsInputReleased("ToggleCharacterSheet", _oSFEvent))
    {
        m_characterSheet->SetVisible(!m_characterSheet->IsVisible(false));
    }

    return true;
}

}   //namespace demoproject
