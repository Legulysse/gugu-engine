#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Rect.hpp>
#include <PugiXml/pugixml.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class UDim2;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Helper for string serialization.
struct xml_string_writer : pugi::xml_writer
{
public:

    xml_string_writer(std::string& target);
    virtual void write(const void* data, size_t size);

private:

    std::string* m_target = nullptr;
};

//Read a single attribute from a node.
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, bool& _bValue);
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, int& _iValue);
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, unsigned int& _uiValue);
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, float& _fValue);
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, double& _dValue);
bool XmlReadAttribute(const pugi::xml_node& _kNode, const std::string& _strName, std::string& _strValue);

//Read a data structure from a node's attributes.
bool XmlReadVector2(const pugi::xml_node& node, Vector2f& value);
bool XmlReadRect(const pugi::xml_node& node, sf::IntRect& value);
bool XmlReadUDim2(const pugi::xml_node& node, UDim2& value);

//Read a value inside a node
bool XmlReadValue(const pugi::xml_node& _kNode, std::string& _strValue);

template<typename T>
bool XmlReadValue(const pugi::xml_node& _kNode, T& _oValue)
{
    if (!_kNode)
        return false;
    
    return FromString(_kNode.child_value(), _oValue);
}

}   // namespace gugu
