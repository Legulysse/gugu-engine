#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class Drawable;
    class RenderTexture;
}

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class RenderViewport
{
public:

    RenderViewport();
    ~RenderViewport();

    void BeginRender();
    void Render(const sf::Drawable* drawable);
    void FinalizeRender();

    void SetSize(Vector2u size);

    Element* GetRoot() const;

private:

    Vector2u m_size;
    sf::RenderTexture* m_renderTexture;
    Element* m_root;
};

}   //namespace gugu
