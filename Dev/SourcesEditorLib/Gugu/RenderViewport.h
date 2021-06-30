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

    RenderViewport(bool fillAvailableArea);
    ~RenderViewport();

    void BeginRender();
    void FinalizeRender();

    void SetSize(Vector2u size);
    void SetZoom(float zoomMultiplier);

    Vector2f GetPickedPosition(const Vector2i& pixelCoords) const;

    Element* GetRoot() const;

private:

    Vector2u m_size;
    float m_zoomMultiplier;
    bool m_fillAvailableArea;

    sf::RenderTexture* m_renderTexture;
    Element* m_root;
};

}   //namespace gugu
