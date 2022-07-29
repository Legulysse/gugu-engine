////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Widget/WidgetRenderer.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

#include <SFML/Graphics/RenderTexture.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void WidgetRenderer::RenderWidget(sf::RenderTexture* renderTexture, Element* root)
{
    FrameInfos frameInfos;
    frameInfos.defaultBoundsShape.setOutlineThickness(-1.f);
    frameInfos.defaultBoundsShape.setOutlineColor(sf::Color(255, 0, 255, 200));
    frameInfos.defaultBoundsShape.setFillColor(sf::Color::Transparent);

    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = renderTexture;
    renderPass.frameInfos = &frameInfos;

    RenderElementHierarchy(renderPass, root, nullptr);
}

}   // namespace gugu
