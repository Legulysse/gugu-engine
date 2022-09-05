////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Resources/ManagerResources.h"

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

bool XmlLoadFile(pugi::xml_document& _kDoc, const std::string& _strFile)
{
    pugi::xml_parse_result result = _kDoc.load_file(GetResources()->GetResourcePathName(_strFile).c_str());
    return result;
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

bool XmlReadUDimension(const pugi::xml_node& _kNode, UDim2& _oDim)
{
    return (    XmlReadAttribute(_kNode, "xRel", _oDim.x.relative)
            &&  XmlReadAttribute(_kNode, "xAbs", _oDim.x.absolute)
            &&  XmlReadAttribute(_kNode, "yRel", _oDim.y.relative)
            &&  XmlReadAttribute(_kNode, "yAbs", _oDim.y.absolute)
            );
}

bool XmlReadVector2(const pugi::xml_node& _kNode, Vector2f& _kVector)
{
    return (    XmlReadAttribute(_kNode, "x", _kVector.x)
            &&  XmlReadAttribute(_kNode, "y", _kVector.y)
            );
}

bool XmlReadValue(const pugi::xml_node& _kNode, std::string& _strValue)
{
    if (!_kNode)
        return false;

    _strValue = _kNode.child_value();

    return true;
}

}   // namespace gugu
