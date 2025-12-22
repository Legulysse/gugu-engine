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

    void OpenFile(const std::string& path)
    {
        m_filePath = FileInfo::FromString_utf8(path);
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

void GatherDatasheetTexts(const FileInfo& fileInfo, CsvWriter& writer)
{
    pugi::xml_document document;
    pugi::xml_parse_result loadResult = document.load_file(fileInfo.GetFileSystemPath().c_str());
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
                if (!memberDefinition->isArray)
                {
                    std::string value = dataNode.attribute("value").value();

                    std::string filename = ToString(fileInfo.GetFileName_utf8());

                    // Write localization text data.
                    writer.WriteField(filename);            // File.
                    writer.WriteField(uuid.ToString());     // Object.
                    writer.WriteField(name);                // Property.
                    writer.WriteField("0000");              // Timestamp.
                    writer.WriteField(value);               // Workstring.
                    writer.WriteField("");                  // TODO: Project-defined list of locales.
                    writer.WriteField("");                  // TODO: Project-defined list of locales.
                    writer.EndLine();
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

}

ExportLocalizationDialog::ExportLocalizationDialog()
    : BaseModalDialog("Export Localization")
{
    // Default settings.
    m_targetDirectory = "../LocalizationExport";

    // User settings.
    //if (!GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath.empty())
    //{
    //    m_targetDirectory = GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath;
    //}
}

ExportLocalizationDialog::~ExportLocalizationDialog()
{
}

void ExportLocalizationDialog::UpdateModalImpl(const DeltaTime& dt)
{
    ImGui::PushItemWidth(800);
    ImGui::InputText("Target Directory", &m_targetDirectory);

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
    // Save settings.
    //GetEditor()->GetUserSettings().importImageSetTargetDirectoryPath = m_targetDirectory;
    //GetEditor()->SaveUserSettings();

    // Setup.
    EnsureDirectoryExists(m_targetDirectory);

    // Prepare writer.
    impl::CsvWriter writer;
    writer.OpenFile(CombinePaths(m_targetDirectory, "TestLoca.csv"));

    // Write debug entry to help external text editor undestrand we want utf8 instead of ansi.
    writer.WriteField("(Data block with some utf8 chars : éàùç)");
    writer.EndLine();

    // Write headers.
    writer.WriteField("File");
    writer.WriteField("Object");
    writer.WriteField("Property");
    writer.WriteField("Timestamp");
    writer.WriteField("Workstring");
    writer.WriteField("en-US Text");    // TODO: Project-defined list of locales.
    writer.WriteField("fr-FR Text");    // TODO: Project-defined list of locales.
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
            impl::GatherDatasheetTexts(resourceInfo->fileInfo, writer);
        }
    }

    // Finalize.
    writer.SaveAndCloseFile();

    GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Export Localization finished."));
}

}   // namespace gugu
