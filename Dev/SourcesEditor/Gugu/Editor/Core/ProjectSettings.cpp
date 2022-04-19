////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Core/ProjectSettings.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"

#include <PugiXml/pugixml.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ProjectSettings::ProjectSettings()
{
}

bool ProjectSettings::LoadFromXml(const std::string& pathFile)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(pathFile.c_str());
    if (!result)
        return false;

    pugi::xml_node nodeProject = document.child("ProjectSettings");
    if (!nodeProject)
        return false;

    std::string projectPath = pathFile;
    PathFromPathFileSelf(projectPath, false);

    std::string projectAssetsPath = CombinePaths(projectPath, nodeProject.child("AssetsPath").attribute("value").value(), false);
    std::string projectBindingPathFile = CombinePathFile(projectPath, nodeProject.child("BindingPathFile").attribute("value").value());

    m_projectPathFile = pathFile;
    m_projectPath = projectPath;
    m_projectAssetsPath = projectAssetsPath;
    m_projectBindingPathFile = projectBindingPathFile;

    return true;
}

}   //namespace gugu
