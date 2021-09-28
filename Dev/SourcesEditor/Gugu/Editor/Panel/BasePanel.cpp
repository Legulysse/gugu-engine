////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Panel/BasePanel.h"

////////////////////////////////////////////////////////////////
// Includes

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BasePanel::BasePanel()
    : m_title("Panel Title")
    , m_focused(false)
{
}

BasePanel::~BasePanel()
{
}

bool BasePanel::IsFocused() const
{
    return m_focused;
}

void BasePanel::ForceFocus()
{
    ImGui::SetWindowFocus(m_title.c_str());
}

}   //namespace gugu
