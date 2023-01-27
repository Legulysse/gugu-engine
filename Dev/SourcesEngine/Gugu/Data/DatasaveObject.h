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

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasaveObject : public DataObject
{
public:

    DatasaveObject();
    virtual ~DatasaveObject();

    bool LoadFromXml(const std::string& _strPathName);
    bool SaveToXml(const std::string& _strPathName);

    virtual void ParseMembers(DataParseContext& _kContext) = 0;
    virtual void SerializeMembers(DataSaveContext& _kContext) const = 0;

    virtual const std::string& GetDataInstanceType() const = 0;
};

}   // namespace gugu
