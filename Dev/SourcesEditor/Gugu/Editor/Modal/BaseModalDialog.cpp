////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/BaseModalDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BaseModalDialog::BaseModalDialog(const std::string& title)
    : m_title(title)
    , m_opening(true)
    , m_closed(false)
{
}

BaseModalDialog::~BaseModalDialog()
{
}

void BaseModalDialog::UpdateModal(const DeltaTime& dt)
{
    if (m_opening)
    {
        m_opening = false;

        // Note: with the new ImGuiPopupFlags_NoReopen, maybe the popup handling could be simplified ?
        ImGuiPopupFlags testPopupFlags = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel;
        if (!ImGui::IsPopupOpen("", testPopupFlags))
        {
            ImGuiPopupFlags openPopupFlags = ImGuiPopupFlags_None;
            ImGui::OpenPopup(m_title.c_str(), openPopupFlags);
        }
        else
        {
            // Force close if we tried to stack modal popups.
            m_closed = true;
        }
    }

    if (!m_closed)
    {
        // TODO: do I need to push some id on the id stack, to avoid potential collisions ?
        if (ImGui::BeginPopupModal(m_title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
        {
            UpdateModalImpl(dt);

            ImGui::EndPopup();
        }
        else
        {
            // Force close if something forced the modal popup to be closed on imgui's side.
            m_closed = true;
        }
    }
}

void BaseModalDialog::CloseModalImpl()
{
    ImGui::CloseCurrentPopup();
    m_closed = true;
}

bool BaseModalDialog::IsClosed() const
{
    return m_closed;
}

}   //namespace gugu
