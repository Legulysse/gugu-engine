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
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementSpriteBase : public Element
{
public:

    ElementSpriteBase();
    virtual ~ElementSpriteBase();

    void SetSubRect(const sf::IntRect& _oRect);
    sf::IntRect GetSubRect() const;

    void SetRepeatTexture(bool repeatTexture);

    void SetFlipTextureV(bool _bFlipTextureV);  // Flip Top-Bottom
    void SetFlipTextureH(bool _bFlipTextureH);  // Flip Left-Right
    void SetFlipTexture(bool _bFlipTextureV, bool _bFlipTextureH);  // Flip Top-Bottom and Left-Right

    void SetColor(const sf::Color& _oColor);
    sf::Color GetColor() const;

    virtual bool LoadFromXml(const pugi::xml_node& _oNodeElement) override;

protected:

    size_t GetRequiredVertexCount() const;
    void RecomputeVerticesPositionAndTextureCoords(sf::Vertex* vertices) const;
    void RecomputeVerticesColor(sf::Vertex* vertices, size_t count) const;

    virtual void RaiseDirtyVertices();

    virtual void OnSizeChanged(Vector2f _kOldSize) override;

protected:

    // TODO: Rename as TextureRect ? or TextureCoords ? (sfml uses textureRect for sprites and texCoords for vertices).
    sf::IntRect m_subRect;
    sf::Color m_color;

    bool m_dirtyVertices;

    // TODO: Split into m_repeatTextureX and m_repeatTextureY ?
    // TODO: Add m_repeatTextureSize as a UDim2 to stretch the tiling quads ?
    bool m_repeatTexture;

    bool m_flipTextureV;
    bool m_flipTextureH;
};

}   // namespace gugu
