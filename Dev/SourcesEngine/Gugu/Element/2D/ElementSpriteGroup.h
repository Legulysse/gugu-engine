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

class ElementSpriteGroupItem : public Element
{
public:

    ElementSpriteGroupItem();
    virtual ~ElementSpriteGroupItem();

    int GetItemVertexCount() const;
    bool ComputeItemVertices(sf::VertexArray& vertices, int& indexFirstVertex) const;

public:

    sf::IntRect m_subRect;
    bool m_isTile;   //TODO: Handle tiling
};

class ElementSpriteGroup : public Element
{
public:

    ElementSpriteGroup();
    virtual ~ElementSpriteGroup();

    void SetTexture(const std::string& _strTexturePath);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);

    Texture* GetTexture() const;

    void AddItem(ElementSpriteGroupItem* _pNewItem);
    ElementSpriteGroupItem* GetItem(int _iIndex) const;

    void RecomputeItemVertices(int _iIndex);

    void LoadFromXml(const std::string& _strPath);  //TODO: use a different name from the function used by Layouts loading ?

protected:

    virtual void DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void SetSizeImpl(sf::Vector2f _kOldSize) override;

protected:

    Texture* m_texture;

    sf::VertexArray m_vertices;
    bool m_needRecompute;

    std::vector<ElementSpriteGroupItem*> m_items;    //TODO: Rename as Components ?
};

}   // namespace gugu
