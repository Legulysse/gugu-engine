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

    ElementDataContext context;
    context.dataBindings = m_dataBindings;
    m_widgetRootElement = m_elementWidget->InstanciateWidget(context);

    if (m_widgetRootElement)
    {
        m_viewportRoot->AddChild(m_widgetRootElement);
    }
}

void ElementWidgetPanel::UpdateHierarchyImpl(const DeltaTime& dt)
{
    static ImGuiTreeNodeFlags itemFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth
        | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

    if (m_widgetRootData)
    {
        ImGui::PushID("_HIERARCHY_TREE");
        BaseElementData* deleted = nullptr;
        DisplayTreeNode(m_widgetRootData, itemFlags, deleted);
        ImGui::PopID();

        if (deleted)
        {
            DeleteElement(deleted);
            RaiseDirty();
        }
    }
}

void ElementWidgetPanel::DisplayTreeNode(BaseElementData* node, int itemFlags, BaseElementData*& deleted)
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
        if (ElementCompositeData* nodeComposite = dynamic_cast<ElementCompositeData*>(node))
        {
            const std::vector<BaseElementData*>& components = nodeComposite->components;

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
            const std::vector<BaseElementData*>& components = nodeButton->components;

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
        const std::vector<BaseElementData*>& children = node->children;

        for (size_t i = 0; i < children.size(); ++i)
        {
            ImGui::PushID((int)i);
            DisplayTreeNode(children[i], itemFlags, deleted);
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

void ElementWidgetPanel::HandleContextMenu(BaseElementData* node, BaseElementData*& deleted)
{
    if (ImGui::BeginPopupContextItem())
    {
        bool isRootNode = node == m_widgetRootData;
        bool isComponent = dynamic_cast<ElementCompositeData*>(node->parent) != nullptr;

        if (ImGui::BeginMenu("Add Child..."))
        {
            BaseElementData* elementData = DisplayElementInstanciationContextMenu();
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

        if (!isComponent)
        {
            if (!isRootNode)
            {
                if (ImGui::BeginMenu("Insert..."))
                {
                    BaseElementData* elementData = DisplayElementInstanciationContextMenu();
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
                BaseElementData* elementData = DisplayElementInstanciationContextMenu();
                if (elementData)
                {
                    // Deep copy.
                    pugi::xml_document xmlDocument;

                    ElementSaveContext saveContext;
                    saveContext.node = xmlDocument;
                    node->SaveToXml(saveContext);

                    ElementParseContext parseContext;
                    parseContext.node = xmlDocument.first_child();
                    elementData->LoadFromXml(parseContext);

                    if (isRootNode)
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

        // TODO: handle components copy/paste.
        if (!isComponent)
        {
            ImGui::Separator();

            if (ImGui::MenuItem("Copy"))
            {
                CopyElementToClipboard(node);
            }

            ImGui::BeginDisabled(isRootNode);
            if (ImGui::MenuItem("Cut"))
            {
                CopyElementToClipboard(node);
                deleted = node;
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(isRootNode);
            if (ImGui::MenuItem("Paste (Insert)"))
            {
                size_t index = StdVectorIndexOf(node->parent->children, node);

                PasteElementFromClipboard(node->parent, index);
            }
            ImGui::EndDisabled();

            if (ImGui::MenuItem("Paste as Child"))
            {
                PasteElementFromClipboard(node);
            }

            ImGui::BeginDisabled(isRootNode);
            if (ImGui::MenuItem("Duplicate"))
            {
                size_t index = StdVectorIndexOf(node->parent->children, node);

                CopyElementToClipboard(node);
                PasteElementFromClipboard(node->parent, index + 1);
            }
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        ImGui::BeginDisabled(isRootNode);
        if (ImGui::MenuItem("Delete") && !deleted)
        {
            deleted = node;
        }
        ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}

BaseElementData* ElementWidgetPanel::DisplayElementInstanciationContextMenu()
{
    BaseElementData* elementData = nullptr;

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

    if (ImGui::MenuItem("Element Widget Instance"))
    {
        elementData = new ElementWidgetInstanceData;
    }

    return elementData;
}

}   //namespace gugu
