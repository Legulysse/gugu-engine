////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ElementWidget.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementWidget::ElementWidget()
    : m_cache(nullptr)
{
}

ElementWidget::~ElementWidget()
{
    Unload();
}

Element* ElementWidget::InstanciateWidget() const
{
    if (!m_cache)
        return nullptr;

    pugi::xml_node rootNode = m_cache->child("ElementWidget").child("Element");
    if (!rootNode)
        return nullptr;

    if (Element* root = InstanciateElement(rootNode))
    {
        ElementParseContext context;
        context.node = rootNode;
        root->LoadFromXml(context);
        return root;
    }
    else
    {
        return nullptr;
    }
}

bool ElementWidget::UpdateFromInstance(const Element* instance)
{
    if (!instance)
        return nullptr;

    Unload();

    pugi::xml_document* document = new pugi::xml_document;

    pugi::xml_node nodeRoot = document->append_child("ElementWidget");
    nodeRoot.append_attribute("serializationVersion") = 1;

    pugi::xml_node nodeRootElement = nodeRoot.append_child("Element");

    ElementSaveContext context;
    context.node = nodeRootElement;
    if (!instance->SaveToXml(context))
    {
        SafeDelete(document);
        return false;
    }

    m_cache = document;
    return true;
}

void ElementWidget::ResetWidget()
{
    Element dummy;
    UpdateFromInstance(&dummy);
}

void ElementWidget::Unload()
{
    SafeDelete(m_cache);
}

bool ElementWidget::LoadFromFile()
{
    Unload();

    pugi::xml_document* document = new pugi::xml_document;
    pugi::xml_parse_result result = document->load_file(GetFileInfo().GetFilePath().c_str());
    if (!result)
    {
        SafeDelete(document);
        return false;
    }

    m_cache = document;
    return true;
}

bool ElementWidget::LoadFromString(const std::string& source)
{
    Unload();

    pugi::xml_document* document = new pugi::xml_document;
    pugi::xml_parse_result result = document->load_string(source.c_str());
    if (!result)
    {
        SafeDelete(document);
        return false;
    }

    m_cache = document;
    return true;
}

bool ElementWidget::SaveToFile() const
{
    if (!m_cache)
        return false;

    return m_cache->save_file(GetFileInfo().GetFilePath().c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
}

bool ElementWidget::SaveToString(std::string& result) const
{
    if (!m_cache)
        return false;

    xml::StringWriter buffer(&result);
    m_cache->save(buffer, "", pugi::format_no_declaration | pugi::format_raw, pugi::encoding_utf8);
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
