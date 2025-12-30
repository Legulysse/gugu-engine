#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct UserSettings
{
    std::string lastProjectFilePath;

    // Imageset generation.
    std::string importImageSetSourceDirectoryPath;
    std::string importImageSetTargetDirectoryPath;
    std::string importImageSetTargetTextureName;
    float importImageSetResizeScale = 0.f;
    std::string importImageSetResizeFilter;

    // Localization.
    std::string localizationExportDirectoryPath;
    std::string localizationExportFileName;
    std::string localizationTargetTable;

public:

    bool LoadFromFile(const std::string& pathFile);
    bool SaveToFile(const std::string& pathFile) const;
};

}   //namespace gugu
