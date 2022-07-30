////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Animation/SpriteAnimation.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/SystemUtility.h"

#include <sstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
SpriteAnimation::SpriteAnimation()
{
    m_sprite = nullptr;

    m_animSet = nullptr;
    m_animation = nullptr;

    m_originFromAnimation = false;
    m_moveFromAnimation = false;

    m_animLoop = true;
    m_animPause = false;
    m_animSpeed = 1.f;

    m_animIndexCurrent = 0;
    m_animDurationCurrent = 0.f;
}

SpriteAnimation::~SpriteAnimation()
{
    // TODO: I should also ensure we dont let a callback on the target sprite (I need a way to retrieve the right callback).
    // - It's not necessarily a problem since the callback does not keep pointers on the animation.
    GetAnimations()->RemoveAnimation(this);
}

void SpriteAnimation::SetSprite(ElementSprite* sprite)
{
    m_sprite = sprite;
}

ElementSprite* SpriteAnimation::GetSprite() const
{
    return m_sprite;
}

void SpriteAnimation::ChangeAnimSet(const std::string& _strFilePath)
{
    ChangeAnimSet(GetResources()->GetAnimSet(_strFilePath));
}

void SpriteAnimation::ChangeAnimSet(AnimSet* _pAnimSet)
{
    StopAnimation();

    m_animSet = _pAnimSet;
    OnAnimsetChanged();
}

bool SpriteAnimation::HasAnimation(const std::string& _strNameAnim) const
{
    return (m_animSet && m_animSet->GetAnimation(_strNameAnim));
}

void SpriteAnimation::StartAnimation(const std::string& _strNameAnim)
{
    StopAnimation();

    if (m_animSet)
        m_animation = m_animSet->GetAnimation(_strNameAnim);

    RestartAnimation();
}

void SpriteAnimation::RestartAnimation()
{
    if (m_animation)
    {
        m_animIndexCurrent  = 0;
        m_animDurationCurrent  = 0.f;

        SetCurrentFrame(m_animIndexCurrent);
    }
}

void SpriteAnimation::StopAnimation()
{
    m_animation = nullptr;
    m_animIndexCurrent  = 0;
    m_animDurationCurrent  = 0.f;

    //TODO: Call InitCurrentAnimationFrame() and force empty Sprites ?
}

bool SpriteAnimation::IsAnimationPlaying() const
{
    return (m_animSet && m_animation);
}

bool SpriteAnimation::IsAnimationPlaying(const std::string& _strNameAnim) const
{
    if (m_animSet && m_animation)
    {
        return m_animation->IsName(_strNameAnim);
    }
    return false;
}

bool SpriteAnimation::IsAnimationPaused() const
{
    return m_animPause;
}

void SpriteAnimation::SetAnimationPause(bool _bPause)
{
    m_animPause = _bPause;
}

void SpriteAnimation::ToggleAnimationPause()
{
    m_animPause = !m_animPause;
}

bool SpriteAnimation::IsAnimationLooped() const
{
    return m_animLoop;
}

void SpriteAnimation::SetAnimationLoop(bool _bLoop)
{
    m_animLoop = _bLoop;
}

float SpriteAnimation::GetAnimationSpeed() const
{
    return m_animSpeed;
}

void SpriteAnimation::SetAnimationSpeed(float _fSpeed)
{
    m_animSpeed = _fSpeed;
}

void SpriteAnimation::SetOriginFromAnimation(bool _bOriginFromAnimation)
{
    m_originFromAnimation = _bOriginFromAnimation;
}

void SpriteAnimation::SetMoveFromAnimation(bool _bMoveFromAnimation)
{
    m_moveFromAnimation = _bMoveFromAnimation;
}

void SpriteAnimation::AddEventCallback(const std::string& _strEvent, const Callback& callbackEvent)
{
    m_events[_strEvent] = callbackEvent;
}

void SpriteAnimation::SetCurrentFrame(size_t _uiIndex)
{
    if (m_animation && _uiIndex >= 0 && _uiIndex < m_animation->GetFrameCount())
    {
        m_animIndexCurrent = _uiIndex;

        AnimationFrame* pCurrentFrame = InitCurrentAnimationFrame();
        if (pCurrentFrame && !m_events.empty() && pCurrentFrame->HasEvents())
        {
            // Events frame start.
            const std::vector<std::string>& frameEvents = pCurrentFrame->GetEvents();
            for (const std::string& frameEvent : frameEvents)
            {
                auto kvp = m_events.find(frameEvent);
                if (kvp != m_events.end())
                {
                    // Fire callback.
                    kvp->second();
                }
            }
        }
    }
}

void SpriteAnimation::SetCurrentFrame(AnimationFrame* _pFrame)
{
    if (m_animation && _pFrame)
    {
        SetCurrentFrame(m_animation->GetFrameIndex(_pFrame));
    }
}

void SpriteAnimation::StepAnimation(const DeltaTime& dt)
{
    AnimationFrame* pCurrentFrame = GetAnimationFrame();
    if (pCurrentFrame && !m_animPause)
    {
        m_animDurationCurrent += dt.s() * m_animSpeed;

        while (!m_animPause && pCurrentFrame && pCurrentFrame->GetDuration() <= m_animDurationCurrent)
        {
            m_animDurationCurrent -= pCurrentFrame->GetDuration();

            if (m_animIndexCurrent < m_animation->GetFrameCount() - 1)
            {
                ++m_animIndexCurrent;
            }
            else
            {
                if (m_animLoop)
                {
                    m_animIndexCurrent = 0;
                }
                else
                {
                    StopAnimation();
                }
            }

            pCurrentFrame = GetAnimationFrame();
            if (pCurrentFrame)
            {
                SetCurrentFrame(pCurrentFrame);

                //Safe check
                if (pCurrentFrame->GetDuration() < Math::Epsilon3)
                    break;
            }
        }
    }
}

AnimSet* SpriteAnimation::GetAnimSet() const
{
    return m_animSet;
}

Animation* SpriteAnimation::GetAnimation() const
{
    return m_animation;
}

AnimationFrame* SpriteAnimation::GetAnimationFrame() const
{
    if (m_animation)
        return m_animation->GetFrame(m_animIndexCurrent);
    return nullptr;
}

void SpriteAnimation::OnAnimsetChanged()
{
}

AnimationFrame* SpriteAnimation::InitCurrentAnimationFrame()
{
    AnimationFrame* pCurrentFrame = GetAnimationFrame();

    if (pCurrentFrame)
    {
        if (pCurrentFrame->GetSubImage())
        {
            m_sprite->SetSubImage(pCurrentFrame->GetSubImage());
        }
        else if (pCurrentFrame->GetTexture())
        {
            m_sprite->SetTexture(pCurrentFrame->GetTexture());
        }

        if (m_originFromAnimation)
        {
            m_sprite->SetOrigin(pCurrentFrame->GetOrigin());
        }

        if (m_moveFromAnimation)
        {
            Vector2f kMove = pCurrentFrame->GetMoveOffset();
            if (m_sprite->GetFlipH())
                kMove.x = -kMove.x;
            if (m_sprite->GetFlipV())
                kMove.y = -kMove.y;
            m_sprite->Move(kMove);
        }
    }

    return pCurrentFrame;
}

}   // namespace gugu
