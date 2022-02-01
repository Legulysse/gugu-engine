#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class RenderTexture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class WidgetRenderer : public Renderer
{
public:

    WidgetRenderer() {}
    virtual ~WidgetRenderer() {}

    virtual void RenderScene(FrameInfos& frameInfos, Window* window, Scene* scene, Camera* camera) override;
    virtual void RenderWindow(FrameInfos& frameInfos, Window* window, Camera* camera) override;

    void RenderWidget(sf::RenderTexture* renderTexture, Element* root);
};

}   // namespace gugu
