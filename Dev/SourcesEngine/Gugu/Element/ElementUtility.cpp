////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Resources/ElementWidget.h"
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
    else if (StringEquals(elementType, "ElementSpriteGroup"))
    {
        result = new ElementSpriteGroup;
    }

    return result;
}

ElementData* InstanciateElementData(const pugi::xml_node& node)
{
    std::string_view elementType = node.attribute("type").value();
    ElementData* result = nullptr;

    if (StringEquals(elementType, "Element"))
    {
        result = new ElementData;
    }
    else if (StringEquals(elementType, "ElementSprite"))
    {
        result = new ElementSpriteData;
    }
    else if (StringEquals(elementType, "ElementSpriteGroup"))
    {
        result = new ElementSpriteGroupData;
    }

    return result;
}

Element* InstanciateElement(ElementData* data)
{
    if (!data)
        return nullptr;

    Element* result = nullptr;

    if (StringEquals(data->GetSerializedType(), "Element"))
    {
        result = new Element;
    }
    else if (StringEquals(data->GetSerializedType(), "ElementSprite"))
    {
        result = new ElementSprite;
    }
    else if (StringEquals(data->GetSerializedType(), "ElementSpriteGroup"))
    {
        result = new ElementSpriteGroup;
    }

    return result;
}

}   // namespace gugu
