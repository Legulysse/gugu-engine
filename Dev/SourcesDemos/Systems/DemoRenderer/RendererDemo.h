#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class RenderTexture;
    class Shader;
    class Sprite;
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

    virtual void RenderScene(gugu::FrameInfos* frameInfos, gugu::Window* window, gugu::Scene* scene, gugu::Camera* camera) override;
    virtual void RenderWindow(gugu::FrameInfos* frameInfos, gugu::Window* window, gugu::Camera* camera) override;

private:
    
    sf::RenderTexture* m_renderTarget_Main;
    sf::RenderTexture* m_renderTarget_Refraction;
    sf::Sprite* m_fullscreenQuad;
    sf::Shader* m_shader_Final;
};

}   //namespace demoproject
