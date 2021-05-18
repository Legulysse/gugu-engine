#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/Vector2.h"

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

//Open a XML File in the Assets folder
bool XmlLoadFile        (pugi::xml_document& _kDoc, const std::string& _strFile);

//Read a single attribute from a node
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, bool& _bValue);
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, int& _iValue);
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, unsigned int& _uiValue);
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, float& _fValue);
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, double& _dValue);
bool XmlReadAttribute   (const pugi::xml_node& _kNode, const std::string& _strName, std::string& _strValue);

//Read a data structure from a node
bool XmlReadUDimension  (const pugi::xml_node& _kNode, UDim2& _oDim);
bool XmlReadVector2     (const pugi::xml_node& _kNode, sf::Vector2f& _kVector);

//Read a value inside a node
bool XmlReadValue       (const pugi::xml_node& _kNode, std::string& _strValue);

template<typename T>
bool XmlReadValue       (const pugi::xml_node& _kNode, T& _oValue)
{
    if (!_kNode)
        return false;
    
    return FromString(_kNode.child_value(), _oValue);
}

}   // namespace gugu
