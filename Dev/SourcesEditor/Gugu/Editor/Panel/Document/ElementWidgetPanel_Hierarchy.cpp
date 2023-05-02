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

    m_widgetRootData = m_elementWidget->GetRootData();

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
            RaiseDirty();
        }
    }
}

void ElementWidgetPanel::DisplayTreeNode(ElementData* node, int itemFlags, ElementData*& deleted)
{
    // Special case : when using the Replace command, a new node will be inserted before the current node, which will then be displayed a second time.
    // TODO: some kind of proper delayed command system would allow to separate the display from modifications.
    if (node == deleted)
        return;

    ImGuiTreeNodeFlags nodeFlags = itemFlags;

    if (node->children.empty())
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
        const std::vector<ElementData*>& children = node->children;

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
    //todo: copy/paste/duplicate

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::BeginMenu("Add Child..."))
        {
            ElementData* elementData = DisplayElementInstanciationContextMenu();
            if (elementData)
            {
                AddChildElement(node, elementData);
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

        if (!dynamic_cast<ElementSpriteGroupItemData*>(node))
        {
            if (node != m_widgetRootData)
            {
                if (ImGui::BeginMenu("Insert..."))
                {
                    ElementData* elementData = DisplayElementInstanciationContextMenu();
                    if (elementData)
                    {
                        InsertElement(node, elementData);
                        RaiseDirty();
                    }

                    ImGui::EndMenu();
                }
            }

            if (ImGui::BeginMenu("Replace...") && !deleted)
            {
                ElementData* elementData = DisplayElementInstanciationContextMenu();
                if (elementData)
                {
                    elementData->DeepCopy(node);

                    if (node == m_widgetRootData)
                    {
                        deleted = node;
                        m_elementWidget->SetRootData(elementData, false);
                    }
                    else
                    {
                        deleted = node;
                        InsertElement(node, elementData);
                    }

                    // Do not raise dirty here, we need to wait for the delete action to proceed.
                    //RaiseDirty();
                }

                ImGui::EndMenu();
            }
        }

        ImGui::Separator();

        ImGui::BeginDisabled(node == m_widgetRootData);
        if (ImGui::MenuItem("Delete") && !deleted)
        {
            deleted = node;
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

ElementData* ElementWidgetPanel::DisplayElementInstanciationContextMenu()
{
    ElementData * elementData = nullptr;

    if (ImGui::MenuItem("Element"))
    {
        elementData = new ElementData;
    }

    if (ImGui::MenuItem("Element Sprite"))
    {
        elementData = new ElementSpriteData;
    }

    if (ImGui::MenuItem("Element Sprite Group"))
    {
        elementData = new ElementSpriteGroupData;
    }

    if (ImGui::MenuItem("Element Text"))
    {
        elementData = new ElementTextData;
    }

    if (ImGui::MenuItem("Element Button"))
    {
        elementData = new ElementButtonData;
    }

    return elementData;
}

}   //namespace gugu
