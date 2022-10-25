#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct UserSettings
{
    std::string lastProjectPathFile;

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
