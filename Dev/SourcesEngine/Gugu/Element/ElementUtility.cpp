////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BaseElementData* InstanciateElementData(const pugi::xml_node& node)
{
    std::string_view elementType = node.attribute("type").value();
    return InstanciateElementData(elementType);
}

BaseElementData* InstanciateElementData(std::string_view elementType)
{
    BaseElementData* result = nullptr;

    if (StringEquals(elementType, "ElementWidget"))
    {
        result = new ElementWidgetData;
    }
    else if (StringEquals(elementType, "Element"))
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
    else if (StringEquals(elementType, "ElementText"))
    {
        result = new ElementTextData;
    }
    else if (StringEquals(elementType, "ElementButton"))
    {
        result = new ElementButtonData;
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
    else if (StringEquals(data->GetSerializedType(), "ElementText"))
    {
        result = new ElementText;
    }
    else if (StringEquals(data->GetSerializedType(), "ElementButton"))
    {
        result = new ElementButton;
    }

    return result;
}

Element* InstanciateAndLoadElement(ElementDataContext& context, Element* parent)
{
    if (!context.data)
        return nullptr;

    Element* result = nullptr;

    if (ElementWidgetData* elementWidgetData = dynamic_cast<ElementWidgetData*>(context.data))
    {
        result = elementWidgetData->widget->InstanciateWidget(context);

        result->SetParent(parent);

        // TODO: Load override data from elementWidgetData.
    }
    else if (ElementData* elementData = dynamic_cast<ElementData*>(context.data))
    {
        result = InstanciateElement(elementData);

        result->SetParent(parent);
        /*bool result =*/ result->LoadFromData(context);
        // TODO: handle bool return.
    }

    return result;
}

}   // namespace gugu
