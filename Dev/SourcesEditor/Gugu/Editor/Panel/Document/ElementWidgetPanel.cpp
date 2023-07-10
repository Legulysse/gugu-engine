////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/Document/ElementWidgetPanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Core/EditorClipboard.h"
#include "Gugu/Editor/Widget/RenderViewport.h"

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/ImGuiUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// Constants

namespace gugu
{
    namespace impl
    {
        const std::string ClipboardElementDataContentType = "ElementData";
    }
}

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidgetPanel::ElementWidgetPanel(ElementWidget* resource)
    : DocumentPanel(resource)
    , m_elementWidget(resource)
    , m_renderViewport(nullptr)
    , m_zoomFactor(1.f)
    , m_showBounds(false)
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

    if (ImGui::Checkbox("Show Bounds", &m_showBounds))
    {
        m_renderViewport->SetShowBounds(m_showBounds);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    m_renderViewport->ImGuiEnd();
}

void ElementWidgetPanel::AddChildElement(BaseElementData* parentData, BaseElementData* newData)
{
    AddChildElement(parentData, newData, system::InvalidIndex);
}

void ElementWidgetPanel::AddChildElement(BaseElementData* parentData, BaseElementData* newData, size_t index)
{
    Element* parent = m_dataBindings->elementFromData.at(parentData);

    Element* element = nullptr;
    if (ElementData* elementData = dynamic_cast<ElementData*>(newData))
    {
        element = InstanciateElement(elementData);
    }
    else if (ElementWidgetData* elementWidgetData = dynamic_cast<ElementWidgetData*>(newData))
    {
        // Add a default empty Element.
        element = new Element;
    }

    if (index == system::InvalidIndex)
    {
        parentData->children.push_back(newData);
        newData->parent = parentData;

        if (element)
        {
            parent->AddChild(element);
        }
    }
    else
    {
        StdVectorInsertAt(parentData->children, index, newData);
        newData->parent = parentData;

        if (element)
        {
            parent->InsertChild(element, index);
        }
    }

    if (element)
    {
        m_dataBindings->elementFromData.insert(std::make_pair(newData, element));
        m_dataBindings->dataFromElement.insert(std::make_pair(element, newData));
    }
}

void ElementWidgetPanel::InsertElement(BaseElementData* referenceData, BaseElementData* newData)
{
    if (BaseElementData* parentData = referenceData->parent)
    {
        size_t index = StdVectorIndexOf(parentData->children, referenceData);

        AddChildElement(parentData, newData, index);
    }
    else
    {
        // Safety.
        SafeDelete(newData);
    }
}

ElementSpriteGroupItem* ElementWidgetPanel::AppendNewComponent(ElementSpriteGroupData* groupData, ElementSpriteGroupItemData* componentData)
{
    ElementSpriteGroup* group = dynamic_cast<ElementSpriteGroup*>(m_dataBindings->elementFromData.at(groupData));
    ElementSpriteGroupItem* component = new ElementSpriteGroupItem;

    groupData->components.push_back(componentData);
    componentData->parent = groupData;

    group->AddItem(component);

    m_dataBindings->elementFromData.insert(std::make_pair(componentData, component));
    m_dataBindings->dataFromElement.insert(std::make_pair(component, componentData));

    return component;
}

void ElementWidgetPanel::DeleteElement(BaseElementData* elementData)
{
    // Special case : we may be deleting the root node through the Replace command.
    if (elementData != m_widgetRootData)
    {
        if (BaseElementData* parentData = elementData->parent)
        {
            // Remove element from parent children.
            StdVectorRemove<BaseElementData*>(parentData->children, elementData);

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
                StdVectorRemove<BaseElementData*>(parentButtonData->components, elementData);

                parentButtonData->RefreshCache();
            }
        }
    }

    // Check if we are deleting an ancestor of the selected element.
    bool selectionDeleted = false;

    BaseElementData* ancestor = m_selectedElementData;
    while (ancestor)
    {
        if (ancestor == elementData)
        {
            selectionDeleted = true;
            break;
        }

        ancestor = ancestor->parent;
    }

    if (selectionDeleted)
    {
        m_selectedElement = nullptr;
        m_selectedElementData = nullptr;
    }

    // Finalize.
    SafeDelete(elementData);

    RebuildHierarchy();
}

void ElementWidgetPanel::CopyElementToClipboard(BaseElementData* elementData)
{
    pugi::xml_document xmlDocument;

    ElementSaveContext saveContext;
    saveContext.node = xmlDocument;
    elementData->SaveToXml(saveContext);

    std::string result = xml::SaveDocumentToString(xmlDocument);

    GetEditorClipboard()->SetStringContent(impl::ClipboardElementDataContentType, result);
}

void ElementWidgetPanel::PasteElementFromClipboard(BaseElementData* parentData)
{
    PasteElementFromClipboard(parentData, system::InvalidIndex);
}

void ElementWidgetPanel::PasteElementFromClipboard(BaseElementData* parentData, size_t index)
{
    if (GetEditorClipboard()->contentType != impl::ClipboardElementDataContentType)
        return;

    std::string clipboard = GetEditorClipboard()->stringContent;

    pugi::xml_document xmlDocument = xml::ParseDocumentFromString(clipboard);
    pugi::xml_node xmlRoot = xmlDocument.first_child();

    BaseElementData* newNode = InstanciateElementData(xmlRoot);

    ElementParseContext parseContext;
    parseContext.node = xmlRoot;
    newNode->LoadFromXml(parseContext);

    AddChildElement(parentData, newNode, index);
    RaiseDirty();
}

}   //namespace gugu
