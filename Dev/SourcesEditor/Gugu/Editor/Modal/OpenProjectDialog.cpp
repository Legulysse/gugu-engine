////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/OpenProjectDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include "Gugu/System/Path.h"
#include "Gugu/External/ImGuiUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

OpenProjectDialog::OpenProjectDialog(const std::string& projectPath)
    : BaseModalDialog("Open Project")
    , m_projectPath(projectPath)
{
}

OpenProjectDialog::~OpenProjectDialog()
{
}

void OpenProjectDialog::UpdateModalImpl(const DeltaTime& dt)
{
    if (ImGui::InputText("Project Path File", &m_projectPath, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        NormalizePathSelf(m_projectPath);
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Validate"))
    {
        GetEditor()->OpenProject(m_projectPath);

        CloseModalImpl();
    }
}

}   //namespace gugu
