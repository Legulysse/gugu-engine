////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Resources/Texture.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu//Engine.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Misc/Logger.h"

#include <SFML/Graphics/Texture.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Texture::Texture()
: m_sfTexture(nullptr)
{
}

Texture::~Texture()
{
    SafeDelete(m_sfTexture);
}

void Texture::SetSFTexture(sf::Texture* _pSFTexture)
{
    m_sfTexture = _pSFTexture;
}

sf::Texture* Texture::GetSFTexture() const
{
    return m_sfTexture;
}

EResourceType::Type	Texture::GetResourceType() const
{
    return EResourceType::Texture;
}

bool Texture::LoadFromFile()
{
	m_sfTexture = new sf::Texture;
    if (!m_sfTexture->loadFromFile(GetFileInfoRef().GetPathName()))
	{
        //Use a purple square
        sf::Image oSFImage;
        oSFImage.create(200, 200, sf::Color(255, 0, 255));
		m_sfTexture->create(200, 200);
        m_sfTexture->update(oSFImage);

        m_sfTexture->setSmooth(false);
        m_sfTexture->setRepeated(false);

		//TODO: Return true ? Curently it will discard the Resource I think
        GetLogEngine()->Print(ELog::Warning, ELogEngine::Resources, StringFormat("Texture not found : {0}", GetFileInfoRef().GetPathName()));
        return false;
	}

    m_sfTexture->setSmooth(false);
    m_sfTexture->setRepeated(false);

    return true;
}

bool Texture::ReloadFromFile()
{
    if (!m_sfTexture)
        return false;

    if (m_sfTexture->loadFromFile(GetFileInfoRef().GetPathName()))
    {
        return true;
    }

    return false;
}

sf::Vector2f Texture::GetSize() const
{
    if (m_sfTexture)
        return sf::Vector2f(m_sfTexture->getSize());
    return sf::Vector2f(0.f, 0.f);
}

}	// namespace gugu
