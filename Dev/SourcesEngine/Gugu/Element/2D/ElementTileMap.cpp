////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementTileMap.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Misc/Grid/HexGrid.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementTileMap::ElementTileMap()
: m_texture(nullptr)
{
}

ElementTileMap::~ElementTileMap()
{
}

void ElementTileMap::SetTexture(const std::string& _strTexturePath)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath));
}

void ElementTileMap::SetTexture(Texture* _pTexture)
{
    if (_pTexture)
    {
        m_texture = _pTexture;
    }
}

Texture* ElementTileMap::GetTexture() const
{
    return m_texture;
}

int ElementTileMap::GetTileCount() const
{
    return m_vertices.getVertexCount() / 6;
}

void ElementTileMap::BuildFromSquareGrid(SquareGrid* grid)
{
    int width = grid->GetWidth();
    int height = grid->GetHeight();

    // Reset vertices.
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    sf::Vector2f tileSize = grid->GetCellSize();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(grid->GetCellPosition(sf::Vector2i(x, y)), tileSize));
        }
    }

    SetSize(grid->GetGridSize());
}

void ElementTileMap::BuildFromHexGrid(HexGrid* grid)
{
    int width = grid->GetWidth();
    int height = grid->GetHeight();

    // Reset vertices.
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    sf::Vector2f tileSize = grid->GetCellSize();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(grid->GetCellPosition(sf::Vector2i(x, y)), tileSize));
        }
    }

    SetSize(grid->GetGridSize());
}

void ElementTileMap::BuildFromTileDimensions(int width, int height, const sf::Vector2f& tileSize)
{
    // Reset vertices.
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(sf::Vector2f(x * tileSize.x, y * tileSize.y), tileSize));
        }
    }

    SetSize(tileSize.x * width, tileSize.y * height);
}

void ElementTileMap::BuildFromTileCount(int count, const sf::Vector2f& mapSize)
{
    // Reset vertices.
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(count * 6);

    SetSize(mapSize);
}

void ElementTileMap::UpdateTilePositionAndSize(int x, int y, int width, const sf::FloatRect& rect)
{
    UpdateTilePositionAndSize(x + y * width, rect);
}

void ElementTileMap::UpdateTilePositionAndSize(int index, const sf::FloatRect& rect)
{
    float left = rect.left;
    float top = rect.top;
    float right = left + rect.width;
    float bottom = top + rect.height;

    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].position = sf::Vector2f(left, top);
    quad[1].position = sf::Vector2f(right, top);
    quad[2].position = sf::Vector2f(left, bottom);

    quad[3].position = sf::Vector2f(right, top);
    quad[4].position = sf::Vector2f(left, bottom);
    quad[5].position = sf::Vector2f(right, bottom);
}

void ElementTileMap::UpdateTileTextureCoords(int x, int y, int width, const sf::IntRect& rect)
{
    UpdateTileTextureCoords(x + y * width, rect);
}

void ElementTileMap::UpdateTileTextureCoords(int index, const sf::IntRect& rect)
{
    float left = (float)rect.left;
    float top = (float)rect.top;
    float right = left + rect.width;
    float bottom = top + rect.height;

    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].texCoords = sf::Vector2f(left, top);
    quad[1].texCoords = sf::Vector2f(right, top);
    quad[2].texCoords = sf::Vector2f(left, bottom);

    quad[3].texCoords = sf::Vector2f(right, top);
    quad[4].texCoords = sf::Vector2f(left, bottom);
    quad[5].texCoords = sf::Vector2f(right, bottom);
}

void ElementTileMap::UpdateTileColor(int x, int y, int width, const sf::Color& color)
{
    UpdateTileColor(x + y * width, color);
}

void ElementTileMap::UpdateTileColor(int index, const sf::Color& color)
{
    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].color = color;
    quad[1].color = color;
    quad[2].color = color;

    quad[3].color = color;
    quad[4].color = color;
    quad[5].color = color;
}

void ElementTileMap::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    //TODO: maybe need a parameter to bypass this check ?
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_vertices.getVertexCount() > 0)
        {
            sf::RenderStates states;
            states.transform = _kTransformSelf;
            states.texture = m_texture->GetSFTexture();
            _kRenderPass.target->draw(m_vertices, states);

            //Stats
            if (_kRenderPass.frameInfos)
                _kRenderPass.frameInfos->statDrawCalls += 1;

            //TODO: special stat category for ElementTileMap
        }
    }
}

}   // namespace gugu
