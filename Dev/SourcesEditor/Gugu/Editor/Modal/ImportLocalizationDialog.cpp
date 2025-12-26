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

class CsvReader
{
public:

    bool OpenFile(const FileInfo& path)
    {
        m_filePath = path;
        m_file.open(m_filePath.GetFileSystemPath().c_str());
        return m_file.is_open();
    }

    void CloseFile()
    {
        m_file.close();
    }

    bool ParseHeaders(size_t headerLineIndex)
    {
        // Skip lines before expected header.
        for (size_t i = 0; i < headerLineIndex; ++i)
        {
            ParseNextLine();
        }

        // Parse expected headers.
        if (ParseNextLine())
        {
            for (size_t i = 0; i < m_currentLineValues.size(); ++i)
            {
                m_headerIndex.insert(std::make_pair(m_currentLineValues[i], i));
            }

            return true;
        }

        return false;
    }

    bool ParseNextLine()
    {
        m_currentLineValues.clear();

        // TODO: I may need to read multiple lines depending on newline chars inside text blocs.
        std::string line;
        if (std::getline(m_file, line))
        {
            size_t cursor = 0;
            while (cursor < line.size())
            {
                // Start parsing new field.
                bool quotedField = (line[cursor] == '\"');
                if (quotedField)
                {
                    ++cursor;
                }

                bool hasData = false;
                size_t fieldBegin = cursor;
                size_t fieldEnd = cursor;

                while (cursor < line.size())
                {
                    if (quotedField && line[cursor] == '\"')
                    {
                        // End quote found.
                        if (cursor == fieldBegin)
                        {
                            //No data between delimiters.
                            hasData = false;
                        }
                        else
                        {
                            hasData = true;
                            fieldEnd = cursor - 1;
                        }

                        ++cursor;   // We want to target the field end-delimiter (or the position where it should be).
                        break;
                    }
                    else if (!quotedField && line[cursor] == ',')
                    {
                        // Field delimiter found.
                        if (cursor == fieldBegin)
                        {
                            //No data between delimiters.
                            hasData = false;
                        }
                        else
                        {
                            hasData = true;
                            fieldEnd = cursor - 1;
                        }

                        //++cursor; // We are already targeting the field end-delimiter.
                        break;
                    }
                    else
                    {
                        ++cursor;
                    }
                }

                // Store field value.
                m_currentLineValues.push_back(!hasData ? "" : line.substr(fieldBegin, fieldEnd - fieldBegin + 1));

                // Try to find next field (just in case there is spacing between quoted field end and the next delimiter).
                while (cursor < line.size())
                {
                    if (line[cursor] == ',')
                    {
                        // Field delimiter found.
                        ++cursor;   // We want to target the next field first character.
                        break;
                    }
                    else
                    {
                        ++cursor;
                    }
                }
            }

            return true;
        }

        return false;
    }

    const std::string& GetField(const std::string& header) const
    {
        size_t index = 0;
        if (StdMapTryGetValue(m_headerIndex, header, index))
        {
            if (index < m_currentLineValues.size())
            {
                return m_currentLineValues[index];
            }
        }
        
        static const std::string defaultValue = "";
        return defaultValue;
    }

private:

    FileInfo m_filePath;
    std::ifstream m_file;
    std::map<std::string, size_t> m_headerIndex;
    std::vector<std::string> m_currentLineValues;
};

}   // namespace impl

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
    // - [done] Parse csv and transfer data into the table.
    // - [done] Store data in table.
    // - [done] Save/Load table data.
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

    // Parse csv file.
    FileInfo sourceFile = FileInfo::FromString_utf8(CombinePaths(m_targetDirectory, "TestLoca.csv"));

    impl::CsvReader reader;
    if (reader.OpenFile(sourceFile))
    {
        // TODO: Retrieve headers line index.
        if (reader.ParseHeaders(1))
        {
            while (reader.ParseNextLine())
            {
                // TODO: Retrieve and parse intended languages.
                LocalizationLanguageCode language = "en-US";

                LocalizationKey key = StringFormat("{0}/{1}/{2}", reader.GetField("File"), reader.GetField("Object"), reader.GetField("Property"));
                LocalizationTextEntry entry;

                entry.timestamp = FromString<int64>(reader.GetField("Timestamp"), 0);
                entry.text = reader.GetField("Workstring");

                targetLocalizationTable->TryRegisterEntry(language, key, entry);
            }
        }

        reader.CloseFile();
    }

    targetLocalizationTable->SaveToFile();

    // Finalize.
    GetLogEngine()->Print(ELog::Info, ELogEngine::Editor, StringFormat("Import Localization finished."));
}

}   // namespace gugu
