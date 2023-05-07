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
    std::map<Element*, BaseElementData*> dataFromElement;
    std::map<BaseElementData*, Element*> elementFromData;
    std::map<std::string, Element*> elementFromName;
};

struct ElementDataContext
{
    BaseElementData* data = nullptr;
    ElementDataBindings* bindings = nullptr;
    std::vector<const ElementWidget*> ancestorWidgets;
};

BaseElementData* InstanciateElementData(const pugi::xml_node& node);
BaseElementData* InstanciateElementData(std::string_view elementType);

Element* InstanciateElement(ElementData* data);
Element* InstanciateAndLoadElement(ElementDataContext& context, Element* parent);

}   // namespace gugu
