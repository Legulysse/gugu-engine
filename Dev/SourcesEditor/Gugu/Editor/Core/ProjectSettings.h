#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ProjectSettings
{
    std::string projectPathFile;
    std::string projectPath;
    std::string projectAssetsPath;
    std::string projectBindingPathFile;

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
