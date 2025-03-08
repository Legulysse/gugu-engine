#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct UserSettings
{
    std::string lastProjectFilePath;

    std::string importImageSetSourceDirectoryPath;
    std::string importImageSetTargetDirectoryPath;

public:

    bool LoadFromFile(const std::string& pathFile);
    bool SaveToFile(const std::string& pathFile) const;
};

}   //namespace gugu
