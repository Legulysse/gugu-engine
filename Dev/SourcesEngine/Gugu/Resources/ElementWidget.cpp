////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ElementWidget.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidget::ElementWidget()
{
}

ElementWidget::~ElementWidget()
{
}

Element* ElementWidget::InstanciateWidget() const
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(GetFileInfo().GetFilePath().c_str());
    if (!result)
        return false;

    pugi::xml_node rootNode = document.child("ElementWidget").child("Element");
    if (!rootNode)
        return false;

    Element* root = InstanciateElement(rootNode);

    if (root)
    {
        ElementParseContext context;
        context.node = rootNode;
        root->LoadFromXml(context);
    }

    return root;
}

bool ElementWidget::SaveInstanceToFile(const Element* instance) const
{
    pugi::xml_document document;

    pugi::xml_node nodeRoot = document.append_child("ElementWidget");
    nodeRoot.append_attribute("serializationVersion") = 1;

    pugi::xml_node nodeRootElement = nodeRoot.append_child("Element");

    ElementSaveContext context;
    context.node = nodeRootElement;
    if (!instance->SaveToXml(context))
        return false;

    return document.save_file(GetFileInfo().GetFilePath().c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
}

bool ElementWidget::LoadInstanceFromString(const std::string& source, Element*& instance)
{
    return false;
}

bool ElementWidget::SaveInstanceToString(const Element* instance, std::string& result) const
{
    return false;
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
