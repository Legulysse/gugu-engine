#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Resource.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Rect.hpp>

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

    void SetRepeated(bool repeated);
    bool IsRepeated() const;

    Vector2i GetSize() const;
    sf::IntRect GetRect() const;

    virtual EResourceType::Type GetResourceType() const override;

    virtual bool LoadFromFile() override;
    virtual bool ReloadFromFile() override;

protected:

    sf::Texture* m_sfTexture;
};

}   // namespace gugu
