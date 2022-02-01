////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "RendererDemo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
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
    m_fullscreenQuad = new sf::Sprite;

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
    SafeDelete(m_fullscreenQuad);
}

void RendererDemo::RenderScene(FrameInfos& frameInfos, Window* window, Scene* scene, Camera* camera)
{
    DefaultRenderScene(frameInfos, window, scene, camera);
}

void RendererDemo::RenderWindow(FrameInfos& frameInfos, Window* window, Camera* camera)
{
    sf::RenderTarget* target = window->GetSFRenderWindow();

    //Handle resize
    Vector2u size = target->getSize();
    if (m_renderTarget_Main->getSize() != size)
    {
        m_renderTarget_Main->create(size.x, size.y);
        m_renderTarget_Refraction->create(size.x, size.y);
    }

    //Main
    RenderPass renderPassMain;
    renderPassMain.pass = DEMO_RENDERPASS_MAIN;
    renderPassMain.target = m_renderTarget_Main;
    renderPassMain.frameInfos = &frameInfos;

    m_renderTarget_Main->clear(sf::Color(0, 0, 0, 0));
    RenderElementHierarchy(renderPassMain, window->GetUINode(), camera);
    m_renderTarget_Main->display();
        
    //Refractive (Mouse Twirl)
    RenderPass renderPassRefractive;
    renderPassRefractive.pass = DEMO_RENDERPASS_REFRACTION;
    renderPassRefractive.target = m_renderTarget_Refraction;
    renderPassRefractive.frameInfos = &frameInfos;

    m_renderTarget_Refraction->clear(sf::Color(0, 0, 0, 255));
    RenderElementHierarchy(renderPassRefractive, window->GetUINode(), camera);
    RenderElementHierarchy(renderPassRefractive, window->GetMouseNode(), camera);
    m_renderTarget_Refraction->display();
    
    //Final
    m_shader_Final->setUniform("texture_main", m_renderTarget_Main->getTexture());
    m_shader_Final->setUniform("texture_refractive", m_renderTarget_Refraction->getTexture());
    
    m_fullscreenQuad->setTexture(m_renderTarget_Main->getTexture());
    target->draw(*m_fullscreenQuad, sf::RenderStates(m_shader_Final));

    //Mouse
    RenderPass renderPassUI;
    renderPassUI.pass = DEMO_RENDERPASS_MAIN;
    renderPassUI.target = target;
    renderPassUI.frameInfos = &frameInfos;

    RenderElementHierarchy(renderPassUI, window->GetMouseNode(), camera);
}

}   //namespace demoproject
