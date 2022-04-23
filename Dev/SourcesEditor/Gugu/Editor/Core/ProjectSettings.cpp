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

bool ProjectSettings::LoadFromFile(const std::string& pathFile)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(pathFile.c_str());
    if (!result)
        return false;

    pugi::xml_node nodeProject = document.child("ProjectSettings");
    if (!nodeProject)
        return false;

    projectPathFile = pathFile;

    projectPath = pathFile;
    PathFromPathFile(pathFile, false, projectPath);

    projectAssetsPath = CombinePaths(projectPath, nodeProject.child("AssetsPath").attribute("value").value(), false);
    projectBindingPathFile = CombinePathFile(projectPath, nodeProject.child("BindingPathFile").attribute("value").value());

    return true;
}

}   //namespace gugu
