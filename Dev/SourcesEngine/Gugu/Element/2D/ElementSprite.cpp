////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSprite.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementData.h"
#include "Gugu/Element/ElementUtility.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/ImageSet.h"
#include "Gugu/Window/Renderer.h"

#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSprite::ElementSprite()
    : m_texture(nullptr)
{
}

ElementSprite::~ElementSprite()
{
}

void ElementSprite::SetTexture(const std::string& _strTexturePath, bool updateTextureRect, bool updateSize)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath), updateTextureRect, updateSize);
}

void ElementSprite::SetTexture(Texture* _pTexture, bool updateTextureRect, bool updateSize)
{
    m_texture = _pTexture;

    if (updateTextureRect)
    {
        if (m_texture)
        {
            SetSubRect(m_texture->GetRect(), updateSize);
        }
        else
        {
            SetSubRect(sf::IntRect(), updateSize);
        }
    }
}

void ElementSprite::SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName, bool updateSize)
{
    ImageSet* pImageSet = GetResources()->GetImageSet(_strImageSetName);
    if (pImageSet)
    {
        SetSubImage(pImageSet->GetSubImage(_strSubImageName), updateSize);
    }
    else
    {
        SetTexture(nullptr);
    }
}

void ElementSprite::SetSubImage(SubImage* _pSubImage, bool updateSize)
{
    if (_pSubImage && _pSubImage->GetImageSet() && _pSubImage->GetImageSet()->GetTexture())
    {
        m_texture = _pSubImage->GetImageSet()->GetTexture();
        SetSubRect(_pSubImage->GetRect(), updateSize);
    }
    else
    {
        SetTexture(nullptr);
    }
}

Texture* ElementSprite::GetTexture() const
{
    return m_texture;
}

void ElementSprite::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (!m_texture || !m_texture->GetSFTexture())
        return;

    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(sf::FloatRect(Vector2::Zero_f, m_size));
    if (_kRenderPass.rectViewport.findIntersection(kGlobalTransformed))
    {
        if (m_dirtyVertices)
        {
            m_dirtyVertices = false;

            RecomputeVerticesPositionAndTextureCoords();
            RecomputeVerticesColor();
        }

        // Draw
        sf::RenderStates states;
        states.transform = _kTransformSelf;
        states.texture = m_texture->GetSFTexture();
        _kRenderPass.target->draw(m_vertices, states);

        // Stats
        if (_kRenderPass.frameInfos)
        {
            _kRenderPass.frameInfos->statDrawCalls += 1;
            _kRenderPass.frameInfos->statTriangles += (int)m_vertices.getVertexCount() / 3;
        }

        _kRenderPass.statRenderedSprites += 1;
    }
}

void ElementSprite::RecomputeVerticesPositionAndTextureCoords()
{
    size_t count = GetRequiredVertexCount();

    // Reset vertices
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(count);

    ElementSpriteBase::RecomputeVerticesPositionAndTextureCoords(&m_vertices[0]);
}

void ElementSprite::RecomputeVerticesColor()
{
    ElementSpriteBase::RecomputeVerticesColor(&m_vertices[0], m_vertices.getVertexCount());
}

bool ElementSprite::LoadFromDataImpl(ElementDataContext& context)
{
    if (!ElementSpriteBase::LoadFromDataImpl(context))
        return false;

    ElementSpriteData* spriteData = dynamic_cast<ElementSpriteData*>(context.data);

    if (spriteData->imageSet)
    {
        if (!spriteData->subImageName.empty())
        {
            SetSubImage(spriteData->imageSet->GetSubImage(spriteData->subImageName), false);
        }
        else
        {
            SetTexture(spriteData->imageSet->GetTexture(), false);
        }
    }
    else if (spriteData->texture)
    {
        SetTexture(spriteData->texture, false);
    }

    return true;
}

}   // namespace gugu
