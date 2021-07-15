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
    class VirtualDatasheet;
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

    struct DataMemberDefinition
    {
        std::string type;   // TODO: I may need a combo of flags for base types, is array, is instance, is reference, and custom types reference.
        std::string name;
        std::string defaultValue;   // TODO: union ?
    };

    struct ClassDefinition
    {
        std::string name;
        std::string baseName;
        ClassDefinition* baseDefinition;
        std::vector<DataMemberDefinition*> dataMembers;
    };

public:

    DatasheetParser();
    ~DatasheetParser();

    bool ParseBinding(const std::string& pathDatasheetBinding);
    void ClearBinding();

    bool IsDatasheet(const FileInfo& fileInfo) const;
    bool GetClassDefinition(const std::string& name, ClassDefinition*& classDefinition) const;

    VirtualDatasheet* InstanciateDatasheetResource(const std::string& resourceID);

private:

    std::vector<EnumDefinition*> m_enumDefinitions;
    std::vector<ClassDefinition*> m_classDefinitions;
};

}   //namespace gugu
