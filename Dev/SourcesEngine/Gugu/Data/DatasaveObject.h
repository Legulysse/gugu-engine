#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataObject.h"

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DataParseContext;
    struct DataSaveContext;
}

namespace pugi
{
    class xml_document;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasaveObject : public DataObject
{
public:

    DatasaveObject();
    virtual ~DatasaveObject();

    bool LoadFromFile(const std::string& path);

    bool SaveToFile(const std::string& path) const;
    bool SaveToString(std::string& result) const;

    virtual void ParseMembers(DataParseContext& _kContext) = 0;
    virtual void SerializeMembers(DataSaveContext& _kContext) const = 0;

    virtual const std::string& GetDataInstanceType() const = 0;

private:

    bool SaveToXml(pugi::xml_document& document) const;
};

}   // namespace gugu
