#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <pugixml.hpp>

#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementData;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ElementParseContext
{
    pugi::xml_node node;
};

struct ElementSaveContext
{
    pugi::xml_node node;
};

struct ElementDataBindings
{
    std::map<Element*, ElementData*> dataFromElement;
    std::map<ElementData*, Element*> elementFromData;
    std::map<std::string, Element*> elementFromName;
};

struct ElementDataContext
{
    ElementData* data;
    ElementDataBindings* bindings;
};

ElementData* InstanciateElementData(const pugi::xml_node& node);
ElementData* InstanciateElementData(std::string_view elementType);
Element* InstanciateElement(ElementData* data);

}   // namespace gugu
