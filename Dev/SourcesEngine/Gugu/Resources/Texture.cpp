////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Texture.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Texture::Texture()
: m_sfTexture(nullptr)
{
}

Texture::~Texture()
{
    Unload();
}

void Texture::SetSFTexture(sf::Texture* _pSFTexture)
{
    m_sfTexture = _pSFTexture;
}

sf::Texture* Texture::GetSFTexture() const
{
    return m_sfTexture;
}

void Texture::SetSmooth(bool smooth)
{
    if (m_sfTexture)
    {
        m_sfTexture->setSmooth(smooth);
    }
}

bool Texture::IsSmooth() const
{
    if (m_sfTexture)
    {
        return m_sfTexture->isSmooth();
    }

    return false;
}

void Texture::SetRepeated(bool repeated)
{
    if (m_sfTexture)
    {
        m_sfTexture->setRepeated(repeated);
    }
}

bool Texture::IsRepeated() const
{
    if (m_sfTexture)
    {
        return m_sfTexture->isRepeated();
    }

    return false;
}

Vector2u Texture::GetSize() const
{
    return m_sfTexture ? m_sfTexture->getSize() : Vector2u();
}

sf::IntRect Texture::GetRect() const
{
    return m_sfTexture ? sf::IntRect(Vector2i(), Vector2i(m_sfTexture->getSize())) : sf::IntRect();
}

EResourceType::Type Texture::GetResourceType() const
{
    return EResourceType::Texture;
}

void Texture::Unload()
{
    SafeDelete(m_sfTexture);
}

bool Texture::LoadFromFile()
{
    Unload();

    m_sfTexture = new sf::Texture;
    if (!m_sfTexture->loadFromFile(GetFileInfo().GetFileSystemPath()))
    {
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Texture file could not be loaded : {0}", GetFileInfo().GetFilePath_utf8()));
        assert(false);
        return false;
    }

    m_sfTexture->setSmooth(GetResources()->IsDefaultTextureSmooth());
    m_sfTexture->setRepeated(false);

    return true;
}

}   // namespace gugu
