////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "RendererDemo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

RendererDemo::RendererDemo()
{
    m_renderTarget_Main = new sf::RenderTexture;
    m_renderTarget_Refraction = new sf::RenderTexture;
    m_shader_Final = new sf::Shader;

    m_renderTarget_Main->create(400, 300);
    m_renderTarget_Main->setSmooth(true);
    m_renderTarget_Refraction->create(400, 300);
    m_renderTarget_Refraction->setSmooth(true);
    
    m_shader_Final->loadFromFile(GetResources()->GetResourcePathName("RenderRefraction.frag"), sf::Shader::Fragment);
}

RendererDemo::~RendererDemo()
{
    SafeDelete(m_renderTarget_Main);
    SafeDelete(m_renderTarget_Refraction);
    SafeDelete(m_shader_Final);
}

void RendererDemo::RenderWindow(FrameInfos& _pFrameInfos, Window* _pWindow, Camera* _pCamera)
{
    sf::RenderTarget* pTarget = _pWindow->GetSFRenderWindow();

    //Handle resize
    sf::Vector2u kSize = pTarget->getSize();
    if (m_renderTarget_Main->getSize() != kSize)
    {
        m_renderTarget_Main->create(kSize.x, kSize.y);
        m_renderTarget_Refraction->create(kSize.x, kSize.y);
    }

    //Main
    RenderPass kRenderPassMain;
    kRenderPassMain.pass = DEMO_RENDERPASS_MAIN;
    kRenderPassMain.target = m_renderTarget_Main;
    kRenderPassMain.frameInfos = &_pFrameInfos;

    m_renderTarget_Main->clear(sf::Color(0, 0, 0, 0));
    Render(kRenderPassMain, _pCamera, _pWindow->GetUINode());
    m_renderTarget_Main->display();
        
    //Refractive (Mouse Twirl)
    RenderPass kRenderPassRefractive;
    kRenderPassRefractive.pass = DEMO_RENDERPASS_REFRACTION;
    kRenderPassRefractive.target = m_renderTarget_Refraction;
    kRenderPassRefractive.frameInfos = &_pFrameInfos;

    m_renderTarget_Refraction->clear(sf::Color(0, 0, 0, 255));
    Render(kRenderPassRefractive, _pCamera, _pWindow->GetUINode());
    Render(kRenderPassRefractive, _pCamera, _pWindow->GetMouseNode());
    m_renderTarget_Refraction->display();
    
    //Final
    m_shader_Final->setUniform("texture_main", m_renderTarget_Main->getTexture());
    m_shader_Final->setUniform("texture_refractive", m_renderTarget_Refraction->getTexture());
    
    sf::Sprite m_pFullscreenQuad;
    m_pFullscreenQuad.setTexture(m_renderTarget_Main->getTexture());
    pTarget->draw(m_pFullscreenQuad, sf::RenderStates(m_shader_Final));

    //Mouse
    RenderPass kRenderPassUI;
    kRenderPassUI.pass = DEMO_RENDERPASS_MAIN;
    kRenderPassUI.target = pTarget;
    kRenderPassUI.frameInfos = &_pFrameInfos;

    Render(kRenderPassUI, _pCamera, _pWindow->GetMouseNode());
}

}   //namespace demoproject
