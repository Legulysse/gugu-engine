////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void ElementWidgetPanel::ClearHierarchy()
{
    m_selectedElementData = nullptr;
    m_selectedElement = nullptr;
    SafeDelete(m_widgetRootElement);
    SafeDelete(m_dataBindings);

    m_widgetRootData = nullptr;
}

void ElementWidgetPanel::RebuildHierarchy()
{
    ClearHierarchy();

    m_widgetRootData = m_elementWidget->GetData();

    m_dataBindings = new ElementDataBindings;
    m_widgetRootElement = m_elementWidget->InstanciateWidget(m_dataBindings);
    if (m_widgetRootElement)
    {
        m_renderViewport->GetRoot()->AddChild(m_widgetRootElement);
    }
}

void ElementWidgetPanel::UpdateHierarchyImpl(const DeltaTime& dt)
{
    static ImGuiTreeNodeFlags itemFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth
        | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    if (m_widgetRootData)
    {
        ImGui::PushID("_HIERARCHY_TREE");
        ElementData* deleted = nullptr;
        DisplayTreeNode(m_widgetRootData, itemFlags, deleted);
        ImGui::PopID();

        if (deleted)
        {
            DeleteElement(deleted);
        }
    }
}

void ElementWidgetPanel::DisplayTreeNode(ElementData* node, int itemFlags, ElementData*& deleted)
{
    ImGuiTreeNodeFlags nodeFlags = itemFlags;

    const std::vector<ElementData*>& children = node->children;

    if (children.empty())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (node == m_selectedElementData)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool isOpen = ImGui::TreeNodeEx(node->GetSerializedType().c_str(), nodeFlags);

    if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
    {
        m_selectedElementData = node;
        m_selectedElement = m_dataBindings->elementFromData.at(node);
    }

    // Context menu.
    HandleContextMenu(node, deleted);

    if (isOpen)
    {
        // Display components.
        if (ElementSpriteGroupData* nodeSpriteGroup = dynamic_cast<ElementSpriteGroupData*>(node))
        {
            const std::vector<ElementSpriteGroupItemData*>& components = nodeSpriteGroup->components;

            ImGuiTreeNodeFlags componentFlags = ImGuiTreeNodeFlags_Leaf;
            ImGui::TreeNodeEx("<Components>", componentFlags);

            for (size_t i = 0; i < components.size(); ++i)
            {
                ImGui::PushID((int)i);
                DisplayTreeNode(components[i], itemFlags, deleted);
                ImGui::PopID();
            }

            ImGui::TreePop();
        }
        else if (ElementButtonData* nodeButton = dynamic_cast<ElementButtonData*>(node))
        {
            const std::vector<ElementData*>& components = nodeButton->components;

            ImGuiTreeNodeFlags componentFlags = ImGuiTreeNodeFlags_Leaf;
            ImGui::TreeNodeEx("<Components>", componentFlags);

            for (size_t i = 0; i < components.size(); ++i)
            {
                ImGui::PushID((int)i);
                DisplayTreeNode(components[i], itemFlags, deleted);
                ImGui::PopID();
            }

            ImGui::TreePop();
        }

        // Display children.
        for (size_t i = 0; i < children.size(); ++i)
        {
            ImGui::PushID((int)i);
            DisplayTreeNode(children[i], itemFlags, deleted);
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

void ElementWidgetPanel::HandleContextMenu(ElementData* node, ElementData*& deleted)
{
    //todo: replace command (or default empty root ?)
    //todo: copy/paste/duplicate

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::BeginMenu("Add..."))
        {
            if (ImGui::MenuItem("Element"))
            {
                AppendNewElement(node, new ElementData);
                RaiseDirty();
            }

            if (ImGui::MenuItem("Element Sprite"))
            {
                AppendNewElement(node, new ElementSpriteData);
                RaiseDirty();
            }

            if (ImGui::MenuItem("Element Sprite Group"))
            {
                AppendNewElement(node, new ElementSpriteGroupData);
                RaiseDirty();
            }

            if (ImGui::MenuItem("Element Button"))
            {
                AppendNewElement(node, new ElementButtonData);
                RaiseDirty();
            }

            if (ElementSpriteGroupData* nodeSpriteGroup = dynamic_cast<ElementSpriteGroupData*>(node))
            {
                ImGui::Separator();

                if (ImGui::MenuItem("Element Sprite Group Item"))
                {
                    AppendNewComponent(nodeSpriteGroup, new ElementSpriteGroupItemData);
                    RaiseDirty();
                }
            }

            ImGui::EndMenu();
        }

        ImGui::BeginDisabled(node == m_widgetRootData);
        if (ImGui::MenuItem("Delete") && !deleted)
        {
            deleted = node;
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

}   //namespace gugu
