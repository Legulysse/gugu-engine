#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class FileInfo;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DatasheetParser
{
public:

    struct EnumDefinition
    {
        std::string name;
        std::vector<std::string> values;
    };

    struct ClassDataDefinition
    {
        std::string type;   // TODO: I may need a combo of flags for base types, is array, is instance, is reference, and custom types reference.
        std::string name;
        std::string defaultValue;   // TODO: union ?
    };

    struct ClassDefinition
    {
        std::string name;
        std::string base;
        std::vector<ClassDataDefinition> dataMembers;
    };

public:

    DatasheetParser();
    ~DatasheetParser();

    bool ParseBinding(const std::string& pathDatasheetBinding);
    void ClearBinding();

    bool IsDatasheet(const FileInfo& fileInfo) const;

    bool GetClassDefinition(const std::string& name, ClassDefinition& classDefinition) const;

private:

    std::vector<EnumDefinition> m_enumDefinitions;
    std::vector<ClassDefinition> m_classDefinitions;
};

}   //namespace gugu
