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

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementTileMap::ElementTileMap()
: m_texture(nullptr)
, m_width(0)
, m_height(0)
, m_needRecompute(true)
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
        m_needRecompute = true;
    }
}

Texture* ElementTileMap::GetTexture() const
{
    return m_texture;
}

void ElementTileMap::SetTileCount(int _iWidth, int _iHeight)
{
    m_width = _iWidth;
    m_height = _iHeight;

    // Reset vertices
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(m_width * m_height * 6);

    //TODO: Handle some kind of quadtree to auto-generate clusters of tiles, each one only drawn if needed ? (I believe it doesn't belong here, clusters themselves should be tilemaps)

    SetSize(m_tileSize.x * m_width, m_tileSize.y * m_height);

    m_needRecompute = true;
}

void ElementTileMap::SetTileSize(const sf::Vector2f& _kTileSize)
{
    m_tileSize = _kTileSize;

    SetSize(m_tileSize.x * m_width, m_tileSize.y * m_height);

    m_needRecompute = true;
}

bool ElementTileMap::SetTile(int x, int y, const sf::IntRect& _kSubRect)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;

    float fLeft = (float)_kSubRect.left;
    float fTop = (float)_kSubRect.top;
    float fRight = fLeft + _kSubRect.width;
    float fBottom = fTop + _kSubRect.height;

    sf::Vertex* quad = &m_vertices[(x + y * m_width) * 6];

    quad[0].texCoords = sf::Vector2f(fLeft, fTop);
    quad[1].texCoords = sf::Vector2f(fRight, fTop);
    quad[2].texCoords = sf::Vector2f(fLeft, fBottom);

    quad[3].texCoords = sf::Vector2f(fRight, fTop);
    quad[4].texCoords = sf::Vector2f(fLeft, fBottom);
    quad[5].texCoords = sf::Vector2f(fRight, fBottom);

    return true;
}

void ElementTileMap::SetSizeImpl(sf::Vector2f _kOldSize)
{
    m_needRecompute = true;
}

void ElementTileMap::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    //TODO: maybe need a parameter to bypass this check ?
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_needRecompute)
        {
            m_needRecompute = false;

            float fTileWidth = m_tileSize.x;
            float fTileHeight = m_tileSize.y;

            for (int y = 0; y < m_height; ++y)
            {
                for (int x = 0; x < m_width; ++x)
                {
                    sf::Vertex* quad = &m_vertices[(x + y * m_width) * 6];

                    quad[0].position = sf::Vector2f(x * fTileWidth, y * fTileHeight);
                    quad[1].position = sf::Vector2f((x + 1) * fTileWidth, y * fTileHeight);
                    quad[2].position = sf::Vector2f(x * fTileWidth, (y + 1) * fTileHeight);

                    quad[3].position = sf::Vector2f((x + 1) * fTileWidth, y * fTileHeight);
                    quad[4].position = sf::Vector2f(x * fTileWidth, (y + 1) * fTileHeight);
                    quad[5].position = sf::Vector2f((x + 1) * fTileWidth, (y + 1) * fTileHeight);
                }
            }
        }

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
