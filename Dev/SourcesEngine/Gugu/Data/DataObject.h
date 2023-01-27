#pragma once

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DataParseContext;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DataObject
{
public:

    DataObject();
    virtual ~DataObject();

    virtual void ParseMembers(DataParseContext& _kContext) = 0;
};

}   // namespace gugu
