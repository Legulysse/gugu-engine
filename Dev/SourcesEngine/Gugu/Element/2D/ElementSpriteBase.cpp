////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSpriteBase.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/External/PugiXmlWrap.h"
#include "Gugu/Math/MathUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSpriteBase::ElementSpriteBase()
: m_color(sf::Color::White)
, m_dirtyVertices(true)
, m_repeatTexture(false)
, m_flipTextureX(false)
, m_flipTextureY(false)
{
}

ElementSpriteBase::~ElementSpriteBase()
{
}

void ElementSpriteBase::SetSubRect(const sf::IntRect& _oRect)
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

    RaiseDirtyVertices();
}

sf::IntRect ElementSpriteBase::GetSubRect() const
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

void ElementSpriteBase::SetRepeatTexture(bool repeatTexture)
{
    if (m_repeatTexture != repeatTexture)
    {
        m_repeatTexture = repeatTexture;
        RaiseDirtyVertices();
    }
}

void ElementSpriteBase::SetFlipTextureX(bool _bFlipTextureX)
{
    SetFlipTexture(_bFlipTextureX, m_flipTextureY);
}

void ElementSpriteBase::SetFlipTextureY(bool _bFlipTextureY)
{
    SetFlipTexture(m_flipTextureX, _bFlipTextureY);
}

void ElementSpriteBase::SetFlipTexture(bool _bFlipTextureX, bool _bFlipTextureY)
{
    if (_bFlipTextureX != m_flipTextureX || _bFlipTextureY != m_flipTextureY)
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

        RaiseDirtyVertices();
    }
}

void ElementSpriteBase::SetColor(const sf::Color& _oColor)
{
    m_color = _oColor;
    RaiseDirtyVertices();
}

sf::Color ElementSpriteBase::GetColor() const
{
    return m_color;
}

void ElementSpriteBase::OnSizeChanged(sf::Vector2f _kOldSize)
{
    RaiseDirtyVertices();
}

void ElementSpriteBase::RaiseDirtyVertices()
{
    m_dirtyVertices = true;
}

size_t ElementSpriteBase::GetRequiredVertexCount() const
{
    if (!m_repeatTexture)
    {
        return 6;
    }
    else
    {
        sf::Vector2f kAreaSize = GetSize();

        // Actual subrect size (= used texture size)
        int iTextureSizeX = Absolute(m_subRect.width);
        int iTextureSizeY = Absolute(m_subRect.height);

        // Nb tiles we can fully render
        int iNbFullTilesX = ((int)kAreaSize.x) / iTextureSizeX;
        int iNbFullTilesY = ((int)kAreaSize.y) / iTextureSizeY;

        // Total tiles
        int iNbTilesX = iNbFullTilesX + 1;
        int iNbTilesY = iNbFullTilesY + 1;

        return iNbTilesX * iNbTilesY * 6;
    }
}

void ElementSpriteBase::RecomputeVerticesPositionAndTextureCoords(sf::Vertex* vertices) const
{
    if (!m_repeatTexture)
    {
        sf::Vector2f kAreaSize = GetSize();

        // Recompute vertices position.
        vertices[0].position = sf::Vector2f(0.f, 0.f);
        vertices[1].position = sf::Vector2f(kAreaSize.x, 0.f);
        vertices[2].position = sf::Vector2f(0.f, kAreaSize.y);
        vertices[3].position = sf::Vector2f(kAreaSize.x, 0.f);
        vertices[4].position = sf::Vector2f(0.f, kAreaSize.y);
        vertices[5].position = sf::Vector2f(kAreaSize.x, kAreaSize.y);

        // Recompute texture coords.
        float fLeft = (float)m_subRect.left;
        float fTop = (float)m_subRect.top;
        float fRight = fLeft + m_subRect.width;
        float fBottom = fTop + m_subRect.height;

        vertices[0].texCoords = sf::Vector2f(fLeft, fTop);
        vertices[1].texCoords = sf::Vector2f(fRight, fTop);
        vertices[2].texCoords = sf::Vector2f(fLeft, fBottom);
        vertices[3].texCoords = sf::Vector2f(fRight, fTop);
        vertices[4].texCoords = sf::Vector2f(fLeft, fBottom);
        vertices[5].texCoords = sf::Vector2f(fRight, fBottom);
    }
    else
    {
        sf::Vector2f kAreaSize = GetSize();

        // Actual subrect size (= used texture size)
        int iTextureSizeX = Absolute(m_subRect.width);
        int iTextureSizeY = Absolute(m_subRect.height);

        // Nb tiles we can fully render
        int iNbFullTilesX = ((int)kAreaSize.x) / iTextureSizeX;
        int iNbFullTilesY = ((int)kAreaSize.y) / iTextureSizeY;

        // Total tiles
        int iNbTilesX = iNbFullTilesX + 1;
        int iNbTilesY = iNbFullTilesY + 1;

        // Targeted texture coordinates (can be flipped)
        float fLeft = (float)m_subRect.left;
        float fTop = (float)m_subRect.top;
        float fRight = fLeft + m_subRect.width;
        float fBottom = fTop + m_subRect.height;

        // Remaining margins for truncated tiles
        float fRemainingAreaX = kAreaSize.x - (iNbFullTilesX * iTextureSizeX);
        float fRemainingAreaY = kAreaSize.y - (iNbFullTilesY * iTextureSizeY);
        float fRemainingRight = fLeft + (float)m_subRect.width * (fRemainingAreaX / (float)iTextureSizeX);
        float fRemainingBottom = fTop + (float)m_subRect.height * (fRemainingAreaY / (float)iTextureSizeY);

        // Convenience cast
        float fTextureSizeX = (float)iTextureSizeX;
        float fTextureSizeY = (float)iTextureSizeY;

        // Full tiles
        {
            for (int y = 0; y < iNbFullTilesY; ++y)
            {
                for (int x = 0; x < iNbFullTilesX; ++x)
                {
                    sf::Vertex* quad = &vertices[(x + y * iNbTilesX) * 6];

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
                sf::Vertex* quad = &vertices[(x + y * iNbTilesX) * 6];

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
                sf::Vertex* quad = &vertices[(x + y * iNbTilesX) * 6];

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
            // TODO: Avoid generating vertices if the number of full tiles matches the total area surface.
            int x = iNbTilesX - 1;
            int y = iNbTilesY - 1;

            sf::Vertex* quad = &vertices[(x + y * iNbTilesX) * 6];

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
    }
}

void ElementSpriteBase::RecomputeVerticesColor(sf::Vertex* vertices, size_t count) const
{
    for (size_t i = 0; i < count; ++i)
    {
        vertices[i].color = m_color;
    }
}

}   // namespace gugu
