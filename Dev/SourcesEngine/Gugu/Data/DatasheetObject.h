#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct DatasheetParserContext;
    class Datasheet;
}

namespace pugi
{
    class xml_node;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class DatasheetObject
{
public:

    DatasheetObject();
    virtual ~DatasheetObject();

    bool LoadFromXml(const std::string& _strPathName, Datasheet* ownerDatasheet, std::vector<class Datasheet*>& ancestors);

    virtual void ParseMembers(DatasheetParserContext& _kContext) = 0;

    // Return the owning datasheet (only if this object is a root object, return nullptr otherwise).
    Datasheet* GetDatasheet() const;

private:

    Datasheet* m_datasheet;
};

}   // namespace gugu
