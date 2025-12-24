////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/ImportLocalizationDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/String.h"
#include "Gugu/System/UUID.h"
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/External/ImGuiUtility.h"
#include "Gugu/Debug/Logger.h"

#include <fstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ImportLocalizationDialog::ImportLocalizationDialog()
    : BaseModalDialog("Import Localization")
{
    // Default settings.
    m_targetDirectory = "../LocalizationExport";

    // User settings.
    //if (!GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath.empty())
    //{
    //    m_targetDirectory = GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath;
    //}
}

ImportLocalizationDialog::~ImportLocalizationDialog()
{
}

void ImportLocalizationDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::PushItemWidth(800);
    ImGui::InputText("Target Directory", &m_targetDirectory);

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Import"))
    {
        ImportLocalization();
        CloseModalImpl();
    }
}

void ImportLocalizationDialog::ImportLocalization()
{
    // TODO:
    // [Importer]
    // - Select target table.
    // - Select source csv.
    // - Parse csv and transfer data into the table.
    // - Store data in table.
    // - Save/Load table data.
    // [Runtime]
    // - Fill datasheet strings with their translation.
    // - Do I need a special LocalizedString type to store an ID and be able to query localization ?
    //   - This could allow for texts reloads without reparsing the datasheets.
    // [Game]
    // - Handle remaining localization sources.
    //   - UI widgets.
    //   - Hard coded texts.

    // Save settings.
    //GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath = m_targetDirectory;
    //GetEditor()->SaveUserSettings();

    // Get target localization table.
    auto targetLocalizationTable = GetResources()->GetLocalizationTable("Default.localization.xml");

    assert(targetLocalizationTable != nullptr);

    // Finalize.
    GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Import Localization finished."));
}

}   // namespace gugu
