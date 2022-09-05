#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Resources/EnumsResources.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class AnimSet;
    class ImageSet;
    class SubImage;
    class Animation;
    class AnimationFrame;
    class SpriteAnimation;
    class ElementSprite;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class AnimSetPanel : public DocumentPanel
{
public:

    AnimSetPanel(AnimSet* resource);
    virtual ~AnimSetPanel();

    void OnSubImageRemoved(SubImage* subImage);
    void OnAllSubImagesRemoved(ImageSet* imageSet);

protected:

    virtual void OnUndoRedo() override;
    virtual void OnVisibilityChanged(bool visible) override;

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

    void SelectAnimation(Animation* animation);

    void OnAddAnimation();
    void OnRemoveAnimation();
    void OnGenerateAnimationFrames();

    void GenerateAnimationFramesFromDirectory(const std::string& path);
    void GenerateAnimationFramesFromImageSet(size_t from, size_t to);

    void ChangeMainImageSet(ImageSet* newImageSet);
    void CopyFrame(AnimationFrame* targetFrame, const AnimationFrame* referenceFrame);  //TODO: invert from/to.

    void OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency);

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;
    float m_speedFactor;
    bool m_autoPlay;
    bool m_originFromAnimation;
    bool m_moveFromAnimation;
    bool m_flipH;

    AnimSet* m_animSet;
    Animation* m_currentAnimation;
    AnimationFrame* m_currentFrame;

    float m_defaultDuration;

    SpriteAnimation* m_spriteAnimation;
    ElementSprite* m_sprite;

    //std::set<std::string> m_openedAnimations;
};

}   //namespace gugu
