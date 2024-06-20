////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/NewDirectoryDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

NewDirectoryDialog::NewDirectoryDialog(const std::string& path, const std::string& name, const DelegateValidation& callback)
    : BaseModalDialog("New Folder")
    , m_path(path)
    , m_name(name)
    , m_callback(callback)
{
}

NewDirectoryDialog::~NewDirectoryDialog()
{
}

void NewDirectoryDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::LabelText("Path", m_path.c_str());

    ImGui::Spacing();
    if (ImGui::InputText("Name", &m_name, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //TODO: Check Availability.
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    ImGui::BeginDisabled(m_name.empty());
    if (ImGui::Button("Validate"))
    {
        m_callback(m_name);

        CloseModalImpl();
    }
    ImGui::EndDisabled();
}

}   //namespace gugu
