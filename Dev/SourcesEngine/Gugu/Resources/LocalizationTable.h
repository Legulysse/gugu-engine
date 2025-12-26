#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/System/UUID.h"

#include <map>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

using LocalizationLanguageCode = std::string;
using LocalizationKey = std::string;

struct LocalizationTextEntry
{
    int64 timestamp = 0;
    std::string text;
};

struct LocalizationLanguageTable
{
    std::map<LocalizationKey, LocalizationTextEntry> entries;
};

enum class LocalizationRegisterResult : uint8
{
    Accepted,                       // The new entry has a more recent timestamp than the current entry, or has a new key (new or updated).

    Accepted_IdenticalTimestamp,    // The new entry has the same timestamp as the current entry, but a different text (a review could be necessary).
    Accepted_IdenticalText,         // The new entry has the same text as the current entry, but with a more recent timestamp (a review could be necessary).

    Refused_Identical,              // The new entry has the same text and timestamp as the new entry (loca is up to date).
    Refused_EmptyText,              // The new entry has an empty text (invalid).
    Refused_Outdated,               // The new entry is older than the current entry (outdated).
};

LocalizationKey GenerateLocalizationKeyForDatasheetMember(const std::string& datasheetId, const std::string& objectUuid, const std::string& memberName);

class LocalizationTable : public Resource
{
public:

    LocalizationTable();
    virtual ~LocalizationTable();

    LocalizationRegisterResult TryRegisterEntry(const LocalizationLanguageCode& language, const LocalizationKey& key, const LocalizationTextEntry& entry);
    const std::string& GetText(const LocalizationLanguageCode& language, const LocalizationKey& key) const;

    virtual EResourceType::Type GetResourceType() const override;

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

protected:

    std::map<LocalizationLanguageCode, LocalizationLanguageTable> m_languageTables;
};

}   // namespace gugu
