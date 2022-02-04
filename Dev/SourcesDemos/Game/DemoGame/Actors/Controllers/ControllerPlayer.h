#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/SceneActor.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class CharacterHero;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerPlayer : public gugu::SceneActor, public gugu::EventListener
{
public:

    ControllerPlayer();
    virtual ~ControllerPlayer();
    
    void InitControllerPlayer   (CharacterHero* _pCharacter);

    virtual void Step(const gugu::DeltaTime& dt) override;
    virtual void Update(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

public:

    CharacterHero* m_character;
    gugu::Vector2f m_moveDirection;
};

}   //namespace demoproject
