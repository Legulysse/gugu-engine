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

private:

    void LoadElement(const pugi::xml_node& _oNodeElement, Element* _pParent);
    virtual bool LoadFromXmlImpl(const pugi::xml_node& _oNodeElement) override;

protected:

    bool m_blockEvents;
};

}   // namespace gugu
