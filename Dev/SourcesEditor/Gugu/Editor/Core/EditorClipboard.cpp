////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Editor/Core/EditorClipboard.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Editor/Editor.h"

#include <SFML/Window/Clipboard.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void EditorClipboard::Reset()
{
    contentType.clear();
    stringContent.clear();
}

void EditorClipboard::SetStringContent(const std::string& type, const std::string& content)
{
    Reset();

    contentType = type;
    stringContent = content;

    // Also copy to OS clipboard for convenience
    sf::Clipboard::setString(sf::String::fromUtf8(content.begin(), content.end()));
}

EditorClipboard* GetEditorClipboard()
{
    return GetEditor()->GetEditorClipboard();
}

}   //namespace gugu
