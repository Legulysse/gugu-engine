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

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, bool& _bValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _bValue = kAttr.as_bool();
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, int& _iValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _iValue = kAttr.as_int();
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, unsigned int& _uiValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _uiValue = kAttr.as_uint();
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, float& _fValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _fValue = kAttr.as_float();
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, double& _dValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _dValue = kAttr.as_double();
    return true;
}

bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, std::string& _strValue)
{
    if (!_kNode)
        return false;

    pugi::xml_attribute kAttr = _kNode.attribute(_strName.c_str());
    if (!kAttr)
        return false;

    _strValue = kAttr.as_string();
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

bool XmlReadValue(const pugi::xml_node& node, std::string& value)
{
    if (!node)
        return false;

    value = node.child_value();
    return true;
}

}   // namespace gugu
