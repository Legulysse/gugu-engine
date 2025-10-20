#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/SceneActor.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerPlayer : public gugu::SceneActor
{
public:

    ControllerPlayer();
    virtual ~ControllerPlayer();
    
    void InitController(Character* _pCharacter, int _playerIndex);

    virtual void Update(const gugu::DeltaTime& dt) override;

public:

    int m_playerIndex;
    Character* m_character;
};

}   //namespace demoproject
