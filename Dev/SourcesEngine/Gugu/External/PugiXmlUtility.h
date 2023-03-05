#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/Math/Vector2.h"
#include "Gugu/Math/UDim.h"

#include <SFML/Graphics/Rect.hpp>

#include <pugixml.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

namespace xml {

// Helper for string serialization.
struct StringWriter : pugi::xml_writer
{
public:

    StringWriter(std::string* target);
    virtual void write(const void* data, size_t size);

private:

    std::string* m_target = nullptr;
};

// Try to Parse a single value from a node's attribute (value will only be modified if the node and attribute exist).
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, bool& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, int& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, unsigned int& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, size_t& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, float& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, double& value);
bool TryParseAttribute(const pugi::xml_node& node, const std::string& attributeName, std::string& value);

// Read a data structure from a node's attributes.
Vector2i ReadVector2i(const pugi::xml_node& node, const Vector2i& defaultValue = Vector2::Zero_i);
Vector2f ReadVector2f(const pugi::xml_node& node, const Vector2f& defaultValue = Vector2::Zero_f);
sf::IntRect ReadRect(const pugi::xml_node& node);

// Parse a data structure from a node's attributes (value will always be modified).
void ParseVector2i(const pugi::xml_node& node, Vector2i& value, const Vector2i& defaultValue = Vector2::Zero_i);
void ParseVector2f(const pugi::xml_node& node, Vector2f& value, const Vector2f& defaultValue = Vector2::Zero_f);
void ParseRect(const pugi::xml_node& node, sf::IntRect& value);
void ParseUDim2(const pugi::xml_node& node, UDim2& value);

// Try to Parse a data structure from a node's attributes (value will only be modified if the node exists).
bool TryParseVector2i(const pugi::xml_node& node, Vector2i& value);
bool TryParseVector2f(const pugi::xml_node& node, Vector2f& value);
bool TryParseRect(const pugi::xml_node& node, sf::IntRect& value);
bool TryParseUDim2(const pugi::xml_node& node, UDim2& value);

// Write a data structure in a node's attributes.
void WriteVector2i(pugi::xml_node node, const Vector2i& value);
void WriteVector2f(pugi::xml_node node, const Vector2f& value);
void WriteRect(pugi::xml_node node, const sf::IntRect& value);
void WriteUDim2(pugi::xml_node node, const UDim2& value);

}   // namespace xml

}   // namespace gugu
