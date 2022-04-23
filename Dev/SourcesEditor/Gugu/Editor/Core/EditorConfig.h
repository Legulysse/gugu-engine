#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

struct EditorConfig
{
    std::string projectPathFile = "";
    size_t maxUndoStateCount = 100;
};

}   // namespace gugu
