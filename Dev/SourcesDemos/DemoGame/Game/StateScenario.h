#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/State.h"
#include "Gugu/Misc/EventListener.h"

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Action;
    class Element;
    class ElementText;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class StateScenario : public gugu::State, public gugu::EventListener
{
public:

    StateScenario();
    virtual ~StateScenario();

    virtual void Init() override;
    virtual void Release() override;

    virtual void Enter(State* _pStateFrom) override;
    virtual void Exit(State* _pStateTo) override;

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

private:

    void AddCharacterSheetAttribute(const std::string& label, gugu::ElementText*& textValue, float positionX, float positionY, float positionColumnX, gugu::Action* onClick);
    void RefreshCharacterSheet();

    enum class EButtonUpdateStat
    {
        AttackSpeed,
    };
    void OnButtonUpdateStat(EButtonUpdateStat button, int value);

private:

    gugu::Element* m_root;

    class ElementBar* m_staminaBar;
    gugu::ElementText* m_textStatus;

    gugu::Element* m_characterSheet;
    gugu::ElementText* m_textExperienceValue;
    gugu::ElementText* m_textAvailablePointsValue;
    gugu::ElementText* m_textHealthValue;
    gugu::ElementText* m_textStaminaValue;
    gugu::ElementText* m_textStaminaRecoveryValue;
    gugu::ElementText* m_textStaminaRecoveryDelayValue;
    gugu::ElementText* m_textAttackSpeedValue;
    gugu::ElementText* m_textAttackStaminaCostValue;
    gugu::ElementText* m_textWalkSpeedValue;
};

}   //namespace demoproject
