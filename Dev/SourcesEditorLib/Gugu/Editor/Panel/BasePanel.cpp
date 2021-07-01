////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/BasePanel.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BasePanel::BasePanel()
    : m_focused(false)
{
}

BasePanel::~BasePanel()
{
}

bool BasePanel::IsFocused() const
{
    return m_focused;
}

}   //namespace gugu
