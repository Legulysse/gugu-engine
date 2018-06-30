#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "SFML/Graphics/Rect.hpp"

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
#define GUGU_RENDERPASS_DEFAULT 0x0001  //The Console is rendered with this flag	//TODO: passe specifique pour les infos de debug et la console

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct FrameInfos
{
    int statDrawCalls = 0;
};

struct RenderPass
{
	RenderPass()
	{
        frameInfos = nullptr;
		target = nullptr;
		pass = 0;

        statRenderedSprites = 0;
        statRenderedTexts = 0;
        statRenderedDrawables = 0;
	}

    FrameInfos* frameInfos;
	sf::RenderTarget* target;
	int pass;
    sf::FloatRect rectViewport;  // Pre-computed viewport

    int statRenderedSprites;
    int statRenderedTexts;
    int statRenderedDrawables;
};

class Renderer
{
public:

            Renderer    () {}
    virtual ~Renderer   () {}
    
    virtual void RenderLevel(FrameInfos* _pFrameInfos, Window* _pWindow, Camera* _pCamera, Level* _pLevel);
    virtual void RenderWindow(FrameInfos* _pFrameInfos, Window* _pWindow, Camera* _pCamera);

protected:

    // If Camera is not null, it will override the RenderTarget view and viewport
    void Render(Element* _pRoot, Camera* _pCamera, RenderPass& _kRenderPass);
};

}   // namespace gugu
