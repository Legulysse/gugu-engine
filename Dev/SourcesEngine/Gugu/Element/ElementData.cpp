////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementData.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Resources/ElementWidget.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/String.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BaseElementData::~BaseElementData()
{
    ClearStdVector(children);
}

bool BaseElementData::LoadFromXml(ElementParseContext& context)
{
    xml::TryParseAttribute(context.node, "name", name);

    bool result = LoadFromXmlImpl(context);

    if (pugi::xml_node childrenNode = context.node.child("Children"))
    {
        pugi::xml_node backupNode = context.node;

        for (pugi::xml_node childNode = childrenNode.child("Element"); childNode; childNode = childNode.next_sibling("Element"))
        {
            if (BaseElementData* child = InstanciateElementData(childNode))
            {
                children.push_back(child);
                child->parent = this;

                context.node = childNode;
                result &= child->LoadFromXml(context);
            }
        }

        context.node = backupNode;
    }

    return result;
}

bool BaseElementData::SaveToXml(ElementSaveContext& context) const
{
    bool result = true;

    // Add new node
    pugi::xml_node contextBackupNode = context.node;
    context.node = context.node.append_child("Element");

    context.node.append_attribute("type").set_value(GetSerializedType().c_str());

    if (!name.empty())
    {
        context.node.append_attribute("name").set_value(name.c_str());
    }

    result = SaveToXmlImpl(context);

    if (!children.empty())
    {
        pugi::xml_node backupNode = context.node;
        context.node = context.node.append_child("Children");

        for (size_t i = 0; i < children.size(); ++i)
        {
            result &= children[i]->SaveToXml(context);
        }

        context.node = backupNode;
    }

    context.node = contextBackupNode;
    return result;
}

const std::string& ElementWidgetInstanceData::GetSerializedType() const
{
    static const std::string serializedType = "ElementWidgetInstance";
    return serializedType;
}

bool ElementWidgetInstanceData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (pugi::xml_attribute sourceWidget = context.node.attribute("widget"))
    {
        widget = GetResources()->GetElementWidget(sourceWidget.as_string(""));
    }

    overrideOrigin = xml::TryParseUDim2(context.node.child("UOrigin"), dimOrigin);
    overridePosition = xml::TryParseUDim2(context.node.child("UPosition"), dimPosition);
    overrideSize = xml::TryParseUDim2(context.node.child("USize"), dimSize);

    overrideRotation = xml::TryParseAttribute(context.node.child("Rotation"), "value", rotation);
    overrideFlipV = xml::TryParseAttribute(context.node.child("FlipV"), "value", flipV);
    overrideFlipH = xml::TryParseAttribute(context.node.child("FlipH"), "value", flipH);

    return true;
}

bool ElementWidgetInstanceData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (widget)
    {
        context.node.append_attribute("widget").set_value(widget->GetID().c_str());
    }

    if (overrideOrigin)
    {
        xml::WriteUDim2(context.node.append_child("UOrigin"), dimOrigin);
    }

    if (overridePosition)
    {
        xml::WriteUDim2(context.node.append_child("UPosition"), dimPosition);
    }

    if (overrideSize)
    {
        xml::WriteUDim2(context.node.append_child("USize"), dimSize);
    }

    if (overrideRotation)
    {
        context.node.append_child("Rotation").append_attribute("value").set_value(rotation);
    }

    if (overrideFlipV)
    {
        context.node.append_child("FlipV").append_attribute("value").set_value(flipV);
    }

    if (overrideFlipH)
    {
        context.node.append_child("FlipH").append_attribute("value").set_value(flipH);
    }

    return true;
}

void ElementWidgetInstanceData::GetDependencies(std::set<Resource*>& dependencies) const
{
    if (widget && !StdSetContains<Resource*>(dependencies, widget))
    {
        dependencies.insert(widget);

        // TODO: Do I need an option to retrieve or not indirect dependencies ?
        widget->GetDependencies(dependencies);
    }
}

ElementData::~ElementData()
{
}

const std::string& ElementData::GetSerializedType() const
{
    static const std::string serializedType = "Element";
    return serializedType;
}

bool ElementData::LoadFromXmlImpl(ElementParseContext& context)
{
    bool result = true;

    if (xml::TryParseUDim2(context.node.child("UOrigin"), dimOrigin))
    {
        useDimOrigin = true;
    }
    else
    {
        xml::TryParseVector2f(context.node.child("Origin"), origin);
    }

    if (xml::TryParseUDim2(context.node.child("UPosition"), dimPosition))
    {
        useDimPosition = true;
    }
    else 
    {
        xml::TryParseVector2f(context.node.child("Position"), position);
    }

    if (xml::TryParseUDim2(context.node.child("USize"), dimSize))
    {
        useDimSize = true;
    }
    else
    {
        xml::TryParseVector2f(context.node.child("Size"), size);
    }

    xml::TryParseAttribute(context.node.child("Rotation"), "value", rotation);
    xml::TryParseAttribute(context.node.child("FlipV"), "value", flipV);
    xml::TryParseAttribute(context.node.child("FlipH"), "value", flipH);

    return result;
}

bool ElementData::SaveToXmlImpl(ElementSaveContext& context) const
{
    bool result = true;

    if (useDimOrigin)
    {
        xml::WriteUDim2(context.node.append_child("UOrigin"), dimOrigin);
    }
    else if (origin != Vector2::Zero_f)
    {
        xml::WriteVector2f(context.node.append_child("Origin"), origin);
    }

    if (useDimPosition)
    {
        xml::WriteUDim2(context.node.append_child("UPosition"), dimPosition);
    }
    else if (position != Vector2::Zero_f)
    {
        xml::WriteVector2f(context.node.append_child("Position"), position);
    }

    if (useDimSize)
    {
        xml::WriteUDim2(context.node.append_child("USize"), dimSize);
    }
    else if (size != Vector2::Zero_f)
    {
        xml::WriteVector2f(context.node.append_child("Size"), size);
    }

    if (!ApproxEqualToZero(rotation, math::Epsilon3))
    {
        context.node.append_child("Rotation").append_attribute("value").set_value(rotation);
    }

    if (flipV)
    {
        context.node.append_child("FlipV").append_attribute("value").set_value(flipV);
    }

    if (flipH)
    {
        context.node.append_child("FlipH").append_attribute("value").set_value(flipH);
    }

    return result;
}

void ElementData::GetDependencies(std::set<Resource*>& dependencies) const
{
    for (auto child : children)
    {
        child->GetDependencies(dependencies);
    }
}

ElementCompositeData::~ElementCompositeData()
{
    ClearStdVector(components);
}

void ElementCompositeData::RefreshCache()
{
}

bool ElementCompositeData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node nodeComponents = context.node.child("Components"))
    {
        pugi::xml_node backupNode = context.node;

        for (pugi::xml_node nodeComponent = nodeComponents.child("Element"); nodeComponent; nodeComponent = nodeComponent.next_sibling("Element"))
        {
            if (BaseElementData* component = InstanciateElementData(nodeComponent))
            {
                context.node = nodeComponent;
                component->LoadFromXml(context);

                components.push_back(component);
                component->parent = this;
            }
        }

        context.node = backupNode;
    }

    RefreshCache();

    return true;
}

bool ElementCompositeData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementData::SaveToXmlImpl(context))
        return false;

    bool result = true;

    if (!components.empty())
    {
        pugi::xml_node backupNode = context.node;
        context.node = context.node.append_child("Components");

        for (size_t i = 0; i < components.size(); ++i)
        {
            result &= components[i]->SaveToXml(context);
        }

        context.node = backupNode;
    }

    return result;
}

void ElementCompositeData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementData::GetDependencies(dependencies);

    for (auto component : components)
    {
        component->GetDependencies(dependencies);
    }
}

bool ElementSpriteBaseData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementData::LoadFromXmlImpl(context))
        return false;

    xml::TryParseRect(context.node.child("TextureRect"), textureRect);
    xml::TryParseColor(context.node.child("Color"), color);
    xml::TryParseAttribute(context.node.child("RepeatTexture"), "value", repeatTexture);
    xml::TryParseAttribute(context.node.child("FlipTextureV"), "value", flipTextureV);
    xml::TryParseAttribute(context.node.child("FlipTextureH"), "value", flipTextureH);

    return true;
}

bool ElementSpriteBaseData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementData::SaveToXmlImpl(context))
        return false;

    if (textureRect != sf::IntRect())
    {
        xml::WriteRect(context.node.append_child("TextureRect"), textureRect);
    }

    if (color != sf::Color::White)
    {
        xml::WriteColor(context.node.append_child("Color"), color);
    }

    if (repeatTexture)
    {
        context.node.append_child("RepeatTexture").append_attribute("value").set_value(repeatTexture);
    }

    if (flipTextureV)
    {
        context.node.append_child("FlipTextureV").append_attribute("value").set_value(flipTextureV);
    }

    if (flipTextureH)
    {
        context.node.append_child("FlipTextureH").append_attribute("value").set_value(flipTextureH);
    }

    return true;
}

void ElementSpriteBaseData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementData::GetDependencies(dependencies);
}

const std::string& ElementSpriteData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSprite";
    return serializedType;
}

bool ElementSpriteData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementSpriteBaseData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node nodeImageSet = context.node.child("ImageSet"))
    {
        imageSet = GetResources()->GetImageSet(nodeImageSet.attribute("source").as_string(""));

        if (pugi::xml_node subImageNode = context.node.child("SubImage"))
        {
            subImageName = subImageNode.attribute("name").as_string("");
        }
    }
    else if (pugi::xml_node textureNode = context.node.child("Texture"))
    {
        texture = GetResources()->GetTexture(textureNode.attribute("source").as_string(""));
    }

    return true;
}

bool ElementSpriteData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementSpriteBaseData::SaveToXmlImpl(context))
        return false;

    if (imageSet)
    {
        context.node.append_child("ImageSet").append_attribute("source").set_value(imageSet->GetID().c_str());

        if (!subImageName.empty())
        {
            context.node.append_child("SubImage").append_attribute("name").set_value(subImageName.c_str());
        }
    }
    else if (texture)
    {
        context.node.append_child("Texture").append_attribute("source").set_value(texture->GetID().c_str());
    }

    return true;
}

void ElementSpriteData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementSpriteBaseData::GetDependencies(dependencies);

    if (imageSet)
    {
        dependencies.insert(imageSet);

        // TODO: Do I need an option to retrieve or not indirect dependencies ?
        imageSet->GetDependencies(dependencies);
    }

    if (texture)
    {
        dependencies.insert(texture);
    }
}

const std::string& ElementSpriteGroupItemData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSpriteGroupItem";
    return serializedType;
}

bool ElementSpriteGroupItemData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementSpriteBaseData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node subImageNode = context.node.child("SubImage"))
    {
        subImageName = subImageNode.attribute("name").as_string("");
    }

    return true;
}

bool ElementSpriteGroupItemData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementSpriteBaseData::SaveToXmlImpl(context))
        return false;

    if (!subImageName.empty())
    {
        context.node.append_child("SubImage").append_attribute("name").set_value(subImageName.c_str());
    }

    return true;
}

void ElementSpriteGroupItemData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementSpriteBaseData::GetDependencies(dependencies);
}

void ElementSpriteGroupData::RefreshCache()
{
    ElementCompositeData::RefreshCache();

    cacheSpriteGroupComponents.clear();

    for (const auto& component : components)
    {
        cacheSpriteGroupComponents.push_back(dynamic_cast<ElementSpriteGroupItemData*>(component));
    }
}

const std::string& ElementSpriteGroupData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSpriteGroup";
    return serializedType;
}

bool ElementSpriteGroupData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementCompositeData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node nodeImageSet = context.node.child("ImageSet"))
    {
        imageSet = GetResources()->GetImageSet(nodeImageSet.attribute("source").as_string(""));
    }
    else if (pugi::xml_node textureNode = context.node.child("Texture"))
    {
        texture = GetResources()->GetTexture(textureNode.attribute("source").as_string(""));
    }

    return true;
}

bool ElementSpriteGroupData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementCompositeData::SaveToXmlImpl(context))
        return false;

    bool result = true;

    if (imageSet)
    {
        context.node.append_child("ImageSet").append_attribute("source").set_value(imageSet->GetID().c_str());
    }
    else if (texture)
    {
        context.node.append_child("Texture").append_attribute("source").set_value(texture->GetID().c_str());
    }

    return result;
}

void ElementSpriteGroupData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementCompositeData::GetDependencies(dependencies);

    if (imageSet)
    {
        dependencies.insert(imageSet);

        // TODO: Do I need an option to retrieve or not indirect dependencies ?
        imageSet->GetDependencies(dependencies);
    }

    if (texture)
    {
        dependencies.insert(texture);
    }
}

const std::string& ElementTextData::GetSerializedType() const
{
    static const std::string serializedType = "ElementText";
    return serializedType;
}

bool ElementTextData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementData::LoadFromXmlImpl(context))
        return false;

    static const std::map<ETextResizeRule::Type, std::string> resizeRuleEnumToString = {
        { ETextResizeRule::FixedSize, "FixedSize" },
        { ETextResizeRule::FitSize, "FitSize" },
        { ETextResizeRule::FitHeight, "FitHeight" },
        { ETextResizeRule::FitScale, "FitScale" },
    };

    static const std::map<std::string, ETextResizeRule::Type> resizeRuleStringToEnum = {
        { "FixedSize", ETextResizeRule::FixedSize },
        { "FitSize", ETextResizeRule::FitSize },
        { "FitHeight", ETextResizeRule::FitHeight },
        { "FitScale", ETextResizeRule::FitScale },
    };

    if (pugi::xml_node fontNode = context.node.child("Font"))
    {
        font = GetResources()->GetFont(fontNode.attribute("source").as_string(""));
    }

    xml::TryParseAttribute(context.node.child("Text"), "value", text);
    xml::TryParseAttribute(context.node.child("Multiline"), "value", multiline);

    std::string resizeRuleString = resizeRuleEnumToString.at(resizeRule);
    xml::TryParseAttribute(context.node.child("ResizeRule"), "value", resizeRuleString);
    if (resizeRuleStringToEnum.find(resizeRuleString) != resizeRuleStringToEnum.end())
    {
        resizeRule = resizeRuleStringToEnum.at(resizeRuleString);
    }

    return true;
}

bool ElementTextData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementData::SaveToXmlImpl(context))
        return false;

    static const std::map<ETextResizeRule::Type, std::string> resizeRuleEnumToString = {
        { ETextResizeRule::FixedSize, "FixedSize" },
        { ETextResizeRule::FitSize, "FitSize" },
        { ETextResizeRule::FitHeight, "FitHeight" },
        { ETextResizeRule::FitScale, "FitScale" },
    };

    if (font)
    {
        context.node.append_child("Font").append_attribute("source").set_value(font->GetID().c_str());
    }

    if (!text.empty())
    {
        context.node.append_child("Text").append_attribute("value").set_value(text.c_str());
    }

    if (multiline)
    {
        context.node.append_child("Multiline").append_attribute("value").set_value(multiline);
    }

    if (resizeRule != ETextResizeRule::FitSize)
    {
        context.node.append_child("ResizeRule").append_attribute("value").set_value(resizeRuleEnumToString.at(resizeRule).c_str());
    }

    return true;
}

void ElementTextData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementData::GetDependencies(dependencies);

    if (font)
    {
        dependencies.insert(font);
    }
}

void ElementButtonData::RefreshCache()
{
    commonComponent = nullptr;
    idleStateComponent = nullptr;
    focusedStateComponent = nullptr;
    disabledStateComponent = nullptr;

    for (auto component : components)
    {
        if (StringEquals(component->name, "Common"))
        {
            commonComponent = component;
        }
        else if (StringEquals(component->name, "Idle"))
        {
            idleStateComponent = component;
        }
        else if (StringEquals(component->name, "Focused"))
        {
            focusedStateComponent = component;
        }
        else if (StringEquals(component->name, "Disabled"))
        {
            disabledStateComponent = component;
        }
    }
}

const std::string& ElementButtonData::GetSerializedType() const
{
    static const std::string serializedType = "ElementButton";
    return serializedType;
}

bool ElementButtonData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementCompositeData::LoadFromXmlImpl(context))
        return false;

    return true;
}

bool ElementButtonData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementCompositeData::SaveToXmlImpl(context))
        return false;

    bool result = true;
    return result;
}

void ElementButtonData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementCompositeData::GetDependencies(dependencies);
}

void ElementCheckboxData::RefreshCache()
{
    commonComponent = nullptr;
    idleStateComponent = nullptr;
    focusedStateComponent = nullptr;
    disabledStateComponent = nullptr;
    checkedComponent = nullptr;

    for (auto component : components)
    {
        if (StringEquals(component->name, "Common"))
        {
            commonComponent = component;
        }
        else if (StringEquals(component->name, "Idle"))
        {
            idleStateComponent = component;
        }
        else if (StringEquals(component->name, "Focused"))
        {
            focusedStateComponent = component;
        }
        else if (StringEquals(component->name, "Disabled"))
        {
            disabledStateComponent = component;
        }
        else if (StringEquals(component->name, "Checked"))
        {
            checkedComponent = component;
        }
    }
}

const std::string& ElementCheckboxData::GetSerializedType() const
{
    static const std::string serializedType = "ElementCheckbox";
    return serializedType;
}

bool ElementCheckboxData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementCompositeData::LoadFromXmlImpl(context))
        return false;

    return true;
}

bool ElementCheckboxData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementCompositeData::SaveToXmlImpl(context))
        return false;

    bool result = true;
    return result;
}

void ElementCheckboxData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementCompositeData::GetDependencies(dependencies);
}

void ElementSliderData::RefreshCache()
{
    idleStateComponent = nullptr;
    focusedStateComponent = nullptr;
    disabledStateComponent = nullptr;
    cursorComponent = nullptr;

    for (auto component : components)
    {
        if (StringEquals(component->name, "Idle"))
        {
            idleStateComponent = component;
        }
        else if (StringEquals(component->name, "Focused"))
        {
            focusedStateComponent = component;
        }
        else if (StringEquals(component->name, "Disabled"))
        {
            disabledStateComponent = component;
        }
        else if (StringEquals(component->name, "Cursor"))
        {
            cursorComponent = component;
        }
    }
}

const std::string& ElementSliderData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSlider";
    return serializedType;
}

bool ElementSliderData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementCompositeData::LoadFromXmlImpl(context))
        return false;

    return true;
}

bool ElementSliderData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementCompositeData::SaveToXmlImpl(context))
        return false;

    bool result = true;
    return result;
}

void ElementSliderData::GetDependencies(std::set<Resource*>& dependencies) const
{
    ElementCompositeData::GetDependencies(dependencies);
}

}   // namespace gugu
