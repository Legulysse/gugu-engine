////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ImageSetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <array>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImageSetPanel::ImageSetPanel(ImageSet* resource)
    : DocumentPanel(resource)
    , m_imageSet(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_selectedIndex(-1)
    , m_gizmoCenter(nullptr)
    , m_isDraggingGizmo(false)
    , m_draggedGizmo(nullptr)
    , m_gizmoOffsetGlobalPosition(0, 0)
{
    // Setup RenderViewport and Sprite.
    m_renderViewport = new RenderViewport(true);

    ElementSprite* sprite = m_renderViewport->GetRoot()->AddChild<ElementSprite>();
    sprite->SetTexture(m_imageSet->GetTexture());

    m_renderViewport->SetSize(Vector2u(sprite->GetSize()));

    // Setup gizmo.
    CreateGizmo();
}

ImageSetPanel::~ImageSetPanel()
{
    SafeDelete(m_renderViewport);
}

void ImageSetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 1.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    UpdateGizmo();
    m_renderViewport->ImGuiEnd();
}

void ImageSetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    // Selected SubImage edition.
    if (m_selectedIndex >= 0)
    {
        std::string name = m_imageSet->GetSubImage(m_selectedIndex)->GetName();
        sf::IntRect rect = m_imageSet->GetSubImage(m_selectedIndex)->GetRect();

        int position[2] = { rect.left, rect.top };
        int size[2] = { rect.width, rect.height };

        if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_imageSet->GetSubImage(m_selectedIndex)->SetName(name);
            RaiseDirty();
        }

        if (ImGui::InputInt2("Position", position))
        {
            m_imageSet->GetSubImage(m_selectedIndex)->SetRect(sf::IntRect(position[0], position[1], size[0], size[1]));
            RaiseDirty();
        }

        if (ImGui::InputInt2("Size", size))
        {
            m_imageSet->GetSubImage(m_selectedIndex)->SetRect(sf::IntRect(position[0], position[1], size[0], size[1]));
            RaiseDirty();
        }
    }
    else
    {
        ImGui::BeginDisabled();

        std::string dummyStr;
        int dummyVecA[2] = { 0, 0 };
        int dummyVecB[2] = { 0, 0 };
        ImGui::InputText("Name", &dummyStr);
        ImGui::InputInt2("Position", dummyVecA);
        ImGui::InputInt2("Size", dummyVecB);

        ImGui::EndDisabled();
    }

    ImGui::Spacing();

    // SubImages list.
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY /* | ImGuiTableFlags_NoPadInnerX */;
    if (ImGui::BeginTable("SubImages Table", 6, flags))
    {
        ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
        ImGui::TableSetupColumn("#", columnFlags, 30.f);
        ImGui::TableSetupColumn("name", columnFlags, 150.f);
        ImGui::TableSetupColumn("left", columnFlags, 40.f);
        ImGui::TableSetupColumn("top", columnFlags, 40.f);
        ImGui::TableSetupColumn("width", columnFlags, 40.f);
        ImGui::TableSetupColumn("height", columnFlags, 40.f);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        const std::vector<SubImage*>& subImages = m_imageSet->GetSubImages();

        // TODO: handle sort (ImGuiTableSortSpecs).
        // TODO: handle big list (ImGuiListClipper).
        for (size_t rowIndex = 0; rowIndex < subImages.size(); ++rowIndex)
        {
            ImGui::PushID(rowIndex);

            float row_min_height = 0.f;
            ImGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

            if (rowIndex == 0)
            {
                // Setup ItemWidth once.
                int headerIndex = 0;

                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
                ImGui::TableSetColumnIndex(headerIndex++);
                ImGui::PushItemWidth(-1);
            }

#if 1
            std::string name = subImages[rowIndex]->GetName();
            sf::IntRect rect = subImages[rowIndex]->GetRect();

            int columnIndex = 0;
            ImGui::TableSetColumnIndex(columnIndex++);

            char label[32];
            sprintf(label, "%04d", rowIndex);
            ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
            if (ImGui::Selectable(label, (int)rowIndex == m_selectedIndex, selectable_flags, ImVec2(0, row_min_height)))
            {
                m_selectedIndex = rowIndex;
            }

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text(name.c_str());

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%d", rect.left);

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%d", rect.top);

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%d", rect.width);

            ImGui::TableSetColumnIndex(columnIndex++);
            ImGui::Text("%d", rect.height);

            ImGui::PopID();
#else
            std::string name = subImages[rowIndex]->GetName();
            sf::IntRect rect = subImages[rowIndex]->GetRect();

            int columnIndex = 0;
            bool dirtyName = false;
            bool dirtyRect = false;
            bool forceFocus = false;

            ImGui::TableSetColumnIndex(columnIndex++);

            char label[32];
            sprintf(label, "%04d", rowIndex);
            ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
            if (ImGui::Selectable(label, (int)rowIndex == m_selectedIndex, selectable_flags , ImVec2(0, row_min_height)))
            {
                m_selectedIndex = rowIndex;
            }

            ImGui::TableSetColumnIndex(columnIndex++);
            dirtyName |= ImGui::InputText("##name", &name);
            forceFocus |= ImGui::IsItemFocused();

            ImGui::TableSetColumnIndex(columnIndex++);
            dirtyRect |= ImGui::InputInt("##left", &rect.left, 0);
            forceFocus |= ImGui::IsItemFocused();

            ImGui::TableSetColumnIndex(columnIndex++);
            dirtyRect |= ImGui::InputInt("##top", &rect.top, 0);
            forceFocus |= ImGui::IsItemFocused();

            ImGui::TableSetColumnIndex(columnIndex++);
            dirtyRect |= ImGui::InputInt("##width", &rect.width, 0);
            forceFocus |= ImGui::IsItemFocused();

            ImGui::TableSetColumnIndex(columnIndex++);
            dirtyRect |= ImGui::InputInt("##height", &rect.height, 0);
            forceFocus |= ImGui::IsItemFocused();

            ImGui::PopID();

            if (dirtyName)
            {
                subImages[rowIndex]->SetName(name);
            }

            if (dirtyRect)
            {
                subImages[rowIndex]->SetRect(rect);
            }

            if (forceFocus || dirtyName || dirtyRect)
            {
                m_selectedIndex = rowIndex;
            }
#endif
        }

        ImGui::EndTable();
    }
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
    // Handle picking (should be used inside a viewport begin/end block).
    ImGuiIO& io = ImGui::GetIO();
    const Vector2f canvas_p0 = ImGui::GetCursorScreenPos();
    const Vector2f mouse_pos_in_canvas(io.MousePos.x - canvas_p0.x, io.MousePos.y - canvas_p0.y);
    Vector2f pickedGlobalPosition = m_renderViewport->GetPickedPosition(Vector2i(mouse_pos_in_canvas));

    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();

    if (m_selectedIndex == -1)
    {
        m_gizmoCenter->SetVisible(false);
    }
    else
    {
        m_gizmoCenter->SetVisible(true);

        // Sync with selected SubImage.
        SubImage* subImage = m_imageSet->GetSubImage(m_selectedIndex);

        sf::IntRect rectBefore = subImage->GetRect();
        m_gizmoCenter->SetPosition((float)rectBefore.left, (float)rectBefore.top);
        m_gizmoCenter->SetSize((float)rectBefore.width, (float)rectBefore.height);

        // Test the selected gizmo drag&drop.
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
                        m_gizmoOffsetGlobalPosition = pickedGlobalPosition - gizmoElement->TransformToGlobal(Vector2::Zero_f);
                    }
                    else
                    {
                        m_gizmoOffsetGlobalPosition = pickedGlobalPosition - gizmoElement->TransformToGlobal(Vector2::Zero_f + gizmoElement->GetOrigin());
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
                }
                else
                {
                    Vector2f elementGlobalPosition = pickedGlobalPosition - m_gizmoOffsetGlobalPosition;
                    OnDragGizmoEdge(m_draggedGizmo, elementGlobalPosition);
                }
            }
            else
            {
                m_draggedGizmo = nullptr;
                m_isDraggingGizmo = false;
            }

            // Apply modifications if needed.
            sf::IntRect rectAfter(sf::Vector2i(m_gizmoCenter->GetPosition()), sf::Vector2i(m_gizmoCenter->GetSize()));
            if (rectBefore != rectAfter)
            {
                subImage->SetRect(rectAfter);
                RaiseDirty();
            }
        }
    }

    // Handle picking of other SubImages (gizmo will be sync next frame).
    if (!m_isDraggingGizmo && is_hovered && is_active && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        const std::vector<SubImage*>& subImages = m_imageSet->GetSubImages();

        for (size_t rowIndex = 0; rowIndex < subImages.size(); ++rowIndex)
        {
            if (subImages[rowIndex]->GetRect().contains(Vector2i(pickedGlobalPosition)))
            {
                m_selectedIndex = rowIndex;
                break;
            }
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
        kPositionTopLeft.x = Min(edgePosition.x, kPositionBottomRight.x - 1.f);      //-1 to ensure no size of 0
    }

    if (edge == m_gizmoEdgeTopLeft || edge == m_gizmoEdgeTop || edge == m_gizmoEdgeTopRight)
    {
        kPositionTopLeft.y = Min(edgePosition.y, kPositionBottomRight.y - 1.f);      //-1 to ensure no size of 0
    }

    // Bottom Right corner
    if (edge == m_gizmoEdgeTopRight || edge == m_gizmoEdgeRight || edge == m_gizmoEdgeBottomRight)
    {
        kPositionBottomRight.x = Max(edgePosition.x, kPositionTopLeft.x + 1.f);      //+1 to ensure no size of 0
    }

    if (edge == m_gizmoEdgeBottomLeft || edge == m_gizmoEdgeBottom || edge == m_gizmoEdgeBottomRight)
    {
        kPositionBottomRight.y = Max(edgePosition.y, kPositionTopLeft.y + 1.f);      //+1 to ensure no size of 0
    }

    // Snap to pixel.
    kPositionTopLeft = Vector2f(Vector2i(kPositionTopLeft));
    kPositionBottomRight = Vector2f(Vector2i(kPositionBottomRight));

    kSize = kPositionBottomRight - kPositionTopLeft;

    m_gizmoCenter->SetPosition(kPositionTopLeft);
    m_gizmoCenter->SetSize(kSize);
}

}   //namespace gugu
