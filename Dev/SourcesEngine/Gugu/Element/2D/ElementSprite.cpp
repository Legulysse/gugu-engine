////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSprite.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/External/PugiXmlWrap.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSprite::ElementSprite()
: m_texture(nullptr)
, m_color(sf::Color::White)
, m_flipTextureX(false)
, m_flipTextureY(false)
{
    m_vertices.setPrimitiveType(sf::TriangleStrip);
    m_vertices.resize(4);
}

ElementSprite::~ElementSprite()
{
}

void ElementSprite::SetTexture(const std::string& _strTexturePath)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath));
}

void ElementSprite::SetTexture(Texture* _pTexture)
{
    if (_pTexture)
    {
        m_texture = _pTexture;
        SetSubRect(sf::IntRect(sf::Vector2i(), sf::Vector2i(m_texture->GetSize())));
    }
}

void ElementSprite::SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName)
{
    ImageSet* pImageSet = GetResources()->GetImageSet(_strImageSetName);
    if (pImageSet)
        SetSubImage(pImageSet->GetSubImage(_strSubImageName));
}

void ElementSprite::SetSubImage(SubImage* _pSubImage)
{
    if (_pSubImage && _pSubImage->GetImageSet() && _pSubImage->GetImageSet()->GetTexture())
    {
        m_texture = _pSubImage->GetImageSet()->GetTexture();
        SetSubRect(_pSubImage->GetRect());
    }
}

Texture* ElementSprite::GetTexture() const
{
    return m_texture;
}

void ElementSprite::SetSubRect(const sf::IntRect& _oRect)
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

    RecomputeVerticesTexture();
}

sf::IntRect ElementSprite::GetSubRect() const
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

void ElementSprite::SetFlipTextureX(bool _bFlipTextureX)
{
    SetFlipTexture(_bFlipTextureX, m_flipTextureY);
}

void ElementSprite::SetFlipTextureY(bool _bFlipTextureY)
{
    SetFlipTexture(m_flipTextureX, _bFlipTextureY);
}

void ElementSprite::SetFlipTexture(bool _bFlipTextureX, bool _bFlipTextureY)
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

    RecomputeVerticesTexture();
}

void ElementSprite::SetColor(const sf::Color& _oColor)
{
    m_color = _oColor;

    RecomputeVerticesColor();
}

sf::Color ElementSprite::GetColor() const
{
    return m_color;
}

void ElementSprite::SetSizeImpl(sf::Vector2f _kOldSize)
{
    RecomputeVerticesPosition();
}

void ElementSprite::RecomputeVerticesPosition()
{
    sf::Vector2f kAreaSize = GetSize();

    m_vertices[0].position = sf::Vector2f(0.f, 0.f);
    m_vertices[1].position = sf::Vector2f(kAreaSize.x, 0.f);
    m_vertices[2].position = sf::Vector2f(0.f, kAreaSize.y);
    m_vertices[3].position = sf::Vector2f(kAreaSize.x, kAreaSize.y);
}

void ElementSprite::RecomputeVerticesTexture()
{
    float fLeft = (float)m_subRect.left;
    float fTop = (float)m_subRect.top;
    float fRight = fLeft + m_subRect.width;
    float fBottom = fTop + m_subRect.height;

    m_vertices[0].texCoords = sf::Vector2f(fLeft, fTop);
    m_vertices[1].texCoords = sf::Vector2f(fRight, fTop);
    m_vertices[2].texCoords = sf::Vector2f(fLeft, fBottom);
    m_vertices[3].texCoords = sf::Vector2f(fRight, fBottom);
}

void ElementSprite::RecomputeVerticesColor()
{
    m_vertices[0].color = m_color;
    m_vertices[1].color = m_color;
    m_vertices[2].color = m_color;
    m_vertices[3].color = m_color;
}

void ElementSprite::DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(sf::Vector2f(), m_size));
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
    {
        // Draw
        sf::RenderStates states;
        states.transform = _kTransformSelf;
        states.texture = m_texture->GetSFTexture();
        _kRenderPass.target->draw(m_vertices, states);

        // Stats
        if (_kRenderPass.frameInfos)
            _kRenderPass.frameInfos->statDrawCalls += 1;

        _kRenderPass.statRenderedSprites += 1;
    }
}

bool ElementSprite::LoadFromXml(const pugi::xml_node& _oNodeElement)
{
    if (!Element::LoadFromXml(_oNodeElement))
        return false;

    pugi::xml_node oNodeTexture = _oNodeElement.child("Texture");
    if (!oNodeTexture.empty())
    {
        std::string strTexturePath = oNodeTexture.attribute("Source").as_string("");
        SetTexture(strTexturePath);
    }

    return true;
}

}   // namespace gugu
