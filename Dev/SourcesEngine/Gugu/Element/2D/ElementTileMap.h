#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementTileMap : public Element
{
public:

    ElementTileMap();
    virtual ~ElementTileMap();

    void SetTexture(const std::string& _strTexturePath);    //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);

    Texture* GetTexture() const;

    void SetTileCount(int _iWidth, int _iHeight);
    void SetTileSize(const sf::Vector2f& _kTileSize);

    bool SetTile(int x, int y, const sf::IntRect& _kSubRect);

protected:

    virtual void DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void SetSizeImpl(sf::Vector2f _kOldSize) override;

protected:

    Texture* m_texture;

    int m_width;
    int m_height;
    sf::Vector2f m_tileSize;

    sf::VertexArray m_vertices;
    bool m_needRecompute;
};

}   // namespace gugu
