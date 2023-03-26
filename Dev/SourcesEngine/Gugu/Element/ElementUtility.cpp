////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Element* InstanciateElement(const pugi::xml_node& node)
{
    return InstanciateElement(node.attribute("type").value());
}

Element* InstanciateElement(std::string_view elementType)
{
    Element* result = nullptr;

    if (StringEquals(elementType, "Element"))
    {
        result = new Element;
    }
    else if (StringEquals(elementType, "ElementSprite"))
    {
        result = new ElementSprite;
    }

    return result;
}

}   // namespace gugu
