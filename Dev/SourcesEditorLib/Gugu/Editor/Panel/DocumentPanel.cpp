////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/DocumentPanel.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DocumentPanel::DocumentPanel()
    : m_dirty(false)
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
