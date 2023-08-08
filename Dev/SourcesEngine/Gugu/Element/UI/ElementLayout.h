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
        Vertical,
        Horizontal,
    };
}

class ElementLayout : public Element
{
public:

    ElementLayout();
    virtual ~ElementLayout();

    void AddItem(Element* item);

    void SetLayoutDirection(ELayoutDirection::Type direction);
    ELayoutDirection::Type GetLayoutDirection() const;

    // TODO: autoresize and wrap are mutually exclusive, and interact with USize, maybe I could use an enum instead ?
    // - LayoutBehaviour: WrapItems, FitDirection, FitSize
    // - I will also need a warning in Recompute if some settings collide.
    void SetAutoResize(bool autoResize);
    void SetWrapItems(bool wrap);

    void SetItemSpacing(float spacing);
    void SetItemSpacing(Vector2f spacing);
    Vector2f GetItemSpacing() const;

private:

    void Recompute();

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
protected:

    std::vector<Element*> m_items;
    ELayoutDirection::Type m_direction;
    Vector2f m_spacing;
    bool m_autoResize;
    bool m_wrap;
    bool m_needRecompute;
};

}   // namespace gugu
