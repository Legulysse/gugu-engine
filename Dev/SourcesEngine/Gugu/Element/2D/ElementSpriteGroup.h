#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSpriteBase.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementSpriteGroup;
    class Texture;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementSpriteGroupItem : public ElementSpriteBase
{
public:

    ElementSpriteGroupItem();
    virtual ~ElementSpriteGroupItem();

    void SetSpriteGroup(ElementSpriteGroup* spriteGroup);

    bool HasDirtyVertices() const;

    size_t RecomputeVertexCount();
    size_t GetCachedVertexCount() const;
    size_t RecomputeItemVertices(sf::VertexArray& vertices, size_t indexFirstVertex);

protected:

    virtual void RaiseDirtyVertices() override;

    virtual void OnTransformChanged() override;
    virtual void OnVisibleChanged() override;

protected:

    ElementSpriteGroup* m_spriteGroup;
    size_t m_cachedVertexCount;
};

class ElementSpriteGroup : public Element
{
public:

    ElementSpriteGroup();
    virtual ~ElementSpriteGroup();

    void SetTexture(const std::string& _strTexturePath);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);

    Texture* GetTexture() const;

    int AddItem(ElementSpriteGroupItem* _pNewItem);
    ElementSpriteGroupItem* GetItem(int _iIndex) const;

    void RaiseNeedRecompute();

    virtual void GetPropagationList(std::vector<Element*>& _vecPropagationList) override;

    bool LoadFromFile(const std::string& _strPath);
    virtual bool LoadFromXml(const pugi::xml_node& nodeSelf) override;

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged(sf::Vector2f _kOldSize) override;

protected:

    Texture* m_texture;
    sf::VertexArray m_vertices;
    bool m_needRecompute;

    std::vector<ElementSpriteGroupItem*> m_items;    //TODO: Rename as Components ?
};

}   // namespace gugu
