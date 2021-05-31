#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
    class Drawable;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class ElementSFDrawable : public Element
{
public:

    ElementSFDrawable();
    virtual ~ElementSFDrawable();

    void            SetSFDrawable   (sf::Drawable* _pSFDrawable);
    sf::Drawable*   GetSFDrawable   () const;
    
private:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    
protected:

    sf::Drawable* m_sfDrawable;
};

}   // namespace gugu
