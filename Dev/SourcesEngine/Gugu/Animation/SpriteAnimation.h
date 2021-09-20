#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"
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
    class ElementSprite;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SpriteAnimation
{
public:

    SpriteAnimation();
    virtual ~SpriteAnimation();

    void SetSprite(ElementSprite* sprite);
    ElementSprite* GetSprite() const;

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

    void    AddEventCallback(const std::string& _strEvent, Callback callbackEvent);

    void    StepAnimation           (const DeltaTime& dt);

    void    SetCurrentFrame         (size_t _uiIndex);
    void    SetCurrentFrame         (AnimationFrame* _pFrame);

    AnimSet*        GetAnimSet          () const;
    Animation*      GetAnimation        () const;
    AnimationFrame* GetAnimationFrame   () const;

protected:

    virtual void    OnAnimsetChanged();
    virtual void    InitCurrentAnimationFrame();

protected:

    ElementSprite* m_sprite;

    AnimSet*    m_animSet;
    Animation*  m_animation;

    bool        m_originFromAnimation;
    bool        m_moveFromAnimation;    //Root Motion

    bool        m_animLoop;
    bool        m_animPause;
    float       m_animSpeed;

    size_t      m_animIndexCurrent;
    float       m_animDurationCurrent;

    std::map<std::string, Callback> m_events;
};

}   // namespace gugu
