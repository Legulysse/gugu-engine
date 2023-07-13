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

    void SetTexture(const std::string& _strTexturePath, bool updateTextureRect = true, bool updateSize = true);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture, bool updateTextureRect = true, bool updateSize = true);
    void SetSubImage(const std::string& _strImageSetName, const std::string& _strSubImageName, bool updateSize = true);
    void SetSubImage(SubImage* _pSubImage, bool updateSize = true);

    Texture* GetTexture() const;

protected:

    void RecomputeVerticesPositionAndTextureCoords();
    void RecomputeVerticesColor();

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    Texture* m_texture;
    sf::VertexArray m_vertices;
};

}   // namespace gugu
