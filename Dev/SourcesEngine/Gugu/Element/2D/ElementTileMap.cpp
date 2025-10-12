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
    m_texture = _pTexture;
}

Texture* ElementTileMap::GetTexture() const
{
    return m_texture;
}

size_t ElementTileMap::GetTileCount() const
{
    return m_vertices.getVertexCount() / 6;
}

void ElementTileMap::BuildFromSquareGrid(SquareGrid* grid)
{
    size_t width = (size_t)grid->GetWidth();
    size_t height = (size_t)grid->GetHeight();

    // Reset vertices.
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    Vector2f tileSize = grid->GetCellSize();

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(grid->GetCellPosition(Vector2i((int)x, (int)y)), tileSize));
        }
    }

    SetSize(grid->GetGridSize());
}

void ElementTileMap::BuildFromHexGrid(HexGrid* grid)
{
    size_t width = (size_t)grid->GetWidth();
    size_t height = (size_t)grid->GetHeight();

    // Reset vertices.
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    Vector2f tileSize = grid->GetCellSize();

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(grid->GetCellPosition(Vector2i((int)x, (int)y)), tileSize));
        }
    }

    SetSize(grid->GetGridSize());
}

void ElementTileMap::BuildFromTileDimensions(size_t width, size_t height, const Vector2f& tileSize)
{
    // Reset vertices.
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    // Compute all tiles position and size.
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            UpdateTilePositionAndSize(x + y * width, sf::FloatRect(Vector2f((float)x * tileSize.x, (float)y * tileSize.y), tileSize));
        }
    }

    SetSize(tileSize.x * (float)width, tileSize.y * (float)height);
}

void ElementTileMap::BuildFromTileCount(size_t count, const Vector2f& mapSize)
{
    // Reset vertices.
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(count * 6);

    SetSize(mapSize);
}

void ElementTileMap::UpdateTilePositionAndSize(size_t x, size_t y, size_t width, const sf::FloatRect& rect)
{
    UpdateTilePositionAndSize(x + y * width, rect);
}

void ElementTileMap::UpdateTilePositionAndSize(size_t index, const sf::FloatRect& rect)
{
    float left = rect.left;
    float top = rect.top;
    float right = left + rect.width;
    float bottom = top + rect.height;

    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].position = Vector2f(left, top);
    quad[1].position = Vector2f(right, top);
    quad[2].position = Vector2f(left, bottom);

    quad[3].position = Vector2f(right, top);
    quad[4].position = Vector2f(left, bottom);
    quad[5].position = Vector2f(right, bottom);
}

void ElementTileMap::UpdateTileTextureCoords(size_t x, size_t y, size_t width, const sf::IntRect& rect)
{
    UpdateTileTextureCoords(x + y * width, rect);
}

void ElementTileMap::UpdateTileTextureCoords(size_t index, const sf::IntRect& rect)
{
    float left = (float)rect.left;
    float top = (float)rect.top;
    float right = left + (float)rect.width;
    float bottom = top + (float)rect.height;

    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].texCoords = Vector2f(left, top);
    quad[1].texCoords = Vector2f(right, top);
    quad[2].texCoords = Vector2f(left, bottom);

    quad[3].texCoords = Vector2f(right, top);
    quad[4].texCoords = Vector2f(left, bottom);
    quad[5].texCoords = Vector2f(right, bottom);
}

void ElementTileMap::UpdateTileColor(size_t x, size_t y, size_t width, const sf::Color& color)
{
    UpdateTileColor(x + y * width, color);
}

void ElementTileMap::UpdateTileColor(size_t index, const sf::Color& color)
{
    sf::Vertex* quad = &m_vertices[index * 6];

    quad[0].color = color;
    quad[1].color = color;
    quad[2].color = color;

    quad[3].color = color;
    quad[4].color = color;
    quad[5].color = color;
}

void ElementTileMap::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    //TODO: maybe need a parameter to bypass this check ?
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
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
            {
                _kRenderPass.frameInfos->statDrawCalls += 1;
                _kRenderPass.frameInfos->statTriangles += (int)m_vertices.getVertexCount() / 3;
            }

            //TODO: special stat category for ElementTileMap
        }
    }
}

}   // namespace gugu
