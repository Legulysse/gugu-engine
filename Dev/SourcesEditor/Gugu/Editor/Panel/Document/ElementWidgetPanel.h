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

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;
    virtual void UpdateHierarchyImpl(const DeltaTime& dt) override;
    virtual void UpdatePropertiesImpl(const DeltaTime& dt) override;

    void DisplayTreeNode(Element* node, int itemFlags);
    void HandleContextMenu(Element* node);

    virtual bool SaveToFileImpl() override;
    virtual bool LoadFromStringImpl(const std::string& value) override;
    virtual bool SaveToStringImpl(std::string& result) override;

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;

    ElementWidget* m_elementWidget;
    Element* m_widgetRoot;
    Element* m_selectedElement;
};

}   //namespace gugu
