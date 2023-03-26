#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <pugixml.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
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

}   // namespace gugu
