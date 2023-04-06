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

struct ElementDataContext
{
    ElementData* data;

    std::map<Element*, ElementData*> dataFromElement;
    std::map<ElementData*, Element*> elementFromData;
    std::map<std::string, Element*> elementFromName;
};

Element* InstanciateElement(const pugi::xml_node& node);
Element* InstanciateElement(std::string_view elementType);
ElementData* InstanciateElementData(const pugi::xml_node& node);
Element* InstanciateElement(ElementData* data);

}   // namespace gugu
