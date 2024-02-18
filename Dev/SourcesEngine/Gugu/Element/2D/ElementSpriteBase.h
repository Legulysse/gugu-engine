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

    void SetSubRect(const sf::IntRect& _oRect, bool updateSize = true);
    sf::IntRect GetSubRect() const;

    void SetRepeatTexture(bool repeatTexture);
    bool GetRepeatTexture() const;

    void SetFlipTextureV(bool _bFlipTextureV);  // Flip Top-Bottom
    void SetFlipTextureH(bool _bFlipTextureH);  // Flip Left-Right
    void SetFlipTexture(bool _bFlipTextureV, bool _bFlipTextureH);  // Flip Top-Bottom and Left-Right
    bool GetFlipTextureV() const;
    bool GetFlipTextureH() const;

    void SetColor(const sf::Color& color);
    void SetColorAlpha(uint8 alpha);    // 0 = transparent, 255 = opaque.
    const sf::Color& GetColor() const;

protected:

    size_t GetRequiredVertexCount() const;
    void RecomputeVerticesPositionAndTextureCoords(sf::Vertex* vertices) const;
    void RecomputeVerticesColor(sf::Vertex* vertices, size_t count) const;

    virtual void RaiseDirtyVertices();

    virtual void OnSizeChanged() override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

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
