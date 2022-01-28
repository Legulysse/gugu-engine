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

void Renderer::RenderScene(FrameInfos& _pFrameInfos, Window* _pWindow, Camera* _pCamera, Scene* scene)
{
    if (scene)
    {
        RenderPass kRenderPass;
        kRenderPass.pass = GUGU_RENDERPASS_DEFAULT;
        kRenderPass.target = _pWindow->GetSFRenderWindow();
        kRenderPass.frameInfos = &_pFrameInfos;

        Render(kRenderPass, _pCamera, scene->GetRootNode());
    }
}

void Renderer::RenderWindow(FrameInfos& _pFrameInfos, Window* _pWindow, Camera* _pCamera)
{
    RenderPass kRenderPass;
    kRenderPass.pass = GUGU_RENDERPASS_DEFAULT;
    kRenderPass.target = _pWindow->GetSFRenderWindow();
    kRenderPass.frameInfos = &_pFrameInfos;

    Render(kRenderPass, _pCamera, _pWindow->GetUINode());
    Render(kRenderPass, _pCamera, _pWindow->GetMouseNode());
}

void Renderer::Render(RenderPass& _kRenderPass, Camera* _pCamera, Element* _pRoot)
{
    if (!_pRoot)
        return;

    // Save View
    sf::View kView = _kRenderPass.target->getView();
    
    // Apply Camera View
    if (_pCamera)
        _kRenderPass.target->setView(_pCamera->GetSFView());

    // Pre-compute viewport's rect (real size, not a [0, 1] range) (will be used by Elements to check if they should be drawn or not)
    _kRenderPass.rectViewport.left = _kRenderPass.target->getView().getCenter().x - _kRenderPass.target->getView().getSize().x / 2.f;
    _kRenderPass.rectViewport.top = _kRenderPass.target->getView().getCenter().y - _kRenderPass.target->getView().getSize().y / 2.f;
    _kRenderPass.rectViewport.width = _kRenderPass.target->getView().getSize().x;
    _kRenderPass.rectViewport.height = _kRenderPass.target->getView().getSize().y;

    //if (true)
    //{
    //    // Debug off-screen rendering
    //    _kRenderPass.rectViewport.left += 50.f;
    //    _kRenderPass.rectViewport.top += 50.f;
    //    _kRenderPass.rectViewport.width -= 100.f;
    //    _kRenderPass.rectViewport.height -= 100.f;
    //}

    // Render
    _pRoot->Render(_kRenderPass, sf::Transform());

    // Restore View
    _kRenderPass.target->setView(kView);
}

}   // namespace gugu
