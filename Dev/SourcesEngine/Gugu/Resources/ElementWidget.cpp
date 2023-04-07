////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ElementWidget.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidget::ElementWidget()
    : m_data(new ElementData)
{
}

ElementWidget::~ElementWidget()
{
    Unload();
}

Element* ElementWidget::InstanciateWidget() const
{
    ElementDataContext context;
    return InstanciateWidget(context);
}

Element* ElementWidget::InstanciateWidget(ElementDataContext& context) const
{
    if (!m_data)
        return nullptr;

    if (Element* root = InstanciateElement(m_data))
    {
        context.data = m_data;
        root->LoadFromData(context);

        context.data = m_data;
        return root;
    }
    else
    {
        return nullptr;
    }
}

ElementData* ElementWidget::GetData() const
{
    return m_data;
}

void ElementWidget::Unload()
{
    SafeDelete(m_data);
}

bool ElementWidget::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("ElementWidget").child("Element");
    if (!rootNode)
        return false;

    if (ElementData* root = InstanciateElementData(rootNode))
    {
        ElementParseContext context;
        context.node = rootNode;
        root->LoadFromXml(context);
        m_data = root;
    }
    else
    {
        return false;
    }

    return true;
}

bool ElementWidget::SaveToXml(pugi::xml_document& document) const
{
    if (!m_data)
        return false;

    pugi::xml_node nodeRoot = document.append_child("ElementWidget");
    nodeRoot.append_attribute("serializationVersion") = 1;

    pugi::xml_node nodeRootElement = nodeRoot.append_child("Element");

    ElementSaveContext context;
    context.node = nodeRootElement;
    if (!m_data->SaveToXml(context))
        return false;

    return true;
}

EResourceType::Type ElementWidget::GetResourceType() const
{
    return EResourceType::ElementWidget;
}

void ElementWidget::GetDependencies(std::set<Resource*>& dependencies) const
{
}

void ElementWidget::OnDependencyRemoved(const Resource* removedDependency)
{
}

}   // namespace gugu
