#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct EditorConfig
{
    std::string defaultProjectFilePath = "";
    size_t maxUndoStateCount = 100;

public:

    bool LoadFromFile(const std::string& pathFile);
};

}   // namespace gugu
