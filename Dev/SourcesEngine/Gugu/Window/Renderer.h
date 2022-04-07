#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class Scene;
    class Camera;
    class Window;
}

namespace sf
{
    class View;
    class RenderTarget;
}

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_RENDERPASS_INVALID 0x0000
#define GUGU_RENDERPASS_DEFAULT 0x0001  //The Console is rendered with this flag    //TODO: passe specifique pour les infos de debug et la console ?

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct FrameInfos
{
    bool showBounds = false;
    sf::RectangleShape defaultBoundsShape;

    int statDrawCalls = 0;
    int statTriangles = 0;
};

struct RenderPass
{
    FrameInfos* frameInfos = nullptr;
    sf::RenderTarget* target = nullptr;
    int pass = GUGU_RENDERPASS_DEFAULT;
    sf::FloatRect rectViewport;  // Pre-computed viewport   //TODO: rename to express the culling usage.

    int statRenderedSprites = 0;
    int statRenderedTexts = 0;
    int statRenderedDrawables = 0;
};

class Renderer
{
public:

    Renderer() {}
    virtual ~Renderer() {}
    
    virtual void RenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera);
    virtual void RenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera);

    static sf::FloatRect ComputeViewport(const sf::View& view);

protected:

    void DefaultRenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera);
    void DefaultRenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera);

    // If Camera is not null, it will override the RenderTarget view and viewport
    void RenderElementHierarchy(RenderPass& renderPass, Element* root, Camera* camera);
};

class DefaultRenderer : public Renderer
{
public:

    DefaultRenderer() {}
    virtual ~DefaultRenderer() {}

    virtual void RenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera) override;
    virtual void RenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera) override;
};

}   // namespace gugu
