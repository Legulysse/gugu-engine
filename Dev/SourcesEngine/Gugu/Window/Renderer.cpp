////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Renderer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Scene/Scene.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void Renderer::DefaultRenderScene(FrameInfos& frameInfos, Window* window, Scene* scene, Camera* camera)
{
    if (!window || !scene)
        return;

    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = window->GetSFRenderWindow();
    renderPass.frameInfos = &frameInfos;

    RenderElementHierarchy(renderPass, scene->GetRootNode(), camera);
}

void Renderer::DefaultRenderWindow(FrameInfos& frameInfos, Window* window, Camera* camera)
{
    if (!window)
        return;

    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = window->GetSFRenderWindow();
    renderPass.frameInfos = &frameInfos;

    RenderElementHierarchy(renderPass, window->GetUINode(), camera);
    RenderElementHierarchy(renderPass, window->GetMouseNode(), camera);
}

void Renderer::RenderElementHierarchy(RenderPass& renderPass, Element* root, Camera* camera)
{
    if (!root)
        return;

    // Save View
    sf::View kView = renderPass.target->getView();
    
    // Apply Camera View
    if (camera)
        renderPass.target->setView(camera->GetSFView());

    // Pre-compute viewport's rect (real size, not a [0, 1] range) (will be used by Elements to check if they should be drawn or not)
    renderPass.rectViewport.left = renderPass.target->getView().getCenter().x - renderPass.target->getView().getSize().x / 2.f;
    renderPass.rectViewport.top = renderPass.target->getView().getCenter().y - renderPass.target->getView().getSize().y / 2.f;
    renderPass.rectViewport.width = renderPass.target->getView().getSize().x;
    renderPass.rectViewport.height = renderPass.target->getView().getSize().y;

    //if (true)
    //{
    //    // Debug off-screen rendering
    //    renderPass.rectViewport.left += 50.f;
    //    renderPass.rectViewport.top += 50.f;
    //    renderPass.rectViewport.width -= 100.f;
    //    renderPass.rectViewport.height -= 100.f;
    //}

    // Render
    root->Render(renderPass, sf::Transform());

    // Restore View
    renderPass.target->setView(kView);
}

void DefaultRenderer::RenderScene(FrameInfos& frameInfos, Window* window, Scene* scene, Camera* camera)
{
    DefaultRenderScene(frameInfos, window, scene, camera);
}

void DefaultRenderer::RenderWindow(FrameInfos& frameInfos, Window* window, Camera* camera)
{
    DefaultRenderWindow(frameInfos, window, camera);
}

}   // namespace gugu
