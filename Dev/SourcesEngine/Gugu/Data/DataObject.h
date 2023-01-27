#pragma once

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DatasheetParserContext;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DataObject
{
public:

    DataObject();
    virtual ~DataObject();

    virtual void ParseMembers(DatasheetParserContext& _kContext) = 0;
};

}   // namespace gugu
