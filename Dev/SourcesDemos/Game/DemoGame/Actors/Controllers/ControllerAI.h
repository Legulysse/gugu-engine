#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/SceneActor.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class Character;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ControllerAI : public gugu::SceneActor
{
public:

    ControllerAI();
    virtual ~ControllerAI();
    
    virtual void Step(const gugu::DeltaTime& dt) override;

public:

    Character* m_character;

    gugu::Vector2f m_direction;
    float m_delay;
};

}   //namespace demoproject
