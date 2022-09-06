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

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_bool(value);
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_int(value);
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_uint(value);
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_float(value);
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_double(value);
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value)
{
    if (!node)
        return false;

    pugi::xml_attribute attribute = node.attribute(attributeName.c_str());
    if (!attribute)
        return false;

    value = attribute.as_string(value.c_str());
    return true;
}

bool XmlReadVector2(const pugi::xml_node& node, Vector2f& value)
{
    if (!node)
        return false;

    value.x = node.attribute("x").as_float(value.x);
    value.y = node.attribute("y").as_float(value.y);
    return true;
}

bool XmlReadRect(const pugi::xml_node& node, sf::IntRect& value)
{
    if (!node)
        return false;

    value.left = node.attribute("x").as_int(value.left);
    value.top = node.attribute("y").as_int(value.top);
    value.width = node.attribute("w").as_int(value.width);
    value.height = node.attribute("h").as_int(value.height);
    return true;
}

bool XmlReadUDim2(const pugi::xml_node& node, UDim2& value)
{
    if (!node)
        return false;

    value.x.relative = node.attribute("xRel").as_float(value.x.relative);
    value.y.relative = node.attribute("yRel").as_float(value.y.relative);
    value.x.absolute = node.attribute("xAbs").as_float(value.x.absolute);
    value.y.absolute = node.attribute("yAbs").as_float(value.y.absolute);
    return true;
}

bool XmlWriteVector2(pugi::xml_node node, const Vector2f& value)
{
    if (!node)
        return false;

    node.append_attribute("x").set_value(value.x);
    node.append_attribute("y").set_value(value.y);
    return true;
}

bool XmlWriteRect(pugi::xml_node node, const sf::IntRect& value)
{
    if (!node)
        return false;

    node.append_attribute("x").set_value(value.left);
    node.append_attribute("y").set_value(value.top);
    node.append_attribute("w").set_value(value.width);
    node.append_attribute("h").set_value(value.height);
    return true;
}

bool XmlWriteUDim2(pugi::xml_node node, const UDim2& value)
{
    if (!node)
        return false;

    node.append_attribute("xRel").set_value(value.x.relative);
    node.append_attribute("yRel").set_value(value.y.relative);
    node.append_attribute("xAbs").set_value(value.x.absolute);
    node.append_attribute("yAbs").set_value(value.y.absolute);
    return true;
}

bool XmlReadValue(const pugi::xml_node& node, std::string& value)
{
    if (!node)
        return false;

    value = node.child_value();
    return true;
}

}   // namespace gugu
