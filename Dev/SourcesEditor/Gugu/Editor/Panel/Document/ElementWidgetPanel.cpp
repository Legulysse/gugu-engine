////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/ElementWidget.h"
//#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidgetPanel::ElementWidgetPanel(ElementWidget* resource)
    : DocumentPanel(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);

    //ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    //sprite->SetTexture(m_texture);

    //m_renderViewport->SetSize(Vector2u(sprite->GetSize()));
}

ElementWidgetPanel::~ElementWidgetPanel()
{
    SafeDelete(m_renderViewport);
}

void ElementWidgetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 1.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

}   //namespace gugu
