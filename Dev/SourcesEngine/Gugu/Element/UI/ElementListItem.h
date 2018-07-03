#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ElementList;
    class Action;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ElementListItem : public Element
{
public:

    ElementListItem();
    virtual ~ElementListItem();

    void        SetList     (ElementList* _pList);
    void        SetElement  (Element* _pElement);
    Element*    GetElement  () const;

    void        SetSelected (bool _bIsSelected);
    bool        IsSelected  () const;

    void        OnListResized   (sf::Vector2f _oListSize);
    
    void        SetOnSelected   (Action* _pAction);
    void        SetOnDeselected (Action* _pAction);

    virtual void OnMouseEnter() override;
    virtual void OnMouseLeave() override;
    virtual bool OnMousePressed() override;
    
private:

    virtual void DrawSelf(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    
protected:

    ElementList*    m_list;
    Element*        m_elementImpl;
    bool            m_isSelected;

    Action*         m_actionOnSelected;
    Action*         m_actionOnDeselected;
};

}
