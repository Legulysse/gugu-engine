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
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/ImGuiUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

#include <SFML/Graphics/RectangleShape.hpp>

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
    , m_viewportRoot(nullptr)
    , m_zoomFactor(1.f)
    , m_showOrigin(true)
    , m_showBounds(true)
    , m_showAllBounds(false)
    , m_dataBindings(nullptr)
    , m_widgetRootData(nullptr)
    , m_widgetRootElement(nullptr)
    , m_selectedElementData(nullptr)
    , m_selectedElement(nullptr)
    , m_gizmoOrigin(nullptr)
    , m_gizmoBounds(nullptr)
    , m_generatorIndex(0)
    , m_tileTopSegment(false)
    , m_tileLeftSegment(false)
    , m_tileRightSegment(false)
    , m_tileBottomSegment(false)
{
    // Setup RenderViewport.
    m_renderViewport = new RenderViewport(true);
    m_renderViewport->SetSize(Vector2u(1280, 720));

    m_viewportRoot = m_renderViewport->GetRoot()->AddChild<Element>();
    m_viewportRoot->SetUnifiedSize(UDim2::SIZE_FULL);

    CreateGizmo();

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

    ImGui::Checkbox("Show Origin", &m_showOrigin);

    ImGui::SameLine();
    ImGui::Checkbox("Show Bounds", &m_showBounds);

    ImGui::SameLine();
    if (ImGui::Checkbox("Show All Bounds", &m_showAllBounds))
    {
        m_renderViewport->SetShowBounds(m_showAllBounds);
    }

    // Viewport.
    m_renderViewport->ImGuiBegin();
    UpdateGizmo();
    m_renderViewport->ImGuiEnd();
}

void ElementWidgetPanel::AddChildElement(BaseElementData* parentData, BaseElementData* newData)
{
    AddChildElement(parentData, newData, system::InvalidIndex);
}

void ElementWidgetPanel::AddChildElement(BaseElementData* parentData, BaseElementData* newData, size_t index)
{
    Element* parent = m_dataBindings->elementFromData.at(parentData);

    ElementDataContext context;
    context.data = newData;
    context.dataBindings = m_dataBindings;
    context.ancestorWidgets.push_back(m_elementWidget);
    Element* element = InstanciateAndLoadElement(context, parent);

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
    groupData->RefreshCache();
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

            // Remove element from owner if it is a component.
            ElementCompositeData* parentCompositeData = dynamic_cast<ElementCompositeData*>(parentData);
            if (parentCompositeData && StdVectorContains(parentCompositeData->components, elementData))
            {
                StdVectorRemove<BaseElementData*>(parentCompositeData->components, elementData);
                parentCompositeData->RefreshCache();
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

void ElementWidgetPanel::CreateGizmo()
{
    // Bounds
    {
        sf::Color color = sf::Color(0, 0, 255, 40);
        sf::VertexArray* vertices = new sf::VertexArray(sf::PrimitiveType::Triangles);

        vertices->append(sf::Vertex(Vector2::Zero_f, color));
        vertices->append(sf::Vertex(Vector2::Zero_f, color));
        vertices->append(sf::Vertex(Vector2::Zero_f, color));
        vertices->append(sf::Vertex(Vector2::Zero_f, color));
        vertices->append(sf::Vertex(Vector2::Zero_f, color));
        vertices->append(sf::Vertex(Vector2::Zero_f, color));

        m_gizmoBounds = m_renderViewport->GetRoot()->AddChild<ElementSFDrawable>();
        m_gizmoBounds->SetSFDrawable(vertices);
    }

    // Origin
    {
        Vector2f position = sf::Vector2f(0.5f, 0.5f);
        Vector2f size = sf::Vector2f(10.f, 10.f);
        sf::Color color = sf::Color(255, 0, 0, 200);
        sf::VertexArray* vertices = new sf::VertexArray(sf::PrimitiveType::Lines);

        vertices->append(sf::Vertex(Vector2f(position.x, -size.y), color));
        vertices->append(sf::Vertex(Vector2f(position.x, size.y + 1.f), color));
        vertices->append(sf::Vertex(Vector2f(-size.x, position.y), color));
        vertices->append(sf::Vertex(Vector2f(size.x + 1.f, position.y), color));

        m_gizmoOrigin = m_renderViewport->GetRoot()->AddChild<ElementSFDrawable>();
        m_gizmoOrigin->SetSFDrawable(vertices);
    }
}

void ElementWidgetPanel::UpdateGizmo()
{
    if (m_showOrigin && m_selectedElement)
    {
        m_gizmoOrigin->SetVisible(true);
        m_gizmoOrigin->SetPosition(m_selectedElement->TransformToGlobal(m_selectedElement->GetOrigin()));
    }
    else
    {
        m_gizmoOrigin->SetVisible(false);
    }

    if (m_showBounds && m_selectedElement)
    {
        Vector2f topLeft, topRight, bottomLeft, bottomRight;
        m_selectedElement->GetGlobalCorners(topLeft, topRight, bottomLeft, bottomRight);

        sf::VertexArray* vertices = static_cast<sf::VertexArray*>(m_gizmoBounds->GetSFDrawable());
        (*vertices)[0].position = topLeft - topLeft;
        (*vertices)[1].position = topRight - topLeft;
        (*vertices)[2].position = bottomLeft - topLeft;
        (*vertices)[3].position = topRight - topLeft;
        (*vertices)[4].position = bottomLeft - topLeft;
        (*vertices)[5].position = bottomRight - topLeft;

        m_gizmoBounds->SetVisible(true);
        m_gizmoBounds->SetPosition(topLeft);
    }
    else
    {
        m_gizmoBounds->SetVisible(false);
    }
}

}   //namespace gugu
