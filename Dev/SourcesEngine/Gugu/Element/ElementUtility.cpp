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
#include "Gugu/Debug/Logger.h"

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

    std::string_view elementType = data->GetSerializedType();
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
    else if (StringEquals(elementType, "ElementText"))
    {
        result = new ElementText;
    }
    else if (StringEquals(elementType, "ElementButton"))
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
        if (elementWidgetData->widget == nullptr)
        {
            // Instantiate a default empty Element.
            result = new Element;
        }
        else
        {
            // There is no point in gathering data-to-element bindings in child widgets for now.
            // - They would generate duplicate entries on widgets including another widget multiple times.
            ElementDataBindings* bindingsBackup = context.bindings;
            context.bindings = nullptr;

            result = elementWidgetData->widget->InstanciateWidget(context);

            context.bindings = bindingsBackup;
        }

        if (result)
        {
            result->SetParent(parent);

            // TODO: Load override data from elementWidgetData.
            // - We cant use LoadFromData here since an ElementWidgetData is not an ElementData.

            if (context.bindings)
            {
                // Fill bindings informations.
                // - The instantiated Element will be referenced by both the ElementWidgetData and the widget root ElementData, but it will only reference the root ElementData.
                // - As a result, the Element will have no knowledge of the ElementWidget it originates from, and get attached directly to the provided parent.
                context.bindings->elementFromData.insert(std::make_pair(elementWidgetData, result));
                //context.bindings->dataFromElement.insert(std::make_pair(result, elementData));

                if (!elementWidgetData->name.empty())
                {
                    // TODO: handle multiple elements with same name ?
                    auto it = context.bindings->elementFromName.find(elementWidgetData->name);
                    if (it == context.bindings->elementFromName.end())
                    {
                        context.bindings->elementFromName.insert(std::make_pair(elementWidgetData->name, result));
                    }
                    else
                    {
                        GetLogEngine()->Print(ELog::Error, ELogEngine::Element, StringFormat("An ElementWidget contains several elements with the same name : {0}", elementWidgetData->name));
                    }
                }
            }

            /*bool result =*/ result->LoadChildrenFromData(context);
            // TODO: handle bool return.
        }
    }
    else if (ElementData* elementData = dynamic_cast<ElementData*>(context.data))
    {
        result = InstanciateElement(elementData);
        if (result)
        {
            result->SetParent(parent);
            /*bool result =*/ result->LoadFromData(context);
            // TODO: handle bool return.
        }
    }

    return result;
}

}   // namespace gugu
