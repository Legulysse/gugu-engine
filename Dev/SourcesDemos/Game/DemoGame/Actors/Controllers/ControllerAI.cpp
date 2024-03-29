////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Actors/Controllers/ControllerAI.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Actors/Characters/Character.h"

#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

ControllerAI::ControllerAI()
{
    m_delay = 0;
}

ControllerAI::~ControllerAI()
{
}

void ControllerAI::Step(const DeltaTime& dt)
{
    SceneActor::Step(dt);

    m_delay -= dt.ms();
    if (ApproxInferiorOrEqualToZero(m_delay, math::Epsilon6))
    {
        m_delay = GetRandomf(100, 1200);

        m_direction.x = GetRandomf(-1.0, 1.0);
        m_direction.y = GetRandomf(-1.0, 1.0);
    }

    m_character->Move(m_direction, dt);
}

}   //namespace demoproject
