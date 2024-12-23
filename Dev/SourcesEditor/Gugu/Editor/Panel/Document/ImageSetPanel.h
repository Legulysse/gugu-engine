#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

#include "Gugu/Resources/EnumsResources.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class ImageSet;
    class Element;
    class ElementSprite;
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

    virtual void OnUndoRedo() override;

    void CreateGizmo();
    void UpdateGizmo();

    void OnDragGizmoEdge(Element* edge, Vector2f position);

    void RefreshSpriteTexture();

    void OnAddSubImage();
    void OnRemoveSubImage();
    void OnRemoveAllSubImages();
    void OnGenerateSubImages();

    void GenerateSubImagesFromCount(int columnCount, int rowCount);
    void GenerateSubImagesFromSize(const Vector2i& itemSize, const Vector2i& itemOffset);

    void OnResourceEvent(const Resource* resource, EResourceEvent event, const Resource* dependency);

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    ImageSet* m_imageSet;
    int m_selectedIndex;
    bool m_forceScrollToSelectedIndex;

    std::string m_frameNameTemplate;

    ElementSprite* m_sprite;

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
