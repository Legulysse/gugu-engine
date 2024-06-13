#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataObject.h"
#include "Gugu/System/UUID.h"

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DataParseContext;
    class Datasheet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasheetObject : public DataObject
{
public:

    DatasheetObject();
    virtual ~DatasheetObject();

    bool LoadFromFile(const std::string& path, Datasheet* ownerDatasheet, std::vector<class Datasheet*>& ancestors);

    virtual void ParseMembers(DataParseContext& _kContext) = 0;

    const UUID& GetUuid() const;

    // Return the owning datasheet.
    Datasheet* GetDatasheet() const;

private:

    UUID m_uuid;
    Datasheet* m_datasheet;
};

}   // namespace gugu
