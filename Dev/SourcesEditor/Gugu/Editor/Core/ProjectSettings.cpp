////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Core/ProjectSettings.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool ProjectSettings::LoadFromFile(const std::string& pathFile)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(pathFile.c_str());
    if (!result)
        return false;

    pugi::xml_node rootNode = document.child("ProjectSettings");
    if (!rootNode)
        return false;

    std::string projectDirectoryPath = DirectoryPartFromPath(pathFile);

    projectAssetsPath = CombinePaths(projectDirectoryPath, rootNode.child("AssetsPath").attribute("value").value());
    projectBindingFilePath = CombinePaths(projectDirectoryPath, rootNode.child("BindingFilePath").attribute("value").value());

    return true;
}

}   //namespace gugu
