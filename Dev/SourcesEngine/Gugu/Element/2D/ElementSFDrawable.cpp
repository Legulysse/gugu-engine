////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/System/Memory.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSFDrawable::ElementSFDrawable()
    : m_sfDrawable(nullptr)
    , m_callbackOnSizeChanged(nullptr)
{
}

ElementSFDrawable::~ElementSFDrawable()
{
    SafeDelete(m_sfDrawable);
}

void ElementSFDrawable::SetSFDrawable(sf::Drawable* _pSFDrawable, const sf::FloatRect& bounds)
{
    SafeDelete(m_sfDrawable);

    // Note: If bounds have a size of zero, the drawable will always get culled (intersection test will fail).
    m_sfDrawable = _pSFDrawable;
    m_bounds = bounds;
}

sf::Drawable* ElementSFDrawable::GetSFDrawable() const
{
    return m_sfDrawable;
}

void ElementSFDrawable::SetCallbackOnSizeChanged(const DelegateElementSizeChanged& callbackOnSizeChanged)
{
    m_callbackOnSizeChanged = callbackOnSizeChanged;
}

void ElementSFDrawable::OnSizeChanged()
{
    if (m_callbackOnSizeChanged)
    {
        m_callbackOnSizeChanged(this);
    }
}

void ElementSFDrawable::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    sf::FloatRect kGlobalTransformed = _kTransformSelf.transformRect(m_bounds);
    if (m_sfDrawable && _kRenderPass.rectViewport.findIntersection(kGlobalTransformed))
    {
        _kRenderPass.target->draw(*m_sfDrawable, _kTransformSelf);

        //Stats
        if (_kRenderPass.frameInfos)
        {
            _kRenderPass.frameInfos->statDrawCalls += 1;
            // TODO: find  away to retrieve triangles count.
        }

        _kRenderPass.statRenderedDrawables += 1;
    }
}

}   // namespace gugu
