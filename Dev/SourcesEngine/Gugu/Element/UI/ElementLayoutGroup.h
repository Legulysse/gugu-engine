#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

namespace ELayoutDirection
{
    enum Type
    {
        LeftToRight,    // (Default for main direction).
        RightToLeft,
        TopToBottom,
        BottomToTop,
        Auto,           // (Default for wrap direction) (Ignored by SetMainDirection).
    };
}

namespace ELayoutBehaviour
{
    enum Type
    {
        ResizeContainer,    // Resize the container to fit the space occupied by the items (Default).
        Overflow,           // Let the items move outside of the container bounds.
        //HideItems,
        //ResizeItems,
        WrapItems,          // Wrap items to fill a new row/column when the current one is overflowing (Ignored by SetWrapDirectionBehaviour).
    };
}

class ElementLayoutGroup : public Element
{
public:

    ElementLayoutGroup();
    virtual ~ElementLayoutGroup();

    void AddItem(Element* item);
    void RemoveItem(Element* item);
    void DeleteItem(Element* item);
    void DeleteAllItems();

    size_t GetItemCount() const;
    const std::vector<Element*>& GetItems() const;

    void SetMainDirection(ELayoutDirection::Type direction);
    void SetWrapDirection(ELayoutDirection::Type direction);
    void SetMainDirectionBehaviour(ELayoutBehaviour::Type behaviour);
    void SetWrapDirectionBehaviour(ELayoutBehaviour::Type behaviour);
    
    ELayoutDirection::Type GetMainDirection() const;
    ELayoutDirection::Type GetWrapDirection() const;
    ELayoutBehaviour::Type GetMainDirectionBehaviour() const;
    ELayoutBehaviour::Type GetWrapDirectionBehaviour() const;

    void SetItemSpacing(float spacing);
    void SetItemSpacing(Vector2f spacing);
    Vector2f GetItemSpacing() const;

private:

    virtual void RecomputeImpl() override;
    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
protected:

    std::vector<Element*> m_items;
    ELayoutDirection::Type m_mainDirection;
    ELayoutDirection::Type m_wrapDirection;
    ELayoutBehaviour::Type m_mainDirectionBehaviour;
    ELayoutBehaviour::Type m_wrapDirectionBehaviour;
    Vector2f m_spacing;
};

}   // namespace gugu
