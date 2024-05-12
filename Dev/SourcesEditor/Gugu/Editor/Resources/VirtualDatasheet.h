#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Parser/DatasheetParser.h"

#include "Gugu/Resources/Resource.h"
#include "Gugu/System/UUID.h"

#include <string>
#include <map>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class VirtualDatasheetObject;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class VirtualDatasheet : public Resource
{
public:

    VirtualDatasheet();
    virtual ~VirtualDatasheet();

    virtual EResourceType::Type GetResourceType() const override;

    virtual void GetDependencies(std::set<Resource*>& dependencies) const override;
    virtual void OnDependencyUpdated(const Resource* dependency) override;
    virtual void OnDependencyRemoved(const Resource* dependency) override;

    bool InstanciateNewRootObject(DatasheetParser::ClassDefinition* classDefinition);
    VirtualDatasheetObject* InstanciateNewObject(DatasheetParser::ClassDefinition* classDefinition);
    VirtualDatasheetObject* InstanciateNewObjectOverride(DatasheetParser::ClassDefinition* classDefinition, const UUID& uuid);
    bool DeleteOrphanedInstanceObjects();
    bool DeleteInstanceObject(VirtualDatasheetObject* instanceObject);

    bool IsAncestor(const VirtualDatasheet* ancestorDatasheet) const;
    bool IsValidAsParent(const VirtualDatasheet* parentDatasheet, bool* invalidRecursiveParent) const;    // TODO: I could use an enum for error returns, and reuse them in other cases of references error feedbacks.
    void SetParentDatasheet(const std::string& parentDatasheetID, VirtualDatasheet* parentDatasheet);

    VirtualDatasheetObject* GetInstanceObjectFromUuid(const UUID& uuid) const;
    VirtualDatasheetObject* GetObjectOverrideFromUuid(const UUID& uuid) const;

    DatasheetParser::ClassDefinition* GetClassDefinition() const;

    void SortDataValues();

    static bool HandleMigration(const FileInfo& fileInfo);
    static bool Migrate_v1_to_v2(const FileInfo& fileInfo, pugi::xml_document& document);

protected:

    virtual void Unload() override;
    virtual bool LoadFromXml(const pugi::xml_document& document) override;
    virtual bool SaveToXml(pugi::xml_document& document) const override;

public:

    VirtualDatasheetObject* m_rootObject;
    std::map<UUID, VirtualDatasheetObject*> m_instanceObjects;
    std::map<UUID, VirtualDatasheetObject*> m_objectOverrides;

    std::string m_parentDatasheetID;
    VirtualDatasheet* m_parentDatasheet;
};

}   // namespace gugu
