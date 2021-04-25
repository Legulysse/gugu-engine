#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class CharacterHero;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerPlayer : public gugu::Actor, public gugu::EventListener
{
public:

    ControllerPlayer();
    virtual ~ControllerPlayer();
    
    void InitControllerPlayer   (CharacterHero* _pCharacter);

    virtual void Step(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

public:

    CharacterHero* m_character;
};

}   //namespace demoproject
