////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/ImageSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/RenderViewport.h"

#include "Gugu/Engine.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <imgui.h>
#include <imgui_internal.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImageSetPanel::ImageSetPanel()
    : m_renderViewport(nullptr)
    , m_gizmoCenter(nullptr)
    , m_isDraggingGizmo(false)
    , m_draggedGizmo(nullptr)
    , m_dragStartPosition(0, 0)
    , m_gizmoStartPosition(0, 0)
{
    // Setup render viewport.
    m_renderViewport = new RenderViewport;

    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetTexture("uipack_rpg.png");

    m_renderViewport->SetSize(Vector2u(sprite->GetSize()));

    // Setup gizmo.
    sf::RectangleShape* shapeCenter = new sf::RectangleShape;
    shapeCenter->setOutlineThickness(1.f);
    shapeCenter->setOutlineColor(sf::Color(255, 0, 0, 255));
    shapeCenter->setFillColor(sf::Color(255, 0, 255, 100));
    shapeCenter->setSize(sf::Vector2f(50, 50));

    m_gizmoCenter = m_renderViewport->GetRoot()->AddChild<ElementSFDrawable>();
    m_gizmoCenter->SetSFDrawable(shapeCenter);
    m_gizmoCenter->SetSize(shapeCenter->getSize());

    const Vector2f sizeEdge(6.f, 6.f);
    auto createEdgeShape = [&sizeEdge]() -> sf::RectangleShape*
    {
        sf::RectangleShape* shapeEdge = new sf::RectangleShape;
        //shapeEdge->setOutlineThickness(1.f);
        //shapeEdge->setOutlineColor(sf::Color(255, 0, 0, 255));
        shapeEdge->setFillColor(sf::Color(255, 0, 0, 255));
        shapeEdge->setSize(sizeEdge);
        return shapeEdge;
    };

    m_gizmoEdgeTopLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTopLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTopLeft->SetUnifiedOrigin(UDim2(1.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeTopLeft->SetUnifiedPosition(UDim2(0.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTopLeft->SetSize(sizeEdge);

    m_gizmoEdgeTop = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTop->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTop->SetUnifiedOrigin(UDim2(.5f, 0.f, 1.f, 8.f));
    m_gizmoEdgeTop->SetUnifiedPosition(UDim2(0.5f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTop->SetSize(sizeEdge);

    m_gizmoEdgeTopRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTopRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTopRight->SetUnifiedOrigin(UDim2(0.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeTopRight->SetUnifiedPosition(UDim2(1.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTopRight->SetSize(sizeEdge);

    m_gizmoEdgeRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeRight->SetUnifiedOrigin(UDim2(0.f, -8.f, .5f, 0.f));
    m_gizmoEdgeRight->SetUnifiedPosition(UDim2(1.f, 0.f, 0.5f, 0.f));
    m_gizmoEdgeRight->SetSize(sizeEdge);

    m_gizmoEdgeBottomRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottomRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottomRight->SetUnifiedOrigin(UDim2(0.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeBottomRight->SetUnifiedPosition(UDim2(1.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottomRight->SetSize(sizeEdge);

    m_gizmoEdgeBottom = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottom->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottom->SetUnifiedOrigin(UDim2(0.5f, 0.f, 0.f, -8.f));
    m_gizmoEdgeBottom->SetUnifiedPosition(UDim2(.5f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottom->SetSize(sizeEdge);

    m_gizmoEdgeBottomLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottomLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottomLeft->SetUnifiedOrigin(UDim2(1.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeBottomLeft->SetUnifiedPosition(UDim2(0.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottomLeft->SetSize(sizeEdge);

    m_gizmoEdgeLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeLeft->SetUnifiedOrigin(UDim2(1.f, 8.f, 0.5f, 0.f));
    m_gizmoEdgeLeft->SetUnifiedPosition(UDim2(0.f, 0.f, .5f, 0.f));
    m_gizmoEdgeLeft->SetSize(sizeEdge);
}

ImageSetPanel::~ImageSetPanel()
{
    SafeDelete(m_renderViewport);
}

void ImageSetPanel::Update(const DeltaTime& dt)
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
    if (ImGui::Begin("ImageSet Editor", false, flags))
    {
        // Toolbar.
        static float zoomFactor = 1.f;
        if (ImGui::SliderFloat("Zoom Factor", &zoomFactor, 1.f, 16.f))
        {
            m_renderViewport->SetZoom(zoomFactor);
        }

        // Render viewport.
        m_renderViewport->BeginRender();

        UpdateGizmo();

        m_renderViewport->FinalizeRender();
    }
    ImGui::End();
}

void ImageSetPanel::UpdateGizmo()
{
    ImGuiIO& io = ImGui::GetIO();
    const Vector2f canvas_p0 = ImGui::GetCursorScreenPos();
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();

    const Vector2f origin(canvas_p0.x, canvas_p0.y);
    const Vector2f mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    Vector2f pickedPosition = m_renderViewport->GetPickedPosition(Vector2i(mouse_pos_in_canvas));

    if (!m_isDraggingGizmo && is_hovered && is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (m_gizmoCenter->IsPicked(pickedPosition))
        {
            m_isDraggingGizmo = true;
            m_draggedGizmo = m_gizmoCenter;
            m_dragStartPosition = pickedPosition;
            m_gizmoStartPosition = m_gizmoCenter->GetPosition();
        }
    }

    if (m_isDraggingGizmo)
    {
        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            Vector2f itemPosition = m_gizmoStartPosition - m_dragStartPosition + pickedPosition;

            // Snap to pixel.
            itemPosition = Vector2f(Vector2i(itemPosition));

            m_draggedGizmo->SetPosition(itemPosition);
        }
        else
        {
            m_isDraggingGizmo = false;
        }
    }
}

}   //namespace gugu
