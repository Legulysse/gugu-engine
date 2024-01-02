#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementSprite;
    class ElementListItem;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementList : public Element
{
public:

    ElementList();
    virtual ~ElementList();

    void SetImageSet        (const std::string& _strImageSetPath);
    
    void AddItem            (ElementListItem* item);
    void RemoveItem         (size_t index);
    void RemoveItem         (ElementListItem* item);
    void DeleteItem         (size_t index);
    void DeleteItem         (ElementListItem* item);
    void DeleteAllItems     ();
    
    void    GetItems        (std::vector<ElementListItem*>& _vecItems) const;
    size_t  GetItemCount    () const;

    void SetAllowSelection      (bool _bAllow);
    void SetMultipleSelection   (bool _bMultiple);

    void SetItemSelected    (size_t _iIndex, bool selected);
    void SetItemSelected    (ElementListItem* _pNewItem, bool selected);
    void ToggleItemSelected (size_t _iIndex);
    void ToggleItemSelected (ElementListItem* _pNewItem);

    size_t              GetSelectedIndex    () const;
    ElementListItem*    GetSelectedItem     () const;
    Element*            GetSelectedElement  () const;
    void                GetSelectedIndexes  (std::vector<size_t>& _vecIndexes) const;
    void                GetSelectedItems    (std::vector<ElementListItem*>& _vecItems) const;
    void                GetSelectedElements (std::vector<Element*>& _vecElements) const;

protected:

    void OnMousePressed(const InteractionInfos& interactionInfos);
    void OnMouseScrolled(const InteractionInfos& interactionInfos);
    void OnSliderDragMoved(const InteractionInfos& interactionInfos);

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;

    void    ClampCurrentIndex();
    int     ScrollItems(int _iDelta);

    void    RecomputeScrollBar();
    void    RecomputeItems();
    
protected:

    ElementSprite* m_scrollBackground;
    ElementSprite* m_scrollSlider;
    ElementSprite* m_scrollButtonTop;
    ElementSprite* m_scrollButtonBottom;

    std::vector<ElementListItem*> m_items;
    size_t m_currentIndexTop;
    size_t m_displayedItemCount;

    bool m_allowSelection;
    bool m_multipleSelection;
};

}   // namespace gugu
