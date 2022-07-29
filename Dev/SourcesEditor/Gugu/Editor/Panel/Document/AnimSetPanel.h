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
    class Animation;
    class AnimationFrame;
    class SpriteAnimation;
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
    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;
    bool m_autoPlay;

    AnimSet* m_animSet;
    SpriteAnimation* m_spriteAnimation;
    Animation* m_currentAnimation;
    AnimationFrame* m_currentFrame;
};

}   //namespace gugu
