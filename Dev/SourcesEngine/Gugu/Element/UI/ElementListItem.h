#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/System/Callback.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementList;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementListItem : public Element
{
public:

    ElementListItem();
    virtual ~ElementListItem();

    void SetElement(Element* element);
    void SetElement(Element* element, Element* selectedStateComponent, Element* unselectedStateComponent);
    Element* GetElement() const;

    void SetSelected(bool _bIsSelected);
    bool IsSelected() const;

    void OnListResized(const Vector2f& _oListSize);
    bool IsItemSizedToContent() const;
    void ResizeToContent();
    
    void SetOnSelected(const Callback& callback);
    void SetOnDeselected(const Callback& callback);

private:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;

    virtual void OnParentChanged() override;

protected:

    Element* m_elementImpl;
    Element* m_selectedStateComponent;
    Element* m_unselectedStateComponent;
    bool m_isSelected;

    Callback m_callbackOnSelected;
    Callback m_callbackOnDeselected;
};

}
