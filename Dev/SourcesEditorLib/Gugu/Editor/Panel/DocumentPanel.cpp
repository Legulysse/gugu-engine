////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DocumentPanel::DocumentPanel()
{
}

DocumentPanel::~DocumentPanel()
{
}

bool DocumentPanel::IsResource(const std::string& resourceID) const
{
    return m_resourceID == resourceID;
}

}   //namespace gugu
