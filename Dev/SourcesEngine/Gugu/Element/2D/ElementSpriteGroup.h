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

    virtual const std::string& GetSerializedType() const override;

protected:

    virtual void RaiseDirtyVertices() override;

    virtual void OnTransformChanged() override;
    virtual void OnVisibleChanged() override;

    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

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

    size_t AddItem(ElementSpriteGroupItem* _pNewItem);
    ElementSpriteGroupItem* GetItem(size_t _iIndex) const;
    const std::vector<ElementSpriteGroupItem*>& GetItems() const;

    void RaiseNeedRecompute();

    bool LoadFromFile(const std::string& _strPath);
    virtual const std::string& GetSerializedType() const override;

protected:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;

    virtual bool LoadFromXmlImpl(ElementParseContext& context) override;
    virtual bool SaveToXmlImpl(ElementSaveContext& context) const override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    Texture* m_texture;

    sf::VertexArray m_vertices;
    bool m_needRecompute;

    std::vector<ElementSpriteGroupItem*> m_items;    //TODO: Rename as Components ?
};

}   // namespace gugu
