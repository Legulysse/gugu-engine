////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidgetPanel::ElementWidgetPanel(ElementWidget* resource)
    : DocumentPanel(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_selectedElement(nullptr)
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

void ElementWidgetPanel::UpdateHierarchyImpl(const DeltaTime& dt)
{
    static ImGuiTreeNodeFlags itemFlags =/* ImGuiTreeNodeFlags_Leaf*/ /*| ImGuiTreeNodeFlags_NoTreePushOnOpen |*/
        ImGuiTreeNodeFlags_SpanAvailWidth
        | ImGuiTreeNodeFlags_SpanFullWidth
        | ImGuiTreeNodeFlags_DefaultOpen
        | ImGuiTreeNodeFlags_OpenOnDoubleClick
        | ImGuiTreeNodeFlags_OpenOnArrow;

    ImGui::PushID("_HIERARCHY_TREE");
    DisplayTreeNode(m_renderViewport->GetRoot(), itemFlags);
    ImGui::PopID();
}

void ElementWidgetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    if (!m_selectedElement)
        return;

    ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

    Element* element = m_selectedElement;
    if (ImGui::CollapsingHeader("Element", headerFlags))
    {
        Vector2f origin = element->GetOrigin();
        if (ImGui::InputFloat2("Origin", &origin))
        {
            element->SetOrigin(origin);
            RaiseDirty();
        }

        Vector2f position = element->GetPosition();
        if (ImGui::InputFloat2("Position", &position))
        {
            element->SetPosition(position);
            RaiseDirty();
        }

        Vector2f size = element->GetSize();
        if (ImGui::InputFloat2("Size", &size))
        {
            element->SetSize(size);
            RaiseDirty();
        }
    }

    ElementSprite* elementSprite = dynamic_cast<ElementSprite*>(m_selectedElement);
    if (elementSprite && ImGui::CollapsingHeader("Element Sprite", headerFlags))
    {
        Texture* texture = elementSprite->GetTexture();
        std::string textureId = !texture ? "" : texture->GetID();
        if (ImGui::InputText("Texture", &textureId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementSprite->SetTexture(textureId);
            RaiseDirty();
        }

        sf::IntRect subRect = elementSprite->GetSubRect();
        if (ImGui::InputInt4("TextureRect", &subRect))
        {
            elementSprite->SetSubRect(subRect);
            RaiseDirty();
        }
    }

    ElementSpriteGroup* elementSpriteGroup = dynamic_cast<ElementSpriteGroup*>(m_selectedElement);
    if (elementSpriteGroup && ImGui::CollapsingHeader("Element Sprite Group", headerFlags))
    {
    }
}

void ElementWidgetPanel::DisplayTreeNode(Element* node, int itemFlags)
{
    ImGuiTreeNodeFlags nodeFlags = itemFlags;

    const std::vector<Element*>& children = node->GetChildren();

    if (children.empty())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (node == m_selectedElement)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool isOpen = ImGui::TreeNodeEx(node->GetSerializedType().c_str(), nodeFlags);

    if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
    {
        m_selectedElement = node;
    }

    // Context menu.
    HandleContextMenu(node);

    if (isOpen)
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            ImGui::PushID((int)i);
            DisplayTreeNode(children[i], itemFlags);
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

void ElementWidgetPanel::HandleContextMenu(Element* node)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::BeginMenu("Add..."))
        {
            if (ImGui::MenuItem("Element"))
            {
                node->AddChild<Element>();
                RaiseDirty();
            }

            if (ImGui::MenuItem("Element Sprite"))
            {
                node->AddChild<ElementSprite>();
                RaiseDirty();
            }

            if (ImGui::MenuItem("Element Sprite Group"))
            {
                node->AddChild<ElementSpriteGroup>();
                RaiseDirty();
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}

}   //namespace gugu
