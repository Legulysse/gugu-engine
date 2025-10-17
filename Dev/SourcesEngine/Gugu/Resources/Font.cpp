////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Font.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Font::Font()
: m_sfFont(nullptr)
{
}

Font::~Font()
{
    Unload();
}

void Font::SetSFFont(sf::Font* _pSFFont)
{
    m_sfFont = _pSFFont;
}

sf::Font* Font::GetSFFont() const
{
    return m_sfFont;
}

EResourceType::Type Font::GetResourceType() const
{
    return EResourceType::Font;
}

void Font::Unload()
{
    SafeDelete(m_sfFont);
}

bool Font::LoadFromFile()
{
    Unload();

    m_sfFont = new sf::Font;
    if (!m_sfFont->openFromFile(GetFileInfo().GetFileSystemPath()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Font not found : {0}", GetFileInfo().GetFilePath_utf8()));
        return false;
    }

    return true;
}

}   // namespace gugu
