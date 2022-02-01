#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class RenderTexture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class WidgetRenderer : public Renderer
{
public:

    WidgetRenderer() {}
    virtual ~WidgetRenderer() {}

    void RenderWidget(sf::RenderTexture* renderTexture, Element* root);
};

}   // namespace gugu
