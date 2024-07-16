////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/TexturePanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/Memory.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

TexturePanel::TexturePanel(Texture* resource)
    : DocumentPanel(resource)
    , m_texture(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
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
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 0.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

void TexturePanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    ImGui::Text("Resolution: %u x %u", m_texture->GetSize().x, m_texture->GetSize().y);
}

bool TexturePanel::SaveStateImpl(std::string& result)
{
    // Currently texture edition is not handled.
    return false;
}

}   //namespace gugu
