////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSpriteAnimated::ElementSpriteAnimated()
{
    m_isStepOnUpdate = true;
}

ElementSpriteAnimated::~ElementSpriteAnimated()
{
}

void ElementSpriteAnimated::Step(const DeltaTime& dt)
{
    ElementSprite::Step(dt);

    if (!IsAnimationPaused() && !m_isStepOnUpdate)
    {
        StepAnimation(dt);
    }
}

void ElementSpriteAnimated::Update(const DeltaTime& dt)
{
    ElementSprite::Update(dt);

    if (!IsAnimationPaused() &&  m_isStepOnUpdate)
    {
        StepAnimation(dt);
    }
}

void ElementSpriteAnimated::InitCurrentAnimationFrame()
{
    AnimationFrame* pCurrentFrame = GetAnimationFrame();

    if (pCurrentFrame)
    {
        if (pCurrentFrame->GetSubImage())
        {
            SetSubImage(pCurrentFrame->GetSubImage());
        }
        else if (pCurrentFrame->GetTexture())
        {
            SetTexture(pCurrentFrame->GetTexture());
        }

        if (m_originFromAnimation)
        {
            SetOrigin(pCurrentFrame->GetOrigin());
        }
        
        if (m_moveFromAnimation)
        {
            sf::Vector2f kMove = pCurrentFrame->GetMoveOffset();
            if (m_flipH)
                kMove.x = -kMove.x;
            if (m_flipV)
                kMove.y = -kMove.y;
            Move(kMove);
        }
    }
}

}   // namespace gugu
