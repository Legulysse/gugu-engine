#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class AnimSet;
    class ImageSet;
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

protected:

    virtual void OnUndoRedo() override;
    virtual void OnVisibilityChanged(bool visible) override;

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

    void OnAddAnimation();
    void OnRemoveAnimation();

    void UpdateMainImageSet(ImageSet* newImageSet);

    void CopyFrame(AnimationFrame* targetFrame, const AnimationFrame* referenceFrame);

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

    SpriteAnimation* m_spriteAnimation;
    ElementSprite* m_sprite;

    //std::set<std::string> m_openedAnimations;
};

}   //namespace gugu
