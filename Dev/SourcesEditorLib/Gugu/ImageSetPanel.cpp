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
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <array>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImageSetPanel::ImageSetPanel()
    : m_renderViewport(nullptr)
    , m_gizmoCenter(nullptr)
    , m_isDraggingGizmo(false)
    , m_draggedGizmo(nullptr)
    , m_gizmoOffsetGlobalPosition(0, 0)
{
    // Setup render viewport.
    m_renderViewport = new RenderViewport(true);

    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetTexture("uipack_rpg.png");

    m_renderViewport->SetSize(Vector2u(sprite->GetSize()));

    // Setup gizmo.
    CreateGizmo();
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

void ImageSetPanel::CreateGizmo()
{
    sf::RectangleShape* shapeCenter = new sf::RectangleShape;
    //shapeCenter->setOutlineThickness(1.f);
    //shapeCenter->setOutlineColor(sf::Color(255, 0, 0, 255));
    shapeCenter->setFillColor(sf::Color(255, 0, 255, 100));
    shapeCenter->setSize(sf::Vector2f(20, 20));

    auto resizeRectangle = [](ElementSFDrawable* element)
    {
        sf::RectangleShape* shape = (sf::RectangleShape*)element->GetSFDrawable();
        shape->setSize(element->GetSize());
    };

    m_gizmoCenter = m_renderViewport->GetRoot()->AddChild<ElementSFDrawable>();
    m_gizmoCenter->SetSFDrawable(shapeCenter);
    m_gizmoCenter->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoCenter->SetSize(100, 50);

    const Vector2f sizeEdge(12.f, 12.f);
    const float offsetSideEdge = 13.f;

    auto createEdgeShape = []() -> sf::RectangleShape*
    {
        sf::RectangleShape* shapeEdge = new sf::RectangleShape;
        //shapeEdge->setOutlineThickness(1.f);
        //shapeEdge->setOutlineColor(sf::Color(255, 0, 0, 255));
        //shapeEdge->setFillColor(sf::Color(255, 0, 0, 255));
        shapeEdge->setOutlineThickness(-1.f);
        shapeEdge->setOutlineColor(sf::Color(0, 0, 0, 255));
        shapeEdge->setFillColor(sf::Color(255, 255, 255, 255));
        shapeEdge->setSize(sf::Vector2f(20, 20));
        return shapeEdge;
    };

    m_gizmoEdgeTopLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTopLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTopLeft->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeTopLeft->SetUnifiedOrigin(UDim2(1.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeTopLeft->SetUnifiedPosition(UDim2(0.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTopLeft->SetSize(sizeEdge);

    m_gizmoEdgeTop = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTop->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTop->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeTop->SetUnifiedOrigin(UDim2(.5f, 0.f, 1.f, offsetSideEdge));
    m_gizmoEdgeTop->SetUnifiedPosition(UDim2(0.5f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTop->SetSize(sizeEdge);

    m_gizmoEdgeTopRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeTopRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeTopRight->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeTopRight->SetUnifiedOrigin(UDim2(0.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeTopRight->SetUnifiedPosition(UDim2(1.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeTopRight->SetSize(sizeEdge);

    m_gizmoEdgeRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeRight->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeRight->SetUnifiedOrigin(UDim2(0.f, -offsetSideEdge, .5f, 0.f));
    m_gizmoEdgeRight->SetUnifiedPosition(UDim2(1.f, 0.f, 0.5f, 0.f));
    m_gizmoEdgeRight->SetSize(sizeEdge);

    m_gizmoEdgeBottomRight = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottomRight->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottomRight->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeBottomRight->SetUnifiedOrigin(UDim2(0.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeBottomRight->SetUnifiedPosition(UDim2(1.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottomRight->SetSize(sizeEdge);

    m_gizmoEdgeBottom = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottom->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottom->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeBottom->SetUnifiedOrigin(UDim2(0.5f, 0.f, 0.f, -offsetSideEdge));
    m_gizmoEdgeBottom->SetUnifiedPosition(UDim2(.5f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottom->SetSize(sizeEdge);

    m_gizmoEdgeBottomLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeBottomLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeBottomLeft->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeBottomLeft->SetUnifiedOrigin(UDim2(1.f, 0.f, 0.f, 0.f));
    m_gizmoEdgeBottomLeft->SetUnifiedPosition(UDim2(0.f, 0.f, 1.f, 0.f));
    m_gizmoEdgeBottomLeft->SetSize(sizeEdge);

    m_gizmoEdgeLeft = m_gizmoCenter->AddChild<ElementSFDrawable>();
    m_gizmoEdgeLeft->SetSFDrawable(createEdgeShape());
    m_gizmoEdgeLeft->SetCallbackOnSizeChanged(resizeRectangle);
    m_gizmoEdgeLeft->SetUnifiedOrigin(UDim2(1.f, offsetSideEdge, 0.5f, 0.f));
    m_gizmoEdgeLeft->SetUnifiedPosition(UDim2(0.f, 0.f, .5f, 0.f));
    m_gizmoEdgeLeft->SetSize(sizeEdge);
}

void ImageSetPanel::UpdateGizmo()
{
    ImGuiIO& io = ImGui::GetIO();
    const Vector2f canvas_p0 = ImGui::GetCursorScreenPos();
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();

    const Vector2f origin(canvas_p0.x, canvas_p0.y);
    const Vector2f mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    Vector2f pickedGlobalPosition = m_renderViewport->GetPickedPosition(Vector2i(mouse_pos_in_canvas));

    if (!m_isDraggingGizmo && is_hovered && is_active && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const std::array<Element*, 9> gizmoElements = {
                m_gizmoCenter,
                m_gizmoEdgeTopLeft,
                m_gizmoEdgeTop,
                m_gizmoEdgeTopRight,
                m_gizmoEdgeRight,
                m_gizmoEdgeBottomRight,
                m_gizmoEdgeBottom,
                m_gizmoEdgeBottomLeft,
                m_gizmoEdgeLeft,
            };

        for (Element* gizmoElement : gizmoElements)
        {
            if (gizmoElement->IsPicked(pickedGlobalPosition))
            {
                m_isDraggingGizmo = true;
                m_draggedGizmo = gizmoElement;

                if (gizmoElement == m_gizmoCenter)
                {
                    m_gizmoOffsetGlobalPosition = pickedGlobalPosition - gizmoElement->TransformToGlobalFull(Vector2f());
                }
                else
                {
                    m_gizmoOffsetGlobalPosition = pickedGlobalPosition - gizmoElement->TransformToGlobalFull(Vector2f() + gizmoElement->GetOrigin());
                }
            }
        }
    }

    if (m_isDraggingGizmo)
    {
        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            if (m_draggedGizmo == m_gizmoCenter)
            {
                Vector2f elementGlobalPosition = pickedGlobalPosition - m_gizmoOffsetGlobalPosition;

                // Snap to pixel.
                elementGlobalPosition = Vector2f(Vector2i(elementGlobalPosition));

                m_draggedGizmo->SetPosition(elementGlobalPosition);
                //ComputePostDrag();
            }
            else
            {
                Vector2f elementGlobalPosition = pickedGlobalPosition - m_gizmoOffsetGlobalPosition;

                OnDragGizmoEdge(m_draggedGizmo, elementGlobalPosition);
                //ComputePostDrag();
            }
        }
        else
        {
            m_draggedGizmo = nullptr;
            m_isDraggingGizmo = false;
        }
    }
}

void ImageSetPanel::OnDragGizmoEdge(Element* edge, Vector2f edgePosition)
{
    Vector2f kSize = m_gizmoCenter->GetSize();
    Vector2f kPositionTopLeft = m_gizmoCenter->GetPosition();
    Vector2f kPositionBottomRight = kPositionTopLeft + kSize;

    // Top Left corner
    if (edge == m_gizmoEdgeTopLeft || edge == m_gizmoEdgeLeft || edge == m_gizmoEdgeBottomLeft)
    {
        kPositionTopLeft.x = gugu::Min(edgePosition.x, kPositionBottomRight.x - 1.f);      //-1 to ensure no size of 0
    }

    if (edge == m_gizmoEdgeTopLeft || edge == m_gizmoEdgeTop || edge == m_gizmoEdgeTopRight)
    {
        kPositionTopLeft.y = gugu::Min(edgePosition.y, kPositionBottomRight.y - 1.f);      //-1 to ensure no size of 0
    }

    // Bottom Right corner
    if (edge == m_gizmoEdgeTopRight || edge == m_gizmoEdgeRight || edge == m_gizmoEdgeBottomRight)
    {
        kPositionBottomRight.x = gugu::Max(edgePosition.x, kPositionTopLeft.x + 1.f);      //+1 to ensure no size of 0
    }

    if (edge == m_gizmoEdgeBottomLeft || edge == m_gizmoEdgeBottom || edge == m_gizmoEdgeBottomRight)
    {
        kPositionBottomRight.y = gugu::Max(edgePosition.y, kPositionTopLeft.y + 1.f);      //+1 to ensure no size of 0
    }

    // Snap to pixel.
    kPositionTopLeft = Vector2f(Vector2i(kPositionTopLeft));
    kPositionBottomRight = Vector2f(Vector2i(kPositionBottomRight));

    kSize = kPositionBottomRight - kPositionTopLeft;

    m_gizmoCenter->SetPosition(kPositionTopLeft);
    m_gizmoCenter->SetSize(kSize);
}

}   //namespace gugu
