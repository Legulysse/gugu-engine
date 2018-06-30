#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Render/Renderer.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class RenderTexture;
    class Shader;
}

////////////////////////////////////////////////////////////////
// Macros

#define DEMO_RENDERPASS_MAIN        0x0001  // Same as GUGU_RENDERPASS_DEFAULT
#define DEMO_RENDERPASS_REFRACTION  0x0002

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class RendererDemo : public gugu::Renderer
{
public:

    RendererDemo();
    virtual ~RendererDemo();
    
    virtual void RenderWindow(gugu::FrameInfos* _pFrameInfos, gugu::Window* _pWindow, gugu::Camera* _pCamera) override;

private:
    
    sf::RenderTexture* m_renderTarget_Main;
    sf::RenderTexture* m_renderTarget_Refraction;
    sf::Shader* m_shader_Final;
};

}   //namespace demoproject
