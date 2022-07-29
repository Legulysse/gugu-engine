////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/AnimSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/AnimSet.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AnimSetPanel::AnimSetPanel(AnimSet* resource)
    : DocumentPanel(resource)
    , m_animSet(resource)
    , m_renderViewport(nullptr)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);
}

AnimSetPanel::~AnimSetPanel()
{
    SafeDelete(m_renderViewport);
}

void AnimSetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    static float zoomFactor = 1.f;
    if (ImGui::SliderFloat("Zoom Factor", &zoomFactor, 1.f, 16.f))
    {
        m_renderViewport->SetZoom(zoomFactor);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

void AnimSetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
}

}   //namespace gugu
