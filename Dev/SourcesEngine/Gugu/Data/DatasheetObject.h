#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataObject.h"

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

    bool LoadFromXml(const std::string& _strPathName, Datasheet* ownerDatasheet, std::vector<class Datasheet*>& ancestors);

    virtual void ParseMembers(DataParseContext& _kContext) = 0;

    // Return the owning datasheet (only if this object is a root object, return nullptr otherwise).
    Datasheet* GetDatasheet() const;

private:

    Datasheet* m_datasheet;
};

}   // namespace gugu
