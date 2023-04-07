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
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidgetPanel::ElementWidgetPanel(ElementWidget* resource)
    : DocumentPanel(resource)
    , m_elementWidget(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_dataContext(nullptr)
    , m_widgetRootData(nullptr)
    , m_widgetRootElement(nullptr)
    , m_selectedElementData(nullptr)
    , m_selectedElement(nullptr)
{
    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(1280, 720));

    // Instanciate Widget
    m_widgetRootData = m_elementWidget->GetData();

    RebuildWidgetHierarchy();
}

ElementWidgetPanel::~ElementWidgetPanel()
{
    m_widgetRootData = nullptr;
    m_widgetRootElement = nullptr;
    m_selectedElementData = nullptr;
    m_selectedElement = nullptr;

    SafeDelete(m_dataContext);
    SafeDelete(m_renderViewport);
}

void ElementWidgetPanel::OnUndoRedo()
{
    RebuildWidgetHierarchy();
}

void ElementWidgetPanel::RebuildWidgetHierarchy()
{
    m_selectedElementData = nullptr;
    m_selectedElement = nullptr;
    SafeDelete(m_widgetRootElement);
    SafeDelete(m_dataContext);

    m_dataContext = new ElementDataContext;
    m_widgetRootElement = m_elementWidget->InstanciateWidget(*m_dataContext);
    if (m_widgetRootElement)
    {
        m_renderViewport->GetRoot()->AddChild(m_widgetRootElement);
        m_selectedElementData = m_widgetRootData;
        m_selectedElement = m_widgetRootElement;
    }
}

void ElementWidgetPanel::UpdatePanelImpl(const DeltaTime& dt)
{
    // Toolbar.
    if (ImGui::SliderFloat("Zoom Factor", &m_zoomFactor, 0.f, 16.f))
    {
        m_renderViewport->SetZoom(m_zoomFactor);
    }

    Vector2u size = m_renderViewport->GetSize();
    if (ImGui::InputInt2("Viewport Size", &size))
    {
        m_renderViewport->SetSize(size);
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

    if (m_widgetRootData)
    {
        ImGui::PushID("_HIERARCHY_TREE");
        DisplayTreeNode(m_widgetRootData, itemFlags);
        ImGui::PopID();
    }
}

void ElementWidgetPanel::UpdatePropertiesImpl(const DeltaTime& dt)
{
    if (!m_selectedElementData)
        return;

    ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen;

    ElementData* elementData = m_selectedElementData;
    Element* element = m_selectedElement;
    if (elementData && ImGui::CollapsingHeader("Common", headerFlags))
    {
        // Origin
        ImGui::BeginDisabled(elementData->useDimOrigin);
        if (ImGui::InputFloat2("Origin", &elementData->origin))
        {
            element->SetOrigin(elementData->origin);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        if (ImGui::Checkbox("Use Unified Origin", &elementData->useDimOrigin))
        {
            if (elementData->useDimOrigin)
            {
                elementData->dimOrigin = UDim2(Vector2::Zero_f, elementData->origin);
                elementData->origin = Vector2::Zero_f;
                element->SetUnifiedOrigin(elementData->dimOrigin);
                RaiseDirty();
            }
            else
            {
                elementData->origin = elementData->dimOrigin.absolute;
                elementData->dimOrigin = UDim2::ZERO;
                element->ResetUnifiedOrigin();
                RaiseDirty();
            }
        }

        ImGui::BeginDisabled(!elementData->useDimOrigin);
        if (ImGui::InputFloat4("Unified Origin", &elementData->dimOrigin))
        {
            element->SetUnifiedOrigin(elementData->dimOrigin);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        ImGui::Spacing();

        // Position
        ImGui::BeginDisabled(elementData->useDimPosition);
        if (ImGui::InputFloat2("Position", &elementData->position))
        {
            element->SetPosition(elementData->position);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        if (ImGui::Checkbox("Use Unified Position", &elementData->useDimPosition))
        {
            if (elementData->useDimPosition)
            {
                elementData->dimPosition = UDim2(Vector2::Zero_f, elementData->position);
                elementData->position = Vector2::Zero_f;
                element->SetUnifiedPosition(elementData->dimPosition);
                RaiseDirty();
            }
            else
            {
                elementData->position = elementData->dimPosition.absolute;
                elementData->dimPosition = UDim2::ZERO;
                element->ResetUnifiedPosition();
                RaiseDirty();
            }
        }

        ImGui::BeginDisabled(!elementData->useDimPosition);
        if (ImGui::InputFloat4("Unified Position", &elementData->dimPosition))
        {
            element->SetUnifiedPosition(elementData->dimPosition);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        ImGui::Spacing();

        // Size
        ImGui::BeginDisabled(elementData->useDimSize);
        if (ImGui::InputFloat2("Size", &elementData->size))
        {
            element->SetSize(elementData->size);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        if (ImGui::Checkbox("Use Unified Size", &elementData->useDimSize))
        {
            if (elementData->useDimSize)
            {
                elementData->dimSize = UDim2(Vector2::Zero_f, elementData->size);
                elementData->size = Vector2::Zero_f;
                element->SetUnifiedSize(elementData->dimSize);
                RaiseDirty();
            }
            else
            {
                elementData->size = elementData->dimSize.absolute;
                elementData->dimSize = UDim2::ZERO;
                element->ResetUnifiedSize();
                RaiseDirty();
            }
        }

        ImGui::BeginDisabled(!elementData->useDimSize);
        if (ImGui::InputFloat4("Unified Size", &elementData->dimSize))
        {
            element->SetUnifiedSize(elementData->dimSize);
            RaiseDirty();
        }
        ImGui::EndDisabled();

        ImGui::Spacing();

        // Rotation
        if (ImGui::InputFloat("Rotation", &elementData->rotation))
        {
            element->SetRotation(elementData->rotation);
            RaiseDirty();
        }

        // Flip
        if (ImGui::Checkbox("Flip V", &elementData->flipV))
        {
            element->SetFlipV(elementData->flipV);
            RaiseDirty();
        }

        if (ImGui::Checkbox("Flip H", &elementData->flipH))
        {
            element->SetFlipH(elementData->flipH);
            RaiseDirty();
        }
    }

    ElementSpriteGroupData* elementSpriteGroupData = dynamic_cast<ElementSpriteGroupData*>(elementData);
    ElementSpriteGroup* elementSpriteGroup = dynamic_cast<ElementSpriteGroup*>(element);
    if (elementSpriteGroupData && ImGui::CollapsingHeader("Sprite Group", headerFlags))
    {
        Texture* texture = elementSpriteGroupData->texture;
        std::string textureId = !texture ? "" : texture->GetID();
        if (ImGui::InputText("Texture", &textureId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementSpriteGroupData->texture = GetResources()->GetTexture(textureId);
            elementSpriteGroupData->imageSet = nullptr;
            elementSpriteGroup->SetTexture(elementSpriteGroupData->texture);
            RaiseDirty();
        }

        ImageSet* imageSet = elementSpriteGroupData->imageSet;
        std::string imageSetId = !imageSet ? "" : imageSet->GetID();
        if (ImGui::InputText("ImageSet", &imageSetId, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            elementSpriteGroupData->imageSet = GetResources()->GetImageSet(imageSetId);
            elementSpriteGroupData->texture = nullptr;
            elementSpriteGroup->SetTexture(!elementSpriteGroupData->imageSet ? nullptr : elementSpriteGroupData->imageSet->GetTexture());
            RaiseDirty();
        }
    }

    ElementSpriteBaseData* elementSpriteBaseData = dynamic_cast<ElementSpriteBaseData*>(elementData);
    ElementSpriteBase* elementSpriteBase = dynamic_cast<ElementSpriteBase*>(element);
    if (elementSpriteBaseData && ImGui::CollapsingHeader("Sprite", headerFlags))
    {
        ElementSpriteData* elementSpriteData = dynamic_cast<ElementSpriteData*>(elementData);
        ElementSprite* elementSprite = dynamic_cast<ElementSprite*>(element);
        ElementSpriteGroupItemData* elementSpriteGroupItemData = dynamic_cast<ElementSpriteGroupItemData*>(elementData);
        ElementSpriteGroupItem* elementSpriteGroupItem = dynamic_cast<ElementSpriteGroupItem*>(element);

        if (elementSpriteData)
        {
            Texture* texture = elementSpriteData->texture;
            std::string textureId = !texture ? "" : texture->GetID();
            if (ImGui::InputText("Texture", &textureId, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                elementSpriteData->texture = GetResources()->GetTexture(textureId);
                elementSpriteData->imageSet = nullptr;
                elementSpriteData->subImageName = "";
                elementSprite->SetTexture(elementSpriteData->texture
                    , texture == nullptr || elementSpriteData->texture == nullptr   // Only update rect if texture was null, or set to null.
                    , texture == nullptr);                                          // Only update size if texture was null.
                elementSpriteData->textureRect = elementSprite->GetSubRect();
                elementSpriteData->size = elementSprite->GetSize();
                RaiseDirty();
            }

            ImageSet* imageSet = elementSpriteData->imageSet;
            std::string imageSetId = !imageSet ? "" : imageSet->GetID();
            if (ImGui::InputText("ImageSet", &imageSetId, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                elementSpriteData->imageSet = GetResources()->GetImageSet(imageSetId);
                elementSpriteData->texture = nullptr;
                elementSpriteData->subImageName = "";
                elementSprite->SetTexture(!elementSpriteData->imageSet ? nullptr : elementSpriteData->imageSet->GetTexture(), false);
                elementSpriteData->textureRect = elementSprite->GetSubRect();
                elementSpriteData->size = elementSprite->GetSize();
                RaiseDirty();
            }

            if (ImGui::InputText("SubImage", &elementSpriteData->subImageName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                SubImage* subImage = elementSpriteData->imageSet->GetSubImage(elementSpriteData->subImageName);
                elementSpriteGroupItem->SetSubRect(!subImage ? sf::IntRect() : subImage->GetRect(), false);
                elementSpriteData->textureRect = sf::IntRect();
                RaiseDirty();
            }

            if (ImGui::InputInt4("TextureRect", &elementSpriteData->textureRect))
            {
                elementSpriteBase->SetSubRect(elementSpriteData->textureRect, false);
                elementSpriteData->subImageName = "";
                RaiseDirty();
            }
        }
        else if (elementSpriteGroupItemData)
        {
            ElementSpriteGroup* ownerGroup = dynamic_cast<ElementSpriteGroup*>(elementSpriteGroupItem->GetParent());
            ElementSpriteGroupData* ownerGroupData = dynamic_cast<ElementSpriteGroupData*>(m_dataContext->dataFromElement.at(ownerGroup));

            if (ImGui::InputText("SubImage", &elementSpriteGroupItemData->subImageName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                SubImage* subImage = ownerGroupData->imageSet->GetSubImage(elementSpriteGroupItemData->subImageName);
                elementSpriteGroupItem->SetSubRect(!subImage ? sf::IntRect() : subImage->GetRect(), false);
                elementSpriteGroupItemData->textureRect = sf::IntRect();
                RaiseDirty();
            }

            if (ImGui::InputInt4("TextureRect", &elementSpriteGroupItemData->textureRect))
            {
                elementSpriteGroupItem->SetSubRect(elementSpriteGroupItemData->textureRect, false);
                elementSpriteGroupItemData->subImageName = "";
                RaiseDirty();
            }
        }

        if (ImGui::Checkbox("Repeat Texture", &elementSpriteBaseData->repeatTexture))
        {
            elementSpriteBase->SetRepeatTexture(elementSpriteBaseData->repeatTexture);
            RaiseDirty();
        }

        if (ImGui::Checkbox("Flip Texture V", &elementSpriteBaseData->flipTextureV))
        {
            elementSpriteBase->SetFlipTextureV(elementSpriteBaseData->flipTextureV);
            RaiseDirty();
        }

        if (ImGui::Checkbox("Flip Texture H", &elementSpriteBaseData->flipTextureH))
        {
            elementSpriteBase->SetFlipTextureH(elementSpriteBaseData->flipTextureH);
            RaiseDirty();
        }

        if (ImGui::ColorEdit4("Color", &elementSpriteBaseData->color))
        {
            elementSpriteBase->SetColor(elementSpriteBaseData->color);
            RaiseDirty();
        }
    }
}

void ElementWidgetPanel::DisplayTreeNode(ElementData* node, int itemFlags)
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
        m_selectedElement = m_dataContext->elementFromData.at(node);
    }

    // Context menu.
    HandleContextMenu(node);

    if (isOpen)
    {
        if (ElementSpriteGroupData* nodeSpriteGroup = dynamic_cast<ElementSpriteGroupData*>(node))
        {
            const std::vector<ElementSpriteGroupItemData*>& components = nodeSpriteGroup->components;

            ImGuiTreeNodeFlags componentFlags = ImGuiTreeNodeFlags_Leaf;
            ImGui::TreeNodeEx("<Components>", componentFlags);

            for (size_t i = 0; i < components.size(); ++i)
            {
                ImGui::PushID((int)i);
                DisplayTreeNode(components[i], itemFlags);
                ImGui::PopID();
            }

            ImGui::TreePop();
        }

        for (size_t i = 0; i < children.size(); ++i)
        {
            ImGui::PushID((int)i);
            DisplayTreeNode(children[i], itemFlags);
            ImGui::PopID();
        }

        ImGui::TreePop();
    }
}

void ElementWidgetPanel::HandleContextMenu(ElementData* node)
{
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

        ImGui::EndPopup();
    }
}

void ElementWidgetPanel::AppendNewElement(ElementData* parentData, ElementData* elementData)
{
    Element* parent = m_dataContext->elementFromData.at(parentData);
    Element* element = InstanciateElement(elementData);

    parentData->children.push_back(elementData);
    parent->AddChild(element);
    m_dataContext->elementFromData.insert(std::make_pair(elementData, element));
    m_dataContext->dataFromElement.insert(std::make_pair(element, elementData));
}

void ElementWidgetPanel::AppendNewComponent(ElementSpriteGroupData* groupData, ElementSpriteGroupItemData* componentData)
{
    ElementSpriteGroup* group = dynamic_cast<ElementSpriteGroup*>(m_dataContext->elementFromData.at(groupData));
    ElementSpriteGroupItem* component = new ElementSpriteGroupItem;

    groupData->components.push_back(componentData);
    group->AddItem(component);
    m_dataContext->elementFromData.insert(std::make_pair(componentData, component));
    m_dataContext->dataFromElement.insert(std::make_pair(component, componentData));
}

}   //namespace gugu
