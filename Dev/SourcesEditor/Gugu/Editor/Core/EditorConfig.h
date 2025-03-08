#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct EditorConfig
{
    std::string defaultProjectFilePath;
    size_t maxUndoStateCount = 100;

    std::string imageMagickDirectoryPath;

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   // namespace gugu
