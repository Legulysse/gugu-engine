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
#include "Gugu/External/PugiXmlUtility.h"
#include "Gugu/Math/MathUtility.h"

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

void ElementSprite::SetTexture(const std::string& _strTexturePath)
{
    SetTexture(GetResources()->GetTexture(_strTexturePath));
}

void ElementSprite::SetTexture(Texture* _pTexture)
{
    m_texture = _pTexture;

    if (m_texture)
    {
        SetSubRect(m_texture->GetRect());
    }
    else
    {
        SetSubRect(sf::IntRect());
    }
}

void ElementSprite::SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName)
{
    ImageSet* pImageSet = GetResources()->GetImageSet(_strImageSetName);
    if (pImageSet)
    {
        SetSubImage(pImageSet->GetSubImage(_strSubImageName));
    }
    else
    {
        SetTexture(nullptr);
    }
}

void ElementSprite::SetSubImage(SubImage* _pSubImage)
{
    if (_pSubImage && _pSubImage->GetImageSet() && _pSubImage->GetImageSet()->GetTexture())
    {
        m_texture = _pSubImage->GetImageSet()->GetTexture();
        SetSubRect(_pSubImage->GetRect());
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
    if (_kRenderPass.rectViewport.intersects(kGlobalTransformed))
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
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(count);

    ElementSpriteBase::RecomputeVerticesPositionAndTextureCoords(&m_vertices[0]);
}

void ElementSprite::RecomputeVerticesColor()
{
    ElementSpriteBase::RecomputeVerticesColor(&m_vertices[0], m_vertices.getVertexCount());
}

bool ElementSprite::LoadFromXmlImpl(const pugi::xml_node& node)
{
    if (!ElementSpriteBase::LoadFromXmlImpl(node))
        return false;

    pugi::xml_node oNodeTexture = node.child("Texture");
    if (!oNodeTexture.empty())
    {
        std::string strTexturePath = oNodeTexture.attribute("source").as_string("");
        SetTexture(strTexturePath);
    }

    return true;
}

const std::string& ElementSprite::GetSerializedType() const
{
    static const std::string serializedType = "ElementSprite";
    return serializedType;
}

}   // namespace gugu
