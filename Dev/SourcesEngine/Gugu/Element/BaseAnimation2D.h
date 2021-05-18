#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Action.h"
#include "Gugu/Core/DeltaTime.h"

#include <string>
#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class AnimSet;
    class Animation;
    class AnimationFrame;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class BaseAnimation2D
{
public:

    BaseAnimation2D();
    virtual ~BaseAnimation2D();

    void    ChangeAnimSet           (const std::string& _strFilePath);
    void    ChangeAnimSet           (AnimSet* _pAnimSet);

    bool    HasAnimation            (const std::string& _strNameAnim) const;

    void    StartAnimation          (const std::string& _strNameAnim, bool _bLoop = true, float _fSpeed = 1.f);
    void    RestartAnimation        ();
    void    StopAnimation           ();

    bool    IsAnimationPlaying      () const;
    bool    IsAnimationPlaying      (const std::string& _strNameAnim) const;

    bool    IsAnimationPaused       () const;
    void    SetAnimationPause       (bool _bPause);
    void    ToggleAnimationPause    ();

    bool    IsAnimationLooped       () const;
    void    SetAnimationLoop        (bool _bLoop);

    float   GetAnimationSpeed       () const;
    void    SetAnimationSpeed       (float _fSpeed);

    void    SetOriginFromAnimation  (bool _bOriginFromAnimation);
    void    SetMoveFromAnimation    (bool _bMoveFromAnimation);

    void    AddEventCallback(const std::string& _strEvent, Action* _pAction);

    void    StepAnimation           (const DeltaTime& dt);

    void    SetCurrentFrame         (size_t _uiIndex);
    void    SetCurrentFrame         (AnimationFrame* _pFrame);

    AnimSet*        GetAnimSet          () const;
    Animation*      GetAnimation        () const;
    AnimationFrame* GetAnimationFrame   () const;

protected:

    virtual void    OnAnimsetChanged();
    virtual void    InitCurrentAnimationFrame    () = 0;

protected:

    AnimSet*    m_animSet;
    Animation*  m_animation;

    bool        m_originFromAnimation;
    bool        m_moveFromAnimation;    //Root Motion

    bool        m_animLoop;
    bool        m_animPause;
    float       m_animSpeed;

    size_t      m_animIndexCurrent;
    float       m_animDurationCurrent;

    std::map<std::string, Action*> m_events;
};

}   // namespace gugu
