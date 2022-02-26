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
    class ImageSet;
    class RenderViewport;
    class Element;
    class ElementSFDrawable;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ImageSetPanel : public DocumentPanel
{
public:

    ImageSetPanel(const std::string& resourceID);
    virtual ~ImageSetPanel();

    virtual void UpdatePanel(const gugu::DeltaTime& dt) override;
    virtual void UpdateProperties(const gugu::DeltaTime& dt) override;
    virtual bool Save() override;

private:

    void CreateGizmo();
    void UpdateGizmo();

    void OnDragGizmoEdge(Element* edge, Vector2f position);

private:

    ImageSet* m_imageSet;
    int m_selectedIndex;

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
