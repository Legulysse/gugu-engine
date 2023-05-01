////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Resources/ElementWidget.h"
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
    , m_dataBindings(nullptr)
    , m_widgetRootData(nullptr)
    , m_widgetRootElement(nullptr)
    , m_selectedElementData(nullptr)
    , m_selectedElement(nullptr)
    , m_generatorIndex(0)
    , m_tileTopSegment(false)
    , m_tileLeftSegment(false)
    , m_tileRightSegment(false)
    , m_tileBottomSegment(false)
{
    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(1280, 720));

    RebuildHierarchy();
    SelectRootNode();
}

ElementWidgetPanel::~ElementWidgetPanel()
{
    m_widgetRootData = nullptr;
    m_widgetRootElement = nullptr;
    m_selectedElementData = nullptr;
    m_selectedElement = nullptr;

    SafeDelete(m_dataBindings);
    SafeDelete(m_renderViewport);
}

void ElementWidgetPanel::OnUndoRedo()
{
    RebuildHierarchy();
}

void ElementWidgetPanel::SelectRootNode()
{
    m_selectedElementData = m_widgetRootData;
    m_selectedElement = m_widgetRootElement;
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

void ElementWidgetPanel::AppendNewElement(ElementData* parentData, ElementData* elementData)
{
    Element* parent = m_dataBindings->elementFromData.at(parentData);
    Element* element = InstanciateElement(elementData);

    parentData->children.push_back(elementData);
    parent->AddChild(element);
    m_dataBindings->elementFromData.insert(std::make_pair(elementData, element));
    m_dataBindings->dataFromElement.insert(std::make_pair(element, elementData));
}

ElementSpriteGroupItem* ElementWidgetPanel::AppendNewComponent(ElementSpriteGroupData* groupData, ElementSpriteGroupItemData* componentData)
{
    ElementSpriteGroup* group = dynamic_cast<ElementSpriteGroup*>(m_dataBindings->elementFromData.at(groupData));
    ElementSpriteGroupItem* component = new ElementSpriteGroupItem;

    groupData->components.push_back(componentData);
    group->AddItem(component);
    m_dataBindings->elementFromData.insert(std::make_pair(componentData, component));
    m_dataBindings->dataFromElement.insert(std::make_pair(component, componentData));

    return component;
}

void ElementWidgetPanel::DeleteElement(ElementData* elementData)
{
    Element* element = m_dataBindings->elementFromData.at(elementData);

    if (Element* parent = element->GetParent())
    {
        // Remove element from parent children.
        ElementData* parentData = m_dataBindings->dataFromElement.at(parent);
        StdVectorRemove(parentData->children, elementData);

        // Remove element from owner if it is a SpriteGroup component.
        ElementSpriteGroupData* parentSpriteGroupData = dynamic_cast<ElementSpriteGroupData*>(parentData);
        ElementSpriteGroupItemData* elementSpriteGroupItemData = dynamic_cast<ElementSpriteGroupItemData*>(elementData);
        if (parentSpriteGroupData && elementSpriteGroupItemData)
        {
            StdVectorRemove(parentSpriteGroupData->components, elementSpriteGroupItemData);
        }

        // Remove element from owner if it is a Button component.
        ElementButtonData* parentButtonData = dynamic_cast<ElementButtonData*>(parentData);
        if (parentButtonData)
        {
            StdVectorRemove(parentButtonData->components, elementData);

            parentButtonData->RefreshCache();
        }
    }

    // Check if we are deleting an ancestor of the selected element.
    bool selectionDeleted = false;

    Element* ancestor = m_selectedElement;
    while (ancestor)
    {
        if (ancestor == element)
        {
            selectionDeleted = true;
            break;
        }

        ancestor = ancestor->GetParent();
    }

    if (selectionDeleted)
    {
        m_selectedElement = nullptr;
        m_selectedElementData = nullptr;
    }

    // Finalize.
    SafeDelete(elementData);

    RebuildHierarchy();
    RaiseDirty();
}

}   //namespace gugu
