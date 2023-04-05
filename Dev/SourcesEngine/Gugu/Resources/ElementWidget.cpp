////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/ElementWidget.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementData::~ElementData()
{
    ClearStdVector(children);
}

const std::string& ElementData::GetSerializedType() const
{
    static const std::string serializedType = "Element";
    return serializedType;
}

bool ElementData::LoadFromXml(ElementParseContext& context)
{
    bool result = LoadFromXmlImpl(context);

    if (pugi::xml_node childrenNode = context.node.child("Children"))
    {
        pugi::xml_node backupNode = context.node;

        for (pugi::xml_node childNode = childrenNode.child("Element"); childNode; childNode = childNode.next_sibling("Element"))
        {
            if (ElementData* child = InstanciateElementData(childNode))
            {
                children.push_back(child);

                context.node = childNode;
                result &= child->LoadFromXml(context);
            }
        }

        context.node = backupNode;
    }

    return result;
}

bool ElementData::SaveToXml(ElementSaveContext& context) const
{
    context.node.append_attribute("type").set_value(GetSerializedType().c_str());

    bool result = SaveToXmlImpl(context);

    if (!children.empty())
    {
        pugi::xml_node childrenNode = context.node.append_child("Children");
        pugi::xml_node backupNode = context.node;

        for (size_t i = 0; i < children.size(); ++i)
        {
            context.node = childrenNode.append_child("Element");
            result &= children[i]->SaveToXml(context);
        }

        context.node = backupNode;
    }

    return result;
}

bool ElementData::LoadFromXmlImpl(ElementParseContext& context)
{
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

    return true;
}

bool ElementData::SaveToXmlImpl(ElementSaveContext& context) const
{
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

    return true;
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

const std::string& ElementSpriteData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSprite";
    return serializedType;
}

bool ElementSpriteData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementSpriteBaseData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node textureNode = context.node.child("Texture"))
    {
        texture = GetResources()->GetTexture(textureNode.attribute("source").as_string(""));
    }

    return true;
}

bool ElementSpriteData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementSpriteBaseData::SaveToXmlImpl(context))
        return false;

    if (texture)
    {
        context.node.append_child("Texture").append_attribute("source").set_value(texture->GetID().c_str());
    }

    return true;
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

    return true;
}

bool ElementSpriteGroupItemData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementSpriteBaseData::SaveToXmlImpl(context))
        return false;

    return true;
}

ElementSpriteGroupData::~ElementSpriteGroupData()
{
    ClearStdVector(components);
}

const std::string& ElementSpriteGroupData::GetSerializedType() const
{
    static const std::string serializedType = "ElementSpriteGroup";
    return serializedType;
}

bool ElementSpriteGroupData::LoadFromXmlImpl(ElementParseContext& context)
{
    if (!ElementData::LoadFromXmlImpl(context))
        return false;

    if (pugi::xml_node nodeImageSet = context.node.child("ImageSet"))
    {
        imageSet = GetResources()->GetImageSet(nodeImageSet.attribute("source").as_string(""));
    }
    else if (pugi::xml_node textureNode = context.node.child("Texture"))
    {
        texture = GetResources()->GetTexture(textureNode.attribute("source").as_string(""));
    }

    if (pugi::xml_node nodeComponents = context.node.child("Components"))
    {
        pugi::xml_node backupNode = context.node;

        for (pugi::xml_node nodeComponent = nodeComponents.child("Component"); nodeComponent; nodeComponent = nodeComponent.next_sibling("Component"))
        {
            ElementSpriteGroupItemData* component = new ElementSpriteGroupItemData;

            // Parse default ElementSpriteBase data.
            context.node = nodeComponent;
            component->LoadFromXml(context);

            // Read additional SubImage data (TextureRect is handled in the ElementSpriteBase parser).
            if (imageSet)
            {
                // TODO: Actual node (SubImage value=""), probably need to pass ImageSet in parse context.
                component->subImage = imageSet->GetSubImage(nodeComponent.attribute("SubImage").as_string());
            }

            // Finalize.
            components.push_back(component);
        }

        context.node = backupNode;
    }

    return true;
}

bool ElementSpriteGroupData::SaveToXmlImpl(ElementSaveContext& context) const
{
    if (!ElementData::SaveToXmlImpl(context))
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

    if (!components.empty())
    {
        pugi::xml_node componentsNode = context.node.append_child("Components");
        pugi::xml_node backupNode = context.node;

        for (size_t i = 0; i < components.size(); ++i)
        {
            context.node = componentsNode.append_child("Component");
            result &= components[i]->SaveToXml(context);
        }

        context.node = backupNode;
    }

    return result;
}


ElementWidget::ElementWidget()
    : m_data(new ElementData)
{
}

ElementWidget::~ElementWidget()
{
    Unload();
}

Element* ElementWidget::InstanciateWidget() const
{
    if (!m_data)
        return nullptr;

    if (Element* root = InstanciateElement(m_data))
    {
        ElementDataContext context;
        context.data = m_data;
        root->LoadFromData(context);
        return root;
    }
    else
    {
        return nullptr;
    }
}

ElementData* ElementWidget::GetData() const
{
    return m_data;
}

void ElementWidget::Unload()
{
    SafeDelete(m_data);
}

bool ElementWidget::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("ElementWidget").child("Element");
    if (!rootNode)
        return false;

    if (ElementData* root = InstanciateElementData(rootNode))
    {
        ElementParseContext context;
        context.node = rootNode;
        root->LoadFromXml(context);
        m_data = root;
    }
    else
    {
        return false;
    }

    return true;
}

bool ElementWidget::SaveToXml(pugi::xml_document& document) const
{
    if (!m_data)
        return false;

    pugi::xml_node nodeRoot = document.append_child("ElementWidget");
    nodeRoot.append_attribute("serializationVersion") = 1;

    pugi::xml_node nodeRootElement = nodeRoot.append_child("Element");

    ElementSaveContext context;
    context.node = nodeRootElement;
    if (!m_data->SaveToXml(context))
        return false;

    return true;
}

EResourceType::Type ElementWidget::GetResourceType() const
{
    return EResourceType::ElementWidget;
}

void ElementWidget::GetDependencies(std::set<Resource*>& dependencies) const
{
}

void ElementWidget::OnDependencyRemoved(const Resource* removedDependency)
{
}

}   // namespace gugu
