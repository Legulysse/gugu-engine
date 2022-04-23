#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct ProjectSettings
{
public:

    std::string projectPathFile;
    std::string projectPath;
    std::string projectAssetsPath;
    std::string projectBindingPathFile;

    bool LoadFromFile(const std::string& pathFile);
};

}   //namespace gugu
