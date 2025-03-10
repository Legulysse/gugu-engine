////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Widget/RenderViewport.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/WidgetRenderer.h"

#include "Gugu/Engine.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/Memory.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderTexture.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

RenderViewport::RenderViewport(bool fillAvailableArea)
    : m_renderer(nullptr)
    , m_renderTexture(nullptr)
    , m_root(nullptr)
    , m_size(100, 100)
    , m_zoomMultiplier(1.f)
    , m_fillAvailableArea(fillAvailableArea)
    , m_showBounds(false)
{
    m_renderer = new WidgetRenderer;
    m_renderTexture = new sf::RenderTexture;
    m_root = new Element;

    m_root->SetSize(Vector2f(m_size));
}

RenderViewport::~RenderViewport()
{
    SafeDelete(m_root);
    SafeDelete(m_renderTexture);
    SafeDelete(m_renderer);
}

void RenderViewport::ImGuiBegin()
{
    // Compute canvas size and area size.
    Vector2u canvas_sz((uint32)((float)m_size.x * m_zoomMultiplier), (uint32)((float)m_size.y * m_zoomMultiplier));
    canvas_sz.x = Max<uint32>(1, canvas_sz.x);
    canvas_sz.y = Max<uint32>(1, canvas_sz.y);

    Vector2u areaSize = m_fillAvailableArea ? Vector2u(0, 0) : canvas_sz;

    // Begin imgui area.
    ImGuiChildFlags child_flags = ImGuiChildFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("RenderViewport", areaSize, child_flags, window_flags);

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    Vector2f canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    //ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    //if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    //if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;

    //ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    //ImGuiIO& io = ImGui::GetIO();
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    //draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("RenderViewportInvisibleButton", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    //const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    //const bool is_active = ImGui::IsItemActive();   // Held
    ////const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
    ////const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    //const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
    //const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Little trick : draw an invisible button the size of the texture, then draw the texture all over it.
    ImGui::SetCursorScreenPos(canvas_p0);

    // Reset render target.
    m_renderTexture->setActive(true);
    if (m_renderTexture->getSize() != m_size)
    {
        m_renderTexture->create(m_size.x, m_size.y);

        sf::View view;
        view.reset(sf::FloatRect(Vector2f(0, 0), Vector2f(m_size)));
        m_renderTexture->setView(view);
    }

    m_renderSize = Vector2f(canvas_sz);
}

void RenderViewport::ImGuiEnd()
{
    // Prepare render.
    m_renderTexture->clear(sf::Color(128, 128, 128, 255));

    // Render scene.
    m_renderer->RenderWidget(m_renderTexture, m_root, m_showBounds);

    // Display.
    m_renderTexture->display();
    ImGui::Image(*m_renderTexture, m_renderSize);
    //m_renderTexture->setActive(false);

    // Finalize imgui area.
    ImGui::EndChild();
}

void RenderViewport::SetSize(Vector2u size)
{
    m_size = size;

    m_root->SetSize(Vector2f(m_size));
}

const Vector2u& RenderViewport::GetSize() const
{
    return m_size;
}

void RenderViewport::SetZoom(float zoomMultiplier)
{
    m_zoomMultiplier = zoomMultiplier;
}

float RenderViewport::GetZoom() const
{
    return m_zoomMultiplier;
}

void RenderViewport::SetShowBounds(bool showBounds)
{
    m_showBounds = showBounds;
}

bool RenderViewport::GetShowBounds() const
{
    return m_showBounds;
}

Vector2f RenderViewport::GetMousePickedPosition() const
{
    // Handle picking (should be used inside a viewport begin/end block).
    ImGuiIO& io = ImGui::GetIO();
    Vector2f canvas_p0 = ImGui::GetCursorScreenPos();

    // Consider mouse pixel position in texture area.
    Vector2f mouse_pos_in_canvas(io.MousePos.x - canvas_p0.x, io.MousePos.y - canvas_p0.y);

    return GetPickedPosition(Vector2i(mouse_pos_in_canvas));
}

Vector2f RenderViewport::GetPickedPosition(const Vector2i& pixelCoords) const
{
    // Consider zoom level.
    Vector2i pixelCoordsWithZoom = Vector2i(Vector2f(pixelCoords.x / m_zoomMultiplier, pixelCoords.y / m_zoomMultiplier));
    return m_renderTexture->mapPixelToCoords(pixelCoordsWithZoom, m_renderTexture->getView());
}

Element* RenderViewport::GetRoot() const
{
    return m_root;
}

}   //namespace gugu
