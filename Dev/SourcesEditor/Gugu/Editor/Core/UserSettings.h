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

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
