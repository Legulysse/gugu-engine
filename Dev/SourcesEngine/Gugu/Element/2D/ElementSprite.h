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
    class SubImage;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementSprite : public Element
{
public:

    ElementSprite();
    virtual ~ElementSprite();

    void SetTexture(const std::string& _strTexturePath);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);
    void SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName);
    void SetSubImage(SubImage* _pSubImage);

    Texture* GetTexture() const;

    void SetSubRect(const sf::IntRect& _oRect);
    sf::IntRect GetSubRect() const;

    void SetFlipTextureX(bool _bFlipTextureX);  // Flip Top-Bottom
    void SetFlipTextureY(bool _bFlipTextureY);  // Flip Left-Right
    void SetFlipTexture(bool _bFlipTextureX, bool _bFlipTextureY);  // Flip Top-Bottom and Left-Right

    void SetColor(const sf::Color& _oColor);
    sf::Color GetColor() const;

    virtual bool LoadFromXml(const pugi::xml_node& _oNodeElement) override;

protected:

    void RecomputeVerticesPosition();
    void RecomputeVerticesTexture();
    void RecomputeVerticesColor();

    virtual void DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void SetSizeImpl(sf::Vector2f _kOldSize) override;

protected:

    Texture* m_texture;
    sf::IntRect m_subRect;
    sf::Color m_color;

    sf::VertexArray m_vertices;

    bool m_flipTextureX;
    bool m_flipTextureY;
};

}   // namespace gugu
