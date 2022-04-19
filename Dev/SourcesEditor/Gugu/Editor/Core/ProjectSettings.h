#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ProjectSettings
{
public:

    ProjectSettings();

    bool LoadFromXml(const std::string& pathFile);

public:

    std::string m_projectPathFile;
    std::string m_projectPath;
    std::string m_projectAssetsPath;
    std::string m_projectBindingPathFile;
};

}   //namespace gugu
