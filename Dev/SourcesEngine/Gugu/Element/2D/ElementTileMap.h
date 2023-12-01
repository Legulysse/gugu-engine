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
    class SquareGrid;
    class HexGrid;
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

    void BuildFromSquareGrid(SquareGrid* grid);
    void BuildFromHexGrid(HexGrid* grid);
    void BuildFromTileDimensions(size_t width, size_t height, const Vector2f& tileSize);
    void BuildFromTileCount(size_t count, const Vector2f& mapSize);

    void UpdateTilePositionAndSize(size_t index, const sf::FloatRect& rect);
    void UpdateTilePositionAndSize(size_t x, size_t y, size_t width, const sf::FloatRect& rect);
    void UpdateTileTextureCoords(size_t index, const sf::IntRect& rect);
    void UpdateTileTextureCoords(size_t x, size_t y, size_t width, const sf::IntRect& rect);
    void UpdateTileColor(size_t index, const sf::Color& color);
    void UpdateTileColor(size_t x, size_t y, size_t width, const sf::Color& color);

    size_t GetTileCount() const;

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

protected:

    Texture* m_texture;
    sf::VertexArray m_vertices;
};

}   // namespace gugu
