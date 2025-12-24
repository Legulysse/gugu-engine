////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/LocalizationTable.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

LocalizationTable::LocalizationTable()
{
}

LocalizationTable::~LocalizationTable()
{
    Unload();
}

LocalizationRegisterResult LocalizationTable::TryRegisterEntry(const LocalizationLanguageCode& language, const LocalizationKey& key, const LocalizationTextEntry& entry)
{
    if (entry.text.empty())
    {
        return LocalizationRegisterResult::Refused_EmptyText;
    }

    auto itLanguage = m_languageTables.find(language);
    if (itLanguage == m_languageTables.end())
    {
        itLanguage = m_languageTables.insert(itLanguage, std::make_pair(language, LocalizationLanguageTable()));
    }

    auto itEntry = itLanguage->second.entries.find(key);
    if (itEntry == itLanguage->second.entries.end())
    {
        itEntry = itLanguage->second.entries.insert(itEntry, std::make_pair(key, entry));
        return LocalizationRegisterResult::Accepted;
    }
    else
    {
        if (itEntry->second.timestamp > entry.timestamp)
        {
            return LocalizationRegisterResult::Refused_Outdated;
        }
        else if (itEntry->second.timestamp == entry.timestamp)
        {
            if (itEntry->second.text == entry.text)
            {
                return LocalizationRegisterResult::Refused_Identical;
            }
            else    // if (itEntry->second.text != entry.text)
            {
                itEntry->second = entry;
                return LocalizationRegisterResult::Accepted_IdenticalTimestamp;
            }
        }
        else    // if (itEntry->second.timestamp < entry.timestamp)
        {
            if (itEntry->second.text == entry.text)
            {
                itEntry->second = entry;
                return LocalizationRegisterResult::Accepted_IdenticalText;
            }
            else    // if (itEntry->second.text != entry.text)
            {
                itEntry->second = entry;
                return LocalizationRegisterResult::Accepted;
            }
        }
    }
}

const std::string& LocalizationTable::GetText(const LocalizationLanguageCode& language, const LocalizationKey& key) const
{
    auto itLanguage = m_languageTables.find(language);
    if (itLanguage != m_languageTables.end())
    {
        auto itEntry = itLanguage->second.entries.find(key);
        if (itEntry != itLanguage->second.entries.end())
        {
            return itEntry->second.text;
        }
    }

    static const std::string defaultValue = "LOCA NOT FOUND";
    return defaultValue;
}

EResourceType::Type LocalizationTable::GetResourceType() const
{
    return EResourceType::LocalizationTable;
}

void LocalizationTable::Unload()
{
    m_languageTables.clear();
}

bool LocalizationTable::LoadFromXml(const pugi::xml_document& document)
{
    Unload();

    pugi::xml_node rootNode = document.child("LocalizationTable");
    if (!rootNode)
        return false;

    for (pugi::xml_node languageNode = rootNode.child("LanguageTable"); languageNode; languageNode = languageNode.next_sibling("LanguageTable"))
    {
        std::string languageCode = languageNode.attribute("code").as_string();
        if (languageCode.empty())
            continue;

        auto kvpLanguageInsert = m_languageTables.insert(std::make_pair(languageCode, LocalizationLanguageTable()));
        auto itLanguage = kvpLanguageInsert.first;

        for (pugi::xml_node entryNode = languageNode.child("Entry"); entryNode; entryNode = entryNode.next_sibling("Entry"))
        {
            std::string key = entryNode.attribute("key").as_string();
            if (key.empty())
                continue;

            LocalizationTextEntry entry;
            entry.timestamp = entryNode.attribute("timestamp").as_llong();
            entry.text = entryNode.attribute("text").as_string();

            itLanguage->second.entries.insert(std::make_pair(key, entry));
        }
    }

    return true;
}

bool LocalizationTable::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node rootNode = document.append_child("LocalizationTable");
    rootNode.append_attribute("serializationVersion") = 1;

    for (const auto& kvpLanguage : m_languageTables)
    {
        pugi::xml_node languageNode = rootNode.append_child("LanguageTable");
        languageNode.append_attribute("code").set_value(kvpLanguage.first.c_str());

        for (const auto& kvpEntry : kvpLanguage.second.entries)
        {
            pugi::xml_node entryNode = languageNode.append_child("Entry");
            entryNode.append_attribute("key").set_value(kvpEntry.first.c_str());
            entryNode.append_attribute("timestamp").set_value(kvpEntry.second.timestamp);
            entryNode.append_attribute("text").set_value(kvpEntry.second.text.c_str());
        }
    }

    return true;
}

}   // namespace gugu
