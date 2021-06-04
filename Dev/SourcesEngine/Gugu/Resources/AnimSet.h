#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Math/Vector2.h"

#include <vector>
#include <map>
#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Texture;
    class ImageSet;
    class SubImage;
    class AnimSet;
    class Animation;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
//----------------------------------------------
// AnimationFrame
// Single Frame used by an Animation.
// May use a SubImage from the owner's ImageSet or a Texture.

class AnimationFrame
{
public:

    AnimationFrame  (Animation* _pAnimation);
    ~AnimationFrame ();

    Animation*  GetAnimation() const;

    void        SetTexture(Texture* _pTexture);
    Texture*    GetTexture() const;

    void        SetSubImage(SubImage* _pSubImage);
    SubImage*   GetSubImage() const;

    void        SetDuration(float _fDuration);
    float       GetDuration() const;

    void        SetEvents(const std::string& _strEvents);
    std::string GetEvents() const;

    void            SetOrigin(const sf::Vector2f _kOrigin);
    sf::Vector2f    GetOrigin() const;
    
    void            SetMoveOffset(const sf::Vector2f _kMoveOffset);
    sf::Vector2f    GetMoveOffset() const;

protected:

    Animation*      m_animation;

    Texture*        m_texture;
    SubImage*       m_subImage;
    float           m_duration;
    std::string     m_events;
    sf::Vector2f    m_origin;
    sf::Vector2f    m_moveOffset;
};

//----------------------------------------------
// Animation

class Animation
{
public:

    Animation   (AnimSet* _pAnimSet, const std::string& _strName);
    ~Animation  ();

    AnimSet*        GetAnimSet() const;

    void            SetName (const std::string& _strName);
    std::string     GetName () const;

    AnimationFrame* AddFrame        ();
    void            DeleteFrame     (AnimationFrame* _pFrame);

    AnimationFrame* GetFrame        (size_t _uiIndex) const;
    size_t          GetFrameIndex   (AnimationFrame* _pFrame) const;

    void            GetFrames       (std::vector<AnimationFrame*>& _vecFrames) const;
    size_t          GetFrameCount  () const;

protected:

    AnimSet* m_animSet;

    std::string m_name;
    std::vector<AnimationFrame*> m_frames;
};

//----------------------------------------------
// AnimSet
// Set of Animations available for an Entity.

class AnimSet : public Resource
{
public:

    AnimSet ();
    ~AnimSet();

    Animation*  AddAnimation        (const std::string& _strName);
    void        DeleteAnimation     (Animation* _pAnimation);

    Animation*  GetAnimation        (const std::string& _strName) const;
    void        GetAnimations       (std::vector<Animation*>& _vecAnimations) const;
    void        GetAnimationNames   (std::vector<std::string>& _vecAnimationNames) const;

    void        SetImageSet (ImageSet* _pImageSet);
    ImageSet*   GetImageSet () const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool    LoadFromFile() override;
    virtual bool    SaveToFile() override;

protected:

    ImageSet* m_imageSet;
    std::vector<Animation*> m_animations;
};

}   // namespace gugu
