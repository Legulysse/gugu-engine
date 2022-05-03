////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Font.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
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
    SafeDelete(m_sfFont);
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

bool Font::LoadFromFile()
{
    m_sfFont = new sf::Font;
    if (!m_sfFont->loadFromFile(GetFileInfo().GetPathName()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Font not found : {0}", GetFileInfo().GetPathName()));
        return false;
    }
    return true;
}

}   // namespace gugu
