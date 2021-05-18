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
    class Level;
    class Camera;
    class Window;
}

namespace sf
{
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
};

struct RenderPass
{
    FrameInfos* frameInfos = nullptr;
    sf::RenderTarget* target = nullptr;
    int pass = GUGU_RENDERPASS_DEFAULT;
    sf::FloatRect rectViewport;  // Pre-computed viewport

    int statRenderedSprites = 0;
    int statRenderedTexts = 0;
    int statRenderedDrawables = 0;
};

class Renderer
{
public:

            Renderer    () {}
    virtual ~Renderer   () {}
    
    virtual void RenderLevel(FrameInfos& _pFrameInfos, Window* _pWindow, Camera* _pCamera, Level* _pLevel);
    virtual void RenderWindow(FrameInfos& _pFrameInfos, Window* _pWindow, Camera* _pCamera);

protected:

    // If Camera is not null, it will override the RenderTarget view and viewport
    void Render(RenderPass& _kRenderPass, Camera* _pCamera, Element* _pRoot);
};

}   // namespace gugu
