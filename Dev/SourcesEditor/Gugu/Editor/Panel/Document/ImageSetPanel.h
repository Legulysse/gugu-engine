#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class ImageSet;
    class Element;
    class ElementSFDrawable;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImageSetPanel : public DocumentPanel
{
public:

    ImageSetPanel(ImageSet* resource);
    virtual ~ImageSetPanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

private:

    void CreateGizmo();
    void UpdateGizmo();

    void OnDragGizmoEdge(Element* edge, Vector2f position);

private:

    RenderViewport* m_renderViewport;

    ImageSet* m_imageSet;
    int m_selectedIndex;

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
