////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/AboutDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorVersion.h"

#include "Gugu/EngineVersion.h"

#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

AboutDialog::AboutDialog()
    : BaseModalDialog("About")
{
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::Text("gugu::Editor %s", GUGU_EDITOR_VERSION);
    ImGui::Spacing();
    ImGui::Text("author: Legulysse");
    ImGui::Spacing();
    ImGui::Text("using gugu::Engine %s", GUGU_ENGINE_VERSION);

    ImGui::Spacing();
    if (ImGui::Button("Close"))
    {
        CloseModalImpl();
    }
}

}   //namespace gugu
