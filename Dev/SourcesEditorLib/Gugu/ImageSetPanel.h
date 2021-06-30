#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class RenderViewport;
    class Element;
    class ElementSFDrawable;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImageSetPanel
{
public:

    ImageSetPanel();
    ~ImageSetPanel();

    void UpdatePanel(const gugu::DeltaTime& dt);
    void UpdateProperties(const gugu::DeltaTime& dt);

private:

    void CreateGizmo();
    void UpdateGizmo();

    void OnDragGizmoEdge(Element* edge, Vector2f position);

private:

    RenderViewport* m_renderViewport;

    ElementSFDrawable* m_gizmoCenter;
    ElementSFDrawable* m_gizmoEdgeTopLeft;
    ElementSFDrawable* m_gizmoEdgeTop;
    ElementSFDrawable* m_gizmoEdgeTopRight;
    ElementSFDrawable* m_gizmoEdgeRight;
    ElementSFDrawable* m_gizmoEdgeBottomRight;
    ElementSFDrawable* m_gizmoEdgeBottom;
    ElementSFDrawable* m_gizmoEdgeBottomLeft;
    ElementSFDrawable* m_gizmoEdgeLeft;

    bool m_isDraggingGizmo;
    Element* m_draggedGizmo;
    Vector2f m_gizmoOffsetGlobalPosition;
};

}   //namespace gugu
