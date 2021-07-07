#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Panel/BasePanel.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DocumentPanel : public BasePanel
{
public:

    DocumentPanel();
    virtual ~DocumentPanel();

    virtual void UpdateProperties(const gugu::DeltaTime& dt) = 0;

    bool IsResource(const std::string& resourceID) const;

protected:

    std::string m_resourceID;
    bool m_dirty;
};

}   //namespace gugu
