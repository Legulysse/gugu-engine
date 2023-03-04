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

// Try to Parse a single value from a node's attribute (value will only be modified if the node and attribute exist).
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, size_t& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value);
bool XmlTryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value);

// Read a data structure from a node's attributes.
Vector2i XmlReadVector2i(const pugi::xml_node& node, const Vector2i& defaultValue = Vector2::Zero_i);
Vector2f XmlReadVector2f(const pugi::xml_node& node, const Vector2f& defaultValue = Vector2::Zero_f);
sf::IntRect XmlReadRect(const pugi::xml_node& node);

// Parse a data structure from a node's attributes (value will always be modified).
void XmlParseVector2i(const pugi::xml_node& node, Vector2i& value, const Vector2i& defaultValue = Vector2::Zero_i);
void XmlParseVector2f(const pugi::xml_node& node, Vector2f& value, const Vector2f& defaultValue = Vector2::Zero_f);
void XmlParseRect(const pugi::xml_node& node, sf::IntRect& value);
void XmlParseUDim2(const pugi::xml_node& node, UDim2& value);

// Try to Parse a data structure from a node's attributes (value will only be modified if the node exists).
bool XmlTryParseVector2i(const pugi::xml_node& node, Vector2i& value);
bool XmlTryParseVector2f(const pugi::xml_node& node, Vector2f& value);
bool XmlTryParseRect(const pugi::xml_node& node, sf::IntRect& value);
bool XmlTryParseUDim2(const pugi::xml_node& node, UDim2& value);

// Write a data structure in a node's attributes.
void XmlWriteVector2i(pugi::xml_node node, const Vector2i& value);
void XmlWriteVector2f(pugi::xml_node node, const Vector2f& value);
void XmlWriteRect(pugi::xml_node node, const sf::IntRect& value);
void XmlWriteUDim2(pugi::xml_node node, const UDim2& value);

}   // namespace gugu
