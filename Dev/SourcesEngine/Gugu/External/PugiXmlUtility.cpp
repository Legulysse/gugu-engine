////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

namespace xml {

StringWriter::StringWriter(std::string* target)
{
    m_target = target;
}

void StringWriter::write(const void* data, size_t size)
{
    m_target->append(static_cast<const char*>(data), size);
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_bool(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_int(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_uint(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, size_t& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_ullong(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_float(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_double(value);
        return true;
    }

    return false;
}

bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value)
{
    if (pugi::xml_attribute attribute = node.attribute(attributeName.c_str()))
    {
        value = attribute.as_string(value.c_str());
        return true;
    }

    return false;
}

Vector2i ReadVector2i(const pugi::xml_node& node, const Vector2i& defaultValue)
{
    return Vector2i(node.attribute("x").as_int(defaultValue.x), node.attribute("y").as_int(defaultValue.y));
}

Vector2f ReadVector2f(const pugi::xml_node& node, const Vector2f& defaultValue)
{
    return Vector2f(node.attribute("x").as_float(defaultValue.x), node.attribute("y").as_float(defaultValue.y));
}

void ParseVector2i(const pugi::xml_node& node, Vector2i& value, const Vector2i& defaultValue)
{
    value.x = node.attribute("x").as_int(defaultValue.x);
    value.y = node.attribute("y").as_int(defaultValue.y);
}

void ParseVector2f(const pugi::xml_node& node, Vector2f& value, const Vector2f& defaultValue)
{
    value.x = node.attribute("x").as_float(defaultValue.x);
    value.y = node.attribute("y").as_float(defaultValue.y);
}

bool TryParseVector2i(const pugi::xml_node& node, Vector2i& value)
{
    if (!node)
        return false;

    ParseVector2i(node, value, value);
    return true;
}

bool TryParseVector2f(const pugi::xml_node& node, Vector2f& value)
{
    if (!node)
        return false;

    ParseVector2f(node, value, value);
    return true;
}

sf::IntRect ReadRect(const pugi::xml_node& node, const sf::IntRect& defaultValue)
{
    return sf::Rect(node.attribute("x").as_int(defaultValue.left)
                    , node.attribute("y").as_int(defaultValue.top)
                    , node.attribute("w").as_int(defaultValue.width)
                    , node.attribute("h").as_int(defaultValue.height));
}

void ParseRect(const pugi::xml_node& node, sf::IntRect& value, const sf::IntRect& defaultValue)
{
    value.left = node.attribute("x").as_int(defaultValue.left);
    value.top = node.attribute("y").as_int(defaultValue.top);
    value.width = node.attribute("w").as_int(defaultValue.width);
    value.height = node.attribute("h").as_int(defaultValue.height);
}

bool TryParseRect(const pugi::xml_node& node, sf::IntRect& value)
{
    if (!node)
        return false;

    ParseRect(node, value, value);
    return true;
}

void ParseUDim2(const pugi::xml_node& node, UDim2& value, const UDim2& defaultValue)
{
    value.relative.x = node.attribute("xRel").as_float(defaultValue.relative.x);
    value.relative.y = node.attribute("yRel").as_float(defaultValue.relative.y);
    value.absolute.x = node.attribute("xAbs").as_float(defaultValue.absolute.x);
    value.absolute.y = node.attribute("yAbs").as_float(defaultValue.absolute.y);
}

bool TryParseUDim2(const pugi::xml_node& node, UDim2& value)
{
    if (!node)
        return false;

    ParseUDim2(node, value, value);
    return true;
}

void WriteVector2i(pugi::xml_node node, const Vector2i& value)
{
    node.append_attribute("x").set_value(value.x);
    node.append_attribute("y").set_value(value.y);
}

void WriteVector2f(pugi::xml_node node, const Vector2f& value)
{
    node.append_attribute("x").set_value(value.x);
    node.append_attribute("y").set_value(value.y);
}

void WriteRect(pugi::xml_node node, const sf::IntRect& value)
{
    node.append_attribute("x").set_value(value.left);
    node.append_attribute("y").set_value(value.top);
    node.append_attribute("w").set_value(value.width);
    node.append_attribute("h").set_value(value.height);
}

void WriteUDim2(pugi::xml_node node, const UDim2& value)
{
    node.append_attribute("xRel").set_value(value.relative.x);
    node.append_attribute("yRel").set_value(value.relative.y);
    node.append_attribute("xAbs").set_value(value.absolute.x);
    node.append_attribute("yAbs").set_value(value.absolute.y);
}

}   // namespace xml

}   // namespace gugu
