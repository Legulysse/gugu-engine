////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"

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
			if (m_flipY)
				kMove.x = -kMove.x;
			if (m_flipX)
				kMove.y = -kMove.y;
            Move(kMove);
        }
    }
}

}	// namespace gugu
