#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerPlayer : public gugu::Actor, public gugu::EventListener
{
public:

    ControllerPlayer();
    virtual ~ControllerPlayer();
    
    void InitControllerPlayer   (Character* _pCharacter);

    virtual void Step(const gugu::DeltaTime& dt) override;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent) override;

public:

    Character* m_character;
};

}   //namespace demoproject
