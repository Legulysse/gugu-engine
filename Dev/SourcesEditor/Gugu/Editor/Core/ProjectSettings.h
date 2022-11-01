#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ProjectSettings
{
    std::string projectAssetsPath;
    std::string projectBindingFilePath;

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
