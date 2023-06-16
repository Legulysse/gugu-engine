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
#include "Gugu/Debug/Logger.h"

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

    if (StdVectorContains(context.ancestorWidgets, this))
    {
        std::string ancestorsLog;
        for (auto ancestor : context.ancestorWidgets)
        {
            ancestorsLog += ancestor->GetFileInfo().GetFileName() + ", ";
        }

        ancestorsLog += GetFileInfo().GetFileName();
        GetLogEngine()->Print(ELog::Error, ELogEngine::Resources, StringFormat("ElementWidget ancestors create an infinite loop : {0}", ancestorsLog));

        return nullptr;
    }

    BaseElementData* backupData = context.data;
    context.data = m_data;
    context.ancestorWidgets.push_back(this);

    Element* root = InstanciateAndLoadElement(context, nullptr);

    context.ancestorWidgets.pop_back();
    context.data = backupData;
    return root;
}

bool ElementWidget::LoadElementFromWidget(Element* element) const
{
    if (!m_data || !element)
        return false;

    ElementDataContext context;
    context.ancestorWidgets.push_back(this);
    context.data = m_data;
    bool result = element->LoadFromData(context);

    return result;
}

BaseElementData* ElementWidget::GetRootData() const
{
    return m_data;
}

void ElementWidget::SetRootData(BaseElementData* data, bool deleteData)
{
    if (deleteData)
    {
        Unload();
    }

    m_data = data;
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

    if (BaseElementData* root = InstanciateElementData(rootNode))
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
    m_data->GetDependencies(dependencies);
}

void ElementWidget::OnDependencyRemoved(const Resource* removedDependency)
{
}

}   // namespace gugu
