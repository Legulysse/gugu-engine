#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Actor.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerPlayer : public gugu::Actor
{
public:

    ControllerPlayer();
    virtual ~ControllerPlayer();
    
    void InitController(Character* _pCharacter, int _playerIndex);

    virtual void Step(const gugu::DeltaTime& dt) override;

public:

    int m_playerIndex;
    Character* m_character;
};

}   //namespace demoproject
