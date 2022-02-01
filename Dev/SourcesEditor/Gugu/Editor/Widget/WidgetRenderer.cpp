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
    RenderPass renderPass;
    renderPass.pass = GUGU_RENDERPASS_DEFAULT;
    renderPass.target = renderTexture;

    RenderElementHierarchy(renderPass, root, nullptr);
}

}   // namespace gugu
