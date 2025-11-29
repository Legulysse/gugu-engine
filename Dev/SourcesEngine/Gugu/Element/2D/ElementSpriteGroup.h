#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSpriteBase.h"

#include <SFML/Graphics/VertexArray.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementWidget;
    class ElementSpriteGroup;
    class Texture;
    class ImageSet;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementSpriteGroupItem : public ElementSpriteBase
{
public:

    ElementSpriteGroupItem();
    virtual ~ElementSpriteGroupItem();

    bool HasDirtyVertices() const;

    size_t RecomputeVertexCount();
    size_t GetCachedVertexCount() const;
    size_t RecomputeItemVertices(sf::VertexArray& vertices, size_t indexFirstVertex);

protected:

    virtual void RaiseDirtyVertices() override;

    virtual void OnParentChanged() override;
    virtual void OnTransformChanged() override;
    virtual void OnVisibleChanged() override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    size_t m_cachedVertexCount;
};

class ElementSpriteGroup : public Element
{
public:

    ElementSpriteGroup();
    virtual ~ElementSpriteGroup();

    bool LoadFromWidget(const std::string& elementWidgetID);
    bool LoadFromWidget(ElementWidget* elementWidget);

    void SetTexture(const std::string& _strTexturePath);  //TODO: Rename as textureID
    void SetTexture(Texture* _pTexture);
    Texture* GetTexture() const;

    ImageSet* GetImageSet() const;

    size_t AddItem(ElementSpriteGroupItem* item);
    size_t InsertItem(ElementSpriteGroupItem* item, size_t index);
    //void RemoveItem(ElementSpriteGroupItem* item);
    ElementSpriteGroupItem* GetItem(size_t _iIndex) const;
    const std::vector<ElementSpriteGroupItem*>& GetItems() const;

    void UpdateItemsColor(const sf::Color& color);
    void UpdateItemsColorAlpha(uint8 alpha);    // 0 = transparent, 255 = opaque.

protected:

    virtual void RecomputeImpl() override;
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;

    virtual bool LoadFromDataImpl(ElementDataContext& context) override;

protected:

    ImageSet* m_imageSet;
    Texture* m_texture;
    sf::VertexArray m_vertices;

    std::vector<ElementSpriteGroupItem*> m_items;    //TODO: Rename as Components ?
};

}   // namespace gugu
