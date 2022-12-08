#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Rect.hpp>
#include <pugixml.hpp>

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

// Read a single attribute from a node.
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value);
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value);
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value);
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value);
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value);
bool XmlReadAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value);

// Read a data structure from a node's attributes.
bool XmlReadVector2(const pugi::xml_node& node, Vector2f& value);
bool XmlReadRect(const pugi::xml_node& node, sf::IntRect& value);
bool XmlReadUDim2(const pugi::xml_node& node, UDim2& value);

// Write a data structure in a node's attributes.
bool XmlWriteVector2(pugi::xml_node node, const Vector2f& value);
bool XmlWriteRect(pugi::xml_node node, const sf::IntRect& value);
bool XmlWriteUDim2(pugi::xml_node node, const UDim2& value);

// Read a value inside a node
bool XmlReadValue(const pugi::xml_node& node, std::string& value);

template<typename T>
bool XmlReadValue(const pugi::xml_node& node, T& value)
{
    if (!node)
        return false;
    
    return FromString(node.child_value(), value);
}

}   // namespace gugu
