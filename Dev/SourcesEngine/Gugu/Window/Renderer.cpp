////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Renderer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Scene/Scene.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void Renderer::RenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera)
{
    DefaultRenderScene(frameInfos, window, scene, camera);
}

void Renderer::RenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera)
{
    DefaultRenderWindow(frameInfos, window, camera);
}

void Renderer::DefaultRenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera)
{
    if (!window || !scene)
        return;

    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = window->GetSFRenderWindow();
    renderPass.frameInfos = frameInfos;

    RenderElementHierarchy(renderPass, scene->GetRootNode(), camera);
}

void Renderer::DefaultRenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera)
{
    if (!window)
        return;

    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = window->GetSFRenderWindow();
    renderPass.frameInfos = frameInfos;

    RenderElementHierarchy(renderPass, window->GetUINode(), camera);
    RenderElementHierarchy(renderPass, window->GetMouseNode(), camera);
}

sf::FloatRect Renderer::ComputeViewport(const sf::View& view)
{
    sf::FloatRect viewport;

    // Pre-compute viewport's rect (real size, not a [0, 1] range) (will be used by Elements to check if they should be drawn or not)
    viewport.position.x = view.getCenter().x - view.getSize().x / 2.f;
    viewport.position.y = view.getCenter().y - view.getSize().y / 2.f;
    viewport.size.x = view.getSize().x;
    viewport.size.y = view.getSize().y;

    return viewport;
}

void Renderer::RenderElementHierarchy(RenderPass& renderPass, Element* root, Camera* camera)
{
    if (!root)
        return;

    // Apply Camera View if needed.
    sf::View backupView;

    if (camera)
    {
        backupView = renderPass.target->getView();
        renderPass.target->setView(camera->GetSFView());
    }

    // Compute Viewport.
    renderPass.rectViewport = ComputeViewport(renderPass.target->getView());

#if 0
    // Debug off-screen rendering.
    renderPass.rectViewport.position.x += 50.f;
    renderPass.rectViewport.position.y += 50.f;
    renderPass.rectViewport.size.x -= 100.f;
    renderPass.rectViewport.size.y -= 100.f;
#endif

    // Render hierarchy.
    root->Render(renderPass, sf::Transform());

    // Restore View if needed.
    if (camera)
    {
        renderPass.target->setView(backupView);
    }
}

void DefaultRenderer::RenderScene(FrameInfos* frameInfos, Window* window, Scene* scene, Camera* camera)
{
    DefaultRenderScene(frameInfos, window, scene, camera);
}

void DefaultRenderer::RenderWindow(FrameInfos* frameInfos, Window* window, Camera* camera)
{
    DefaultRenderWindow(frameInfos, window, camera);
}

}   // namespace gugu
