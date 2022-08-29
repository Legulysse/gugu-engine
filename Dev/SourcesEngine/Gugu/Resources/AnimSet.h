#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Math/Vector2.h"

#include <vector>
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

    void RegisterEvents(const std::string& events);
    bool HasEvents() const;
    const std::vector<std::string>& GetEvents() const;
    bool FillEventString(std::string& events) const;

    void            SetOrigin(const Vector2f _kOrigin);
    const Vector2f& GetOrigin() const;
    
    void            SetMoveOffset(const Vector2f _kMoveOffset);
    const Vector2f& GetMoveOffset() const;

protected:

    Animation*      m_animation;

    Texture*        m_texture;
    SubImage*       m_subImage;
    float           m_duration;
    std::vector<std::string> m_events;
    Vector2f    m_origin;
    Vector2f    m_moveOffset;
};

//----------------------------------------------
// Animation

class Animation
{
public:

    Animation   (AnimSet* _pAnimSet, const std::string& _strName);
    ~Animation  ();

    AnimSet*        GetAnimSet() const;

    void            SetName (const std::string& name);
    bool            IsName(const std::string& name) const;
    const std::string& GetName () const;

    AnimationFrame* AddFrame        ();
    AnimationFrame* AddFrame        (size_t insertIndex);
    void            DeleteFrame     (AnimationFrame* _pFrame);

    AnimationFrame* GetFrame        (size_t _uiIndex) const;
    size_t          GetFrameIndex   (AnimationFrame* _pFrame) const;

    const std::vector<AnimationFrame*>& GetFrames() const;
    size_t GetFrameCount() const;

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

    Animation* GetAnimation(const std::string& _strName) const;
    Animation* GetAnimation(size_t index) const;

    const std::vector<Animation*>& GetAnimations() const;
    void GetAnimationNames(std::vector<std::string>& _vecAnimationNames) const;

    void        SetImageSet (ImageSet* _pImageSet);
    ImageSet*   GetImageSet () const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::vector<Resource*>& dependencies) const override;
    virtual void OnDependencyRemoved(const Resource* removedDependency) override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    ImageSet* m_imageSet;
    std::vector<Animation*> m_animations;
};

}   // namespace gugu
