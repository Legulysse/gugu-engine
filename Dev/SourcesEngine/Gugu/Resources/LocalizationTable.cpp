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

    return true;
}

bool LocalizationTable::SaveToXml(pugi::xml_document& document) const
{
    pugi::xml_node rootNode = document.append_child("LocalizationTable");
    rootNode.append_attribute("serializationVersion") = 1;

    return true;
}

}   // namespace gugu
