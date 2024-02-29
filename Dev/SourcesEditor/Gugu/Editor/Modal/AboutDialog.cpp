////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/AboutDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/EditorVersion.h"

#include "Gugu/EngineVersion.h"

#include <imgui.h>
#include <pugixml.hpp>

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
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Based on :");

    ImGui::Text("Dear ImGui %s", IMGUI_VERSION);
    ImGui::Text("SFML %i.%i.%i", SFML_VERSION_MAJOR, SFML_VERSION_MINOR, SFML_VERSION_PATCH);
    ImGui::Text("ImGui-SFML");  // No version macro available here.

    int pugixmlMajor = PUGIXML_VERSION / 1000;
    int pugixmlMinor = (PUGIXML_VERSION - (pugixmlMajor * 1000)) / 10;
    int pugixmlPatch = PUGIXML_VERSION - (pugixmlMajor * 1000) - (pugixmlMinor * 10);
    ImGui::Text("PugiXml %i.%i.%i", pugixmlMajor, pugixmlMinor, pugixmlPatch);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Close"))
    {
        CloseModalImpl();
    }
}

}   //namespace gugu
