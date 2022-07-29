////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/TexturePanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

#include <imgui.h>
#include <imgui_stdlib.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

TexturePanel::TexturePanel(Texture* resource)
    : DocumentPanel(resource)
    , m_texture(resource)
    , m_renderViewport(nullptr)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);

    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetTexture(m_texture);

    m_renderViewport->SetSize(Vector2u(sprite->GetSize()));
}

TexturePanel::~TexturePanel()
{
    SafeDelete(m_renderViewport);
}

void TexturePanel::UpdatePanelImpl(const DeltaTime& dt)
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

void TexturePanel::UpdateProperties(const gugu::DeltaTime& dt)
{
}

}   //namespace gugu
