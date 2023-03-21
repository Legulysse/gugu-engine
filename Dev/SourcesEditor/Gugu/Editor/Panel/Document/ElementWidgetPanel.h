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
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementWidgetPanel : public DocumentPanel
{
public:

    ElementWidgetPanel(ElementWidget* ressource);
    virtual ~ElementWidgetPanel();

protected:

    virtual void UpdatePanelImpl(const DeltaTime& dt) override;

private:

    RenderViewport* m_renderViewport;
    float m_zoomFactor;
};

}   //namespace gugu
