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
    class WidgetRenderer;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class RenderViewport
{
public:

    RenderViewport(bool fillAvailableArea);
    ~RenderViewport();

    void ImGuiBegin();
    void ImGuiEnd();

    void SetSize(Vector2u size);
    const Vector2u& GetSize() const;

    void SetZoom(float zoomMultiplier);
    float GetZoom() const;

    void SetShowBounds(bool showBounds);

    Vector2f GetMousePickedPosition() const;
    Vector2f GetPickedPosition(const Vector2i& pixelCoords) const;

    Element* GetRoot() const;

private:

    WidgetRenderer* m_renderer;
    sf::RenderTexture* m_renderTexture;
    Element* m_root;

    Vector2u m_size;
    Vector2f m_renderSize;
    float m_zoomMultiplier;
    bool m_fillAvailableArea;
    bool m_showBounds;
};

}   //namespace gugu
