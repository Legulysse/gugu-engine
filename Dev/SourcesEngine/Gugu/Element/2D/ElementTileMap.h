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
    void BuildFromTileDimensions(int width, int height, const Vector2f& tileSize);
    void BuildFromTileCount(int count, const Vector2f& mapSize);

    void UpdateTilePositionAndSize(int index, const sf::FloatRect& rect);
    void UpdateTilePositionAndSize(int x, int y, int width, const sf::FloatRect& rect);
    void UpdateTileTextureCoords(int index, const sf::IntRect& rect);
    void UpdateTileTextureCoords(int x, int y, int width, const sf::IntRect& rect);
    void UpdateTileColor(int index, const sf::Color& color);
    void UpdateTileColor(int x, int y, int width, const sf::Color& color);

    size_t GetTileCount() const;

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

protected:

    Texture* m_texture;
    sf::VertexArray m_vertices;
};

}   // namespace gugu
