////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementUILayout.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Element/2D/ElementSprite.h"

#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlWrap.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementUILayout::ElementUILayout()
{
    m_blockEvents = false;
}

ElementUILayout::~ElementUILayout()
{
}

bool ElementUILayout::LoadFromFile(const std::string& _strPath)
{
    std::string strResourcePath = GetResources()->GetResourcePathName(_strPath);

    pugi::xml_document oDoc;
    pugi::xml_parse_result result = oDoc.load_file(strResourcePath.c_str());
    if (!result)
        return false;

    pugi::xml_node oNodeLayout = oDoc.child("Layout");
    if (oNodeLayout.empty())
        return false;
    
    LoadFromXml(oNodeLayout);

    for (pugi::xml_node oNodeElement = oNodeLayout.child("Element"); oNodeElement; oNodeElement = oNodeElement.next_sibling("Element"))
    {
        LoadElement(oNodeElement, this);
    }

    return true;
}

void ElementUILayout::LoadElement(const pugi::xml_node& _oNodeElement, Element* _pParent)
{
    std::string strType = _oNodeElement.attribute("Type").value();
    std::string strName = _oNodeElement.attribute("Name").value();

    Element* pNewChild = nullptr;
    if (strType == "Sprite")
        pNewChild = _pParent->AddChild<ElementSprite>();

    if (pNewChild)
    {
        pNewChild->LoadFromXml(_oNodeElement);
        
        pugi::xml_node oNodeChildren = _oNodeElement.child("Children");
        if (oNodeChildren)
        {
            for (pugi::xml_node oNodeElement = oNodeChildren.child("Element"); oNodeElement; oNodeElement = oNodeElement.next_sibling("Element"))
            {
                LoadElement(oNodeElement, pNewChild);
            }
        }
    }
}

bool ElementUILayout::LoadFromXml(const pugi::xml_node& _oNodeElement)
{
    if (!Element::LoadFromXml(_oNodeElement))
        return false;

    return true;
}

}   // namespace gugu
