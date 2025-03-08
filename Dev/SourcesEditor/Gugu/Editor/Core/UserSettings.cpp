////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Core/UserSettings.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/FileInfo.h"
#include "Gugu/External/PugiXmlUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool UserSettings::LoadFromFile(const std::string& pathFile)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(FileInfo::FromString_utf8(pathFile).GetFileSystemPath().c_str());
    if (!result)
        return false;

    pugi::xml_node rootNode = document.child("UserSettings");
    if (!rootNode)
        return false;

    lastProjectFilePath = rootNode.child("LastProjectFilePath").attribute("value").value();
    importImageSetSourceDirectoryPath = rootNode.child("ImportImageSetSourceDirectoryPath").attribute("value").value();
    importImageSetTargetDirectoryPath = rootNode.child("ImportImageSetTargetDirectoryPath").attribute("value").value();

    return true;
}

bool UserSettings::SaveToFile(const std::string& pathFile) const
{
    pugi::xml_document document;
    pugi::xml_node rootNode = document.append_child("UserSettings");

    rootNode.append_child("LastProjectFilePath").append_attribute("value").set_value(lastProjectFilePath.c_str());
    rootNode.append_child("ImportImageSetSourceDirectoryPath").append_attribute("value").set_value(importImageSetSourceDirectoryPath.c_str());
    rootNode.append_child("ImportImageSetTargetDirectoryPath").append_attribute("value").set_value(importImageSetTargetDirectoryPath.c_str());

    return document.save_file(FileInfo::FromString_utf8(pathFile).GetFileSystemPath().c_str(), PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_utf8);
}

}   //namespace gugu
