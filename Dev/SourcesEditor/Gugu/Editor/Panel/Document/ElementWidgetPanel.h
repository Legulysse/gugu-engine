#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/Document/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class RenderViewport;
    class ElementWidget;
    class ElementData;
    struct ElementDataContext;
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidgetPanel : public DocumentPanel
{
public:

    ElementWidgetPanel(ElementWidget* ressource);
    virtual ~ElementWidgetPanel();

private:

    virtual void OnUndoRedo() override;

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdateHierarchyImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

    void DisplayTreeNode(ElementData* node, int itemFlags);
    void HandleContextMenu(ElementData* node);

    void RebuildWidgetHierarchy();

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    ElementWidget* m_elementWidget;
    ElementDataContext* m_dataContext;
    ElementData* m_widgetRootData;
    Element* m_widgetRootElement;
    ElementData* m_selectedElementData;
    Element* m_selectedElement;
};

}   //namespace gugu
