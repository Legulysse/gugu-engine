#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/System/Callback.h"

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

    using DelegateElementSizeChanged = std::function<void (ElementSFDrawable*)>;

public:

    ElementSFDrawable();
    virtual ~ElementSFDrawable();

    void SetSFDrawable(sf::Drawable* _pSFDrawable, const sf::FloatRect& bounds = sf::FloatRect(Vector2f(0.f, 0.f), Vector2f(1.f, 1.f)));
    sf::Drawable* GetSFDrawable() const;

    void SetBounds(const sf::FloatRect& bounds);
    const sf::FloatRect& GetBounds() const;

    void SetCallbackOnSizeChanged(const DelegateElementSizeChanged& callbackOnSizeChanged);

private:

    virtual void RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf) override;
    virtual void OnSizeChanged() override;
    
protected:

    sf::Drawable* m_sfDrawable;
    sf::FloatRect m_bounds;
    DelegateElementSizeChanged m_callbackOnSizeChanged;
};

}   // namespace gugu
