////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementTile.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Manager/ManagerResources.h"
#include "Gugu/Render/Renderer.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Utility/Math.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementTile::ElementTile()
: m_texture(nullptr)
, m_color(sf::Color::White)
, m_needRecompute(true)
, m_flipTextureX(false)
, m_flipTextureY(false)
{
}

ElementTile::~ElementTile()
{
}

void ElementTile::SetTexture(const std::string& _strTexturePath)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath));
}

void ElementTile::SetTexture(Texture* _pTexture)
{
    if (_pTexture)
    {
        m_texture = _pTexture;
        SetSubRect(sf::IntRect(sf::Vector2i(), sf::Vector2i(m_texture->GetSize())));
    }
}

void ElementTile::SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName)
{
    ImageSet* pImageSet = GetResources()->GetImageSet(_strImageSetName);
    if (pImageSet)
        SetSubImage(pImageSet->GetSubImage(_strSubImageName));
}

void ElementTile::SetSubImage(SubImage* _pSubImage)
{
    if (_pSubImage && _pSubImage->GetImageSet() && _pSubImage->GetImageSet()->GetTexture())
    {
        m_texture = _pSubImage->GetImageSet()->GetTexture();
        SetSubRect(_pSubImage->GetRect());
    }
}

Texture* ElementTile::GetTexture() const
{
    return m_texture;
}

void ElementTile::SetSubRect(const sf::IntRect& _oRect)
{
    sf::IntRect kOldRect = m_subRect;
    m_subRect = _oRect;
    if (m_flipTextureX)
    {
        m_subRect.top += m_subRect.height;
        m_subRect.height = -m_subRect.height;
    }
    if (m_flipTextureY)
    {
        m_subRect.left += m_subRect.width;
        m_subRect.width = -m_subRect.width;
    }

    if (m_subRect.width != kOldRect.width || m_subRect.height != kOldRect.height)
    {
        SetSize(sf::Vector2f((float)m_subRect.width, (float)m_subRect.height));
    }

    m_needRecompute = true;
}

sf::IntRect ElementTile::GetSubRect() const
{
    sf::IntRect kRect = m_subRect;
    if (m_flipTextureX)
    {
        kRect.top += kRect.height;
        kRect.height = -kRect.height;
    }
    if (m_flipTextureY)
    {
        kRect.left += kRect.width;
        kRect.width = -kRect.width;
    }

    return kRect;
}

void ElementTile::SetFlipTextureX(bool _bFlipTextureX)
{
    SetFlipTexture(_bFlipTextureX, m_flipTextureY);
}

void ElementTile::SetFlipTextureY(bool _bFlipTextureY)
{
    SetFlipTexture(m_flipTextureX, _bFlipTextureY);
}

void ElementTile::SetFlipTexture(bool _bFlipTextureX, bool _bFlipTextureY)
{
    if (_bFlipTextureX != m_flipTextureX)
    {
        m_subRect.top += m_subRect.height;
        m_subRect.height = -m_subRect.height;
    }
    if (_bFlipTextureY != m_flipTextureY)
    {
        m_subRect.left += m_subRect.width;
        m_subRect.width = -m_subRect.width;
    }

    m_flipTextureX = _bFlipTextureX;
    m_flipTextureY = _bFlipTextureY;

    m_needRecompute = true;
}

void ElementTile::SetColor(const sf::Color& _oColor)
{
    m_color = _oColor;

    m_needRecompute = true;
}

sf::Color ElementTile::GetColor() const
{
    return m_color;
}

void ElementTile::SetSizeImpl(sf::Vector2f _kOldSize)
{
    m_needRecompute = true;
}

void ElementTile::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        if (m_needRecompute)
        {
            m_needRecompute = false;

            sf::Vector2f kAreaSize = GetSize();

            // Targeted texture coordinates (can be flipped)
            float fLeft = (float)m_subRect.left;
            float fTop = (float)m_subRect.top;
            float fRight = fLeft + m_subRect.width;
            float fBottom = fTop + m_subRect.height;

            // Actual subrect size (= used texture size)
            int iTextureSizeX = Absolute(m_subRect.width);
            int iTextureSizeY = Absolute(m_subRect.height);

            // Nb tiles we can fully render
            int	iNbFullTilesX = ((int)kAreaSize.x) / iTextureSizeX;
            int	iNbFullTilesY = ((int)kAreaSize.y) / iTextureSizeY;

            // Remaining margins for truncated tiles
            float fRemainingAreaX = kAreaSize.x - (iNbFullTilesX * iTextureSizeX);
            float fRemainingAreaY = kAreaSize.y - (iNbFullTilesY * iTextureSizeY);
            float fRemainingRight = fLeft + (float)m_subRect.width * (fRemainingAreaX / (float)iTextureSizeX);
            float fRemainingBottom = fTop + (float)m_subRect.height * (fRemainingAreaY / (float)iTextureSizeY);

            // Total tiles
            int iNbTilesX = iNbFullTilesX + 1;
            int iNbTilesY = iNbFullTilesY + 1;

            // Convenience cast
            float fTextureSizeX = (float)iTextureSizeX;
            float fTextureSizeY = (float)iTextureSizeY;

            // Reset vertices
            m_vertices.setPrimitiveType(sf::Triangles);
            m_vertices.resize(iNbTilesX * iNbTilesY * 6);

            // Full tiles
            {
                for (int y = 0; y < iNbFullTilesY; ++y)
                {
                    for (int x = 0; x < iNbFullTilesX; ++x)
                    {
                        sf::Vertex* quad = &m_vertices[(x + y * iNbTilesX) * 6];

                        quad[0].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY);
                        quad[1].position = sf::Vector2f((x + 1) * fTextureSizeX, y * fTextureSizeY);
                        quad[2].position = sf::Vector2f(x * fTextureSizeX, (y + 1) * fTextureSizeY);

                        quad[3].position = sf::Vector2f((x + 1) * fTextureSizeX, y * fTextureSizeY);
                        quad[4].position = sf::Vector2f(x * fTextureSizeX, (y + 1) * fTextureSizeY);
                        quad[5].position = sf::Vector2f((x + 1) * fTextureSizeX, (y + 1) * fTextureSizeY);

                        quad[0].texCoords = sf::Vector2f(fLeft, fTop);
                        quad[1].texCoords = sf::Vector2f(fRight, fTop);
                        quad[2].texCoords = sf::Vector2f(fLeft, fBottom);

                        quad[3].texCoords = sf::Vector2f(fRight, fTop);
                        quad[4].texCoords = sf::Vector2f(fLeft, fBottom);
                        quad[5].texCoords = sf::Vector2f(fRight, fBottom);
                    }
                }
            }

            // Right tiles
            {
                int x = iNbTilesX - 1;
                for (int y = 0; y < iNbFullTilesY; ++y)
                {
                    sf::Vertex* quad = &m_vertices[(x + y * iNbTilesX) * 6];

                    quad[0].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY);
                    quad[1].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, y * fTextureSizeY);
                    quad[2].position = sf::Vector2f(x * fTextureSizeX, (y + 1) * fTextureSizeY);

                    quad[3].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, y * fTextureSizeY);
                    quad[4].position = sf::Vector2f(x * fTextureSizeX, (y + 1) * fTextureSizeY);
                    quad[5].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, (y + 1) * fTextureSizeY);

                    quad[0].texCoords = sf::Vector2f(fLeft, fTop);
                    quad[1].texCoords = sf::Vector2f(fRemainingRight, fTop);
                    quad[2].texCoords = sf::Vector2f(fLeft, fBottom);

                    quad[3].texCoords = sf::Vector2f(fRemainingRight, fTop);
                    quad[4].texCoords = sf::Vector2f(fLeft, fBottom);
                    quad[5].texCoords = sf::Vector2f(fRemainingRight, fBottom);
                }
            }

            // Bottom tiles
            {
                int y = iNbTilesY - 1;
                for (int x = 0; x < iNbFullTilesX; ++x)
                {
                    sf::Vertex* quad = &m_vertices[(x + y * iNbTilesX) * 6];

                    quad[0].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY);
                    quad[1].position = sf::Vector2f((x + 1) * fTextureSizeX, y * fTextureSizeY);
                    quad[2].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY + fRemainingAreaY);

                    quad[3].position = sf::Vector2f((x + 1) * fTextureSizeX, y * fTextureSizeY);
                    quad[4].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY + fRemainingAreaY);
                    quad[5].position = sf::Vector2f((x + 1) * fTextureSizeX, y * fTextureSizeY + fRemainingAreaY);

                    quad[0].texCoords = sf::Vector2f(fLeft, fTop);
                    quad[1].texCoords = sf::Vector2f(fRight, fTop);
                    quad[2].texCoords = sf::Vector2f(fLeft, fRemainingBottom);

                    quad[3].texCoords = sf::Vector2f(fRight, fTop);
                    quad[4].texCoords = sf::Vector2f(fLeft, fRemainingBottom);
                    quad[5].texCoords = sf::Vector2f(fRight, fRemainingBottom);
                }
            }

            // Last tile
            {
                int x = iNbTilesX - 1;
                int y = iNbTilesY - 1;

                sf::Vertex* quad = &m_vertices[(x + y * iNbTilesX) * 6];

                quad[0].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY);
                quad[1].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, y * fTextureSizeY);
                quad[2].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY + fRemainingAreaY);

                quad[3].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, y * fTextureSizeY);
                quad[4].position = sf::Vector2f(x * fTextureSizeX, y * fTextureSizeY + fRemainingAreaY);
                quad[5].position = sf::Vector2f(x * fTextureSizeX + fRemainingAreaX, y * fTextureSizeY + fRemainingAreaY);

                quad[0].texCoords = sf::Vector2f(fLeft, fTop);
                quad[1].texCoords = sf::Vector2f(fRemainingRight, fTop);
                quad[2].texCoords = sf::Vector2f(fLeft, fRemainingBottom);

                quad[3].texCoords = sf::Vector2f(fRemainingRight, fTop);
                quad[4].texCoords = sf::Vector2f(fLeft, fRemainingBottom);
                quad[5].texCoords = sf::Vector2f(fRemainingRight, fRemainingBottom);
            }

            // Apply color on all tiles
            {
                for (int y = 0; y < iNbTilesY; ++y)
                {
                    for (int x = 0; x < iNbTilesX; ++x)
                    {
                        sf::Vertex* quad = &m_vertices[(x + y * iNbTilesX) * 6];

                        quad[0].color = m_color;
                        quad[1].color = m_color;
                        quad[2].color = m_color;

                        quad[3].color = m_color;
                        quad[4].color = m_color;
                        quad[5].color = m_color;
                    }
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

            _kRenderPass.statRenderedSprites += 1;
        }
    }
}

}	// namespace gugu
