////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Modal/ExportLocalizationDialog.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"
#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/ResourceInfo.h"
#include "Gugu/Resources/LocalizationTable.h"
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

namespace impl {

class CsvWriter
{
public:

    void OpenFile(const FileInfo& path)
    {
        m_filePath = path;
        m_firstLineField = true;
    }

    void SaveAndCloseFile()
    {
        if (!m_filePath.IsValid())
            return;

        std::ofstream file;
        file.open(m_filePath.GetFileSystemPath().c_str(), std::ios::out | std::ios::trunc);

        if (file)
        {
            file << m_buffer.str();
            file.close();
        }

        m_buffer.str("");
        m_buffer.clear();
    }

    void WriteField(const std::string& value)
    {
        if (!m_firstLineField)
        {
            m_buffer << ",";
        }

        m_buffer << "\"" << value << "\"";
        m_firstLineField = false;
    }

    void EndLine()
    {
        m_buffer << std::endl;
        m_firstLineField = true;
    }

private:

    FileInfo m_filePath;
    std::ostringstream m_buffer;
    bool m_firstLineField;
};

void GatherDatasheetTexts(const LocalizationTable* localizationTable, const std::vector<std::string>& languageCodes, const FileInfo& datasheetFileInfo, CsvWriter& writer)
{
    pugi::xml_document document;
    pugi::xml_parse_result loadResult = document.load_file(datasheetFileInfo.GetFileSystemPath().c_str());
    if (!loadResult)
        return;

    pugi::xml_node datasheetNode = document.child("Datasheet");
    if (!datasheetNode)
        return;

    auto gatherDatasheetObjectTexts = [&](const pugi::xml_node& objectNode)
    {
        UUID uuid = UUID::FromString(objectNode.attribute("uuid").value());

        std::string className = objectNode.attribute("type").value();
        DatasheetParser::ClassDefinition* classDefinition;
        GetEditor()->GetDatasheetParser()->GetClassDefinition(className, classDefinition);

        if (uuid.IsZero() || !classDefinition)
            return;

        for (pugi::xml_node dataNode = objectNode.child("Data"); dataNode; dataNode = dataNode.next_sibling("Data"))
        {
            std::string name = dataNode.attribute("name").value();
            auto memberDefinition = classDefinition->GetDataMemberDefinition(name);

            if (memberDefinition
                && memberDefinition->isLocalized
                && memberDefinition->type == DatasheetParser::DataMemberDefinition::String)
            {
                pugi::xml_node localizationNode = dataNode.child("Localization");
                if (localizationNode)
                {
                    if (!memberDefinition->isArray)
                    {
                        std::string key = localizationNode.attribute("key").value();
                        std::string timestampStr = localizationNode.attribute("timestamp").value();
                        std::string value = dataNode.attribute("value").value();
                        int64 timestamp = FromString<int64>(timestampStr, 0);

                        // Write localization text data.
                        writer.WriteField(key);             // Key.
                        writer.WriteField(timestampStr);    // Timestamp.
                        writer.WriteField(value);           // Workstring.
                        for (const auto& languageCode : languageCodes)
                        {
                            if (auto entry = localizationTable->GetEntry(languageCode, key))
                            {
                                if (entry->timestamp < timestamp)
                                    writer.WriteField("UPDATE");    // Text Status.
                                else
                                    writer.WriteField("OK");        // Text Status.
                                writer.WriteField(entry->text);     // Text.
                            }
                            else
                            {
                                writer.WriteField("NEW");           // Text Status.
                                writer.WriteField("");              // Text.
                            }
                        }
                        writer.EndLine();
                    }
                    else
                    {
                        // TODO: Handle array of localized properties.
                        // - I will need to solve array items inheritance before handling this, to have a proper way to reference array items.
                    }
                }
            }
        }
    };

    // Root
    pugi::xml_node rootObjectNode = datasheetNode.child("RootObject");
    gatherDatasheetObjectTexts(rootObjectNode);

    // Instance objects
    for (pugi::xml_node instanceNode = datasheetNode.child("Object"); instanceNode; instanceNode = instanceNode.next_sibling("Object"))
    {
        gatherDatasheetObjectTexts(instanceNode);
    }

    // Object overrides
    for (pugi::xml_node overrideNode = datasheetNode.child("ObjectOverride"); overrideNode; overrideNode = overrideNode.next_sibling("ObjectOverride"))
    {
        gatherDatasheetObjectTexts(overrideNode);
    }
}

}   // namespace impl

ExportLocalizationDialog::ExportLocalizationDialog()
    : BaseModalDialog("Export Localization")
{
    // Default settings.
    m_exportDirectoryPath = "../../LocalizationExport";
    m_exportFileName = "Localization.csv";
    m_targetTable = "Default.localization.xml";

    // User settings.
    if (!GetEditor()->GetUserSettings().localizationExportDirectoryPath.empty())
    {
        m_exportDirectoryPath = GetEditor()->GetUserSettings().localizationExportDirectoryPath;
    }

    if (!GetEditor()->GetUserSettings().localizationExportFileName.empty())
    {
        m_exportFileName = GetEditor()->GetUserSettings().localizationExportFileName;
    }

    if (!GetEditor()->GetUserSettings().localizationTargetTable.empty())
    {
        m_targetTable = GetEditor()->GetUserSettings().localizationTargetTable;
    }
}

ExportLocalizationDialog::~ExportLocalizationDialog()
{
}

void ExportLocalizationDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::PushItemWidth(800);
    ImGui::InputText("Export Directory", &m_exportDirectoryPath);
    ImGui::InputText("Export File Name", &m_exportFileName);
    ImGui::InputText("Target Table", &m_targetTable);

    ImGui::Spacing();
    if (ImGui::Button("Cancel"))
    {
        CloseModalImpl();
    }

    ImGui::SameLine();
    if (ImGui::Button("Export"))
    {
        ExportLocalization();
        CloseModalImpl();
    }
}

void ExportLocalizationDialog::ExportLocalization()
{
    // Notes:
    // - When opening the csv file with LibreOffice, be careful to enforce "" fields as string, and avoid auto-detecting numbers (timestamps may be altered if treated as numbers).

    // Save settings.
    GetEditor()->GetUserSettings().localizationExportDirectoryPath = m_exportDirectoryPath;
    GetEditor()->GetUserSettings().localizationExportFileName = m_exportFileName;
    GetEditor()->GetUserSettings().localizationTargetTable = m_targetTable;
    GetEditor()->SaveUserSettings();

    // Setup.
    EnsureDirectoryExists(m_exportDirectoryPath);

    // Get target localization table.
    auto targetLocalizationTable = GetResources()->GetLocalizationTable(m_targetTable);
    assert(targetLocalizationTable != nullptr);

    // TODO: Project-defined list of locales.
    std::vector<std::string> languageCodes = { "en-US", "fr-FR" };

    // Prepare csv writer.
    FileInfo targetFile = FileInfo::FromString_utf8(CombinePaths(m_exportDirectoryPath, m_exportFileName));

    impl::CsvWriter writer;
    writer.OpenFile(targetFile);

    // Write debug entry to help external text editor undestrand we want utf8 instead of ansi.
    writer.WriteField("(Data block with some utf8 chars : éàùç)");
    writer.EndLine();

    // Write headers.
    writer.WriteField("Key");
    writer.WriteField("Timestamp");
    writer.WriteField("Workstring");
    for (const auto& languageCode : languageCodes)
    {
        writer.WriteField(StringFormat("{0} Status", languageCode));
        writer.WriteField(StringFormat("{0} Text", languageCode));
    }
    writer.EndLine();

    // Gather texts.
    DatasheetParser* datasheetParser = GetEditor()->GetDatasheetParser();

    std::vector<const ResourceInfo*> resourceInfos;
    GetResources()->GetAllResourceInfos(resourceInfos);
    for (const auto& resourceInfo : resourceInfos)
    {
        EResourceType::Type resourceType = GetResources()->GetResourceType(resourceInfo->fileInfo);

        // Check datasheets.
        if (resourceType == EResourceType::Unknown
            && datasheetParser && datasheetParser->IsDatasheet(resourceInfo->fileInfo))
        {
            impl::GatherDatasheetTexts(targetLocalizationTable, languageCodes, resourceInfo->fileInfo, writer);
        }
    }

    // Finalize.
    writer.SaveAndCloseFile();

    GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Export Localization finished."));
}

}   // namespace gugu
