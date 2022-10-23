#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct UserSettings
{
public:

    std::string lastProjectPathFile;

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
