#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class EditorClipboard
{
public:

    std::string contentType;
    std::string stringContent;

    void Reset();
    void SetStringContent(const std::string& type, const std::string& content);
};

EditorClipboard* GetEditorClipboard();

}   // namespace gugu
