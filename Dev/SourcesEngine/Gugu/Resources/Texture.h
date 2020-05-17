#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Texture : public Resource
{
public:

    Texture();
    virtual ~Texture();

    void SetSFTexture(sf::Texture* _pSFTexture);
    sf::Texture* GetSFTexture() const;
    
    void SetSmooth(bool smooth);
    bool IsSmooth() const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
    virtual bool ReloadFromFile() override;

    sf::Vector2f GetSize() const;

protected:

    sf::Texture* m_sfTexture;
};

}   // namespace gugu
