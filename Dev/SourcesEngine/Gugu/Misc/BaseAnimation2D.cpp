////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Misc/BaseAnimation2D.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Utility/System.h"

#include <sstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
BaseAnimation2D::BaseAnimation2D()
{
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

BaseAnimation2D::~BaseAnimation2D()
{
    ClearStdMap(m_events);
}

void BaseAnimation2D::ChangeAnimSet(const std::string& _strFilePath)
{
    ChangeAnimSet(GetResources()->GetAnimSet(_strFilePath));
}

void BaseAnimation2D::ChangeAnimSet(AnimSet* _pAnimSet)
{
    StopAnimation();

    m_animSet = _pAnimSet;
    OnAnimsetChanged();
}

bool BaseAnimation2D::HasAnimation(const std::string& _strNameAnim) const
{
    return (m_animSet && m_animSet->GetAnimation(_strNameAnim));
}

void BaseAnimation2D::StartAnimation(const std::string& _strNameAnim, bool _bLoop, float _fSpeed)
{
    StopAnimation();

    if (m_animSet)
        m_animation = m_animSet->GetAnimation(_strNameAnim);

    m_animLoop = _bLoop;

    RestartAnimation();
}

void BaseAnimation2D::RestartAnimation()
{
    if (m_animation)
    {
        m_animPause            = false;

        m_animIndexCurrent  = 0;
        m_animDurationCurrent  = 0.f;

        SetCurrentFrame(m_animIndexCurrent);
    }
}

void BaseAnimation2D::StopAnimation()
{
    m_animation         = nullptr;
    m_animPause         = false;

    m_animIndexCurrent  = 0;
    m_animDurationCurrent  = 0.f;

    //TODO: Call InitCurrentAnimationFrame() and force empty Sprites ?
}

bool BaseAnimation2D::IsAnimationPlaying() const
{
    return (m_animSet && m_animation);
}

bool BaseAnimation2D::IsAnimationPlaying(const std::string& _strNameAnim) const
{
    if (m_animSet && m_animation)
    {
        return (m_animation->GetName() == _strNameAnim);
    }
    return false;
}

bool BaseAnimation2D::IsAnimationPaused() const
{
    return m_animPause;
}

void BaseAnimation2D::SetAnimationPause(bool _bPause)
{
    m_animPause = _bPause;
}

void BaseAnimation2D::ToggleAnimationPause()
{
    m_animPause = !m_animPause;
}

bool BaseAnimation2D::IsAnimationLooped() const
{
    return m_animLoop;
}

void BaseAnimation2D::SetAnimationLoop(bool _bLoop)
{
    m_animLoop = _bLoop;
}

float BaseAnimation2D::GetAnimationSpeed() const
{
    return m_animSpeed;
}

void BaseAnimation2D::SetAnimationSpeed(float _fSpeed)
{
    m_animSpeed = _fSpeed;
}

void BaseAnimation2D::SetOriginFromAnimation(bool _bOriginFromAnimation)
{
    m_originFromAnimation = _bOriginFromAnimation;
}

void BaseAnimation2D::SetMoveFromAnimation(bool _bMoveFromAnimation)
{
    m_moveFromAnimation = _bMoveFromAnimation;
}

void BaseAnimation2D::AddEventCallback(const std::string& _strEvent, Action* _pAction)
{
    m_events[_strEvent] = _pAction;
}

void BaseAnimation2D::SetCurrentFrame(size_t _uiIndex)
{
    if (m_animation && _uiIndex >= 0 && _uiIndex < m_animation->GetFrameCount())
    {
        m_animIndexCurrent = _uiIndex;

        InitCurrentAnimationFrame();

        AnimationFrame* pCurrentFrame = GetAnimationFrame();
        if (pCurrentFrame)
        {
            //Events frame start
            std::istringstream ss(pCurrentFrame->GetEvents());
            while (!ss.eof())
            {
                std::string field;
                std::getline(ss, field, '|');
                if (field.empty())
                    continue;

                auto kvp = m_events.find(field);
                if (kvp != m_events.end())
                {
                    kvp->second->Call();
                }
            }
        }
    }
}

void BaseAnimation2D::SetCurrentFrame(AnimationFrame* _pFrame)
{
    if (m_animation && _pFrame)
    {
        SetCurrentFrame(m_animation->GetFrameIndex(_pFrame));
    }
}

void BaseAnimation2D::StepAnimation(const DeltaTime& dt)
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
                if (pCurrentFrame->GetDuration() < Math::Epsilon)
                    break;
            }
        }
    }
}

AnimSet* BaseAnimation2D::GetAnimSet() const
{
    return m_animSet;
}

Animation* BaseAnimation2D::GetAnimation() const
{
    return m_animation;
}

AnimationFrame* BaseAnimation2D::GetAnimationFrame() const
{
    if (m_animation)
        return m_animation->GetFrame(m_animIndexCurrent);
    return nullptr;
}

void BaseAnimation2D::OnAnimsetChanged()
{
}

}   // namespace gugu
