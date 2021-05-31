#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSpriteBase.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class SubImage;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementSprite : public ElementSpriteBase
{
public:

    ElementSprite();
    virtual ~ElementSprite();

    void SetTexture(const std::string& _strTexturePath);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);
    void SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName);
    void SetSubImage(SubImage* _pSubImage);

    Texture* GetTexture() const;

    virtual bool LoadFromXml(const pugi::xml_node& _oNodeElement) override;

protected:

    void RecomputeVerticesPositionAndTextureCoords();
    void RecomputeVerticesColor();

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

protected:

    Texture* m_texture;
    sf::VertexArray m_vertices;
};

}   // namespace gugu
