#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementUILayout : public Element
{
public:

    ElementUILayout();
    virtual ~ElementUILayout();

    bool LoadFromFile(const std::string& _strPath);
    virtual bool LoadFromXml(const pugi::xml_node& _oNodeElement) override;

private:

    void LoadElement(const pugi::xml_node& _oNodeElement, Element* _pParent);

protected:

    bool m_blockEvents;
};

}   // namespace gugu
