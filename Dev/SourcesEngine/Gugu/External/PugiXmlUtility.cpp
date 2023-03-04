////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/UDim.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
xml_string_writer::xml_string_writer(std::string& target)
{
    m_target = &target;
}

void xml_string_writer::write(const void* data, size_t size)
{
    m_target->append(static_cast<const char*>(data), size);
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_bool(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_int(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_uint(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, size_t& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_ullong(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_float(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_double(value);
        return true;
    }

    return false;
}

bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_string(value.c_str());
        return true;
    }

    return false;
}

Vector2i XmlReadVector2i(const pugi::xml_node& node, const Vector2i& defaultValue)
{
    return Vector2i(node.attribute("x").as_int(defaultValue.x), node.attribute("y").as_int(defaultValue.y));
}

Vector2f XmlReadVector2f(const pugi::xml_node& node, const Vector2f& defaultValue)
{
    return Vector2f(node.attribute("x").as_float(defaultValue.x), node.attribute("y").as_float(defaultValue.y));
}

void XmlParseVector2i(const pugi::xml_node& node, Vector2i& value, const Vector2i& defaultValue)
{
    value.x = node.attribute("x").as_int(defaultValue.x);
    value.y = node.attribute("y").as_int(defaultValue.y);
}

void XmlParseVector2f(const pugi::xml_node& node, Vector2f& value, const Vector2f& defaultValue)
{
    value.x = node.attribute("x").as_float(defaultValue.x);
    value.y = node.attribute("y").as_float(defaultValue.y);
}

bool XmlTryParseVector2i(const pugi::xml_node& node, Vector2i& value)
{
    if (!node)
        return false;

    XmlParseVector2i(node, value, value);
    return true;
}

bool XmlTryParseVector2f(const pugi::xml_node& node, Vector2f& value)
{
    if (!node)
        return false;

    XmlParseVector2f(node, value, value);
    return true;
}

sf::IntRect XmlReadRect(const pugi::xml_node& node)
{
    return sf::Rect(node.attribute("x").as_int(0)
                    , node.attribute("y").as_int(0)
                    , node.attribute("w").as_int(0)
                    , node.attribute("h").as_int(0));
}

void XmlParseRect(const pugi::xml_node& node, sf::IntRect& value)
{
    value.left = node.attribute("x").as_int(value.left);
    value.top = node.attribute("y").as_int(value.top);
    value.width = node.attribute("w").as_int(value.width);
    value.height = node.attribute("h").as_int(value.height);
}

bool XmlTryParseRect(const pugi::xml_node& node, sf::IntRect& value)
{
    if (!node)
        return false;

    XmlParseRect(node, value);
    return true;
}

void XmlParseUDim2(const pugi::xml_node& node, UDim2& value)
{
    value.x.relative = node.attribute("xRel").as_float(value.x.relative);
    value.y.relative = node.attribute("yRel").as_float(value.y.relative);
    value.x.absolute = node.attribute("xAbs").as_float(value.x.absolute);
    value.y.absolute = node.attribute("yAbs").as_float(value.y.absolute);
}

bool XmlTryParseUDim2(const pugi::xml_node& node, UDim2& value)
{
    if (!node)
        return false;

    XmlParseUDim2(node, value);
    return true;
}

void XmlWriteVector2i(pugi::xml_node node, const Vector2i& value)
{
    node.append_attribute("x").set_value(value.x);
    node.append_attribute("y").set_value(value.y);
}

void XmlWriteVector2f(pugi::xml_node node, const Vector2f& value)
{
    node.append_attribute("x").set_value(value.x);
    node.append_attribute("y").set_value(value.y);
}

void XmlWriteRect(pugi::xml_node node, const sf::IntRect& value)
{
    node.append_attribute("x").set_value(value.left);
    node.append_attribute("y").set_value(value.top);
    node.append_attribute("w").set_value(value.width);
    node.append_attribute("h").set_value(value.height);
}

void XmlWriteUDim2(pugi::xml_node node, const UDim2& value)
{
    node.append_attribute("xRel").set_value(value.x.relative);
    node.append_attribute("yRel").set_value(value.y.relative);
    node.append_attribute("xAbs").set_value(value.x.absolute);
    node.append_attribute("yAbs").set_value(value.y.absolute);
}

}   // namespace gugu
