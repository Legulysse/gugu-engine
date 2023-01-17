#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DatasheetParserContext;
    struct DataSaveContext;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasaveObject
{
public:

    DatasaveObject();
    virtual ~DatasaveObject();

    bool LoadFromXml(const std::string& _strPathName);
    bool SaveToXml(const std::string& _strPathName);

protected:

    virtual void ParseMembers(DatasheetParserContext& _kContext) = 0;
    virtual void SerializeMembers(DataSaveContext& _kContext) = 0;
};

}   // namespace gugu
