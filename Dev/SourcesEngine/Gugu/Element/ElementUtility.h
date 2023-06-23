#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <pugixml.hpp>

#include <map>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class BaseElementData;
    class ElementData;
    class ElementWidget;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ElementDataBindings
{
    std::map<const Element*, BaseElementData*> dataFromElement;
    std::map<const BaseElementData*, Element*> elementFromData;
};

struct ElementPathBindings
{
    // The root of an ElementWidget may be accessed through two different paths.
    // - The path to the widget, and the path to the root inside the widget.
    std::map<const std::string, Element*> elementFromPath;

    Element* GetElement(const std::string& path) const;
};

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
    BaseElementData* data = nullptr;
    ElementDataBindings* dataBindings = nullptr;
    ElementPathBindings* pathBindings = nullptr;
    std::vector<std::string> path;
    std::vector<const ElementWidget*> ancestorWidgets;
};

BaseElementData* InstanciateElementData(const pugi::xml_node& node);
BaseElementData* InstanciateElementData(std::string_view elementType);

Element* InstanciateElement(ElementData* data);
Element* InstanciateAndLoadElement(ElementDataContext& context, Element* parent);

}   // namespace gugu
