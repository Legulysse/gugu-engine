#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <pugixml.hpp>

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

Element* InstanciateElement(const pugi::xml_node& node);
Element* InstanciateElement(std::string_view elementType);
ElementData* InstanciateElementData(const pugi::xml_node& node);

}   // namespace gugu
