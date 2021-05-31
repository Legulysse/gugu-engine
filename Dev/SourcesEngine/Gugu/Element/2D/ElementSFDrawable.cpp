////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementSFDrawable::ElementSFDrawable()
{
    m_sfDrawable = nullptr;
}

ElementSFDrawable::~ElementSFDrawable()
{
    SafeDelete(m_sfDrawable);
}

void ElementSFDrawable::SetSFDrawable(sf::Drawable* _pSFDrawable)
{
    SafeDelete(m_sfDrawable);

    m_sfDrawable = _pSFDrawable;
}

sf::Drawable* ElementSFDrawable::GetSFDrawable() const
{
    return m_sfDrawable;
}

void ElementSFDrawable::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (m_sfDrawable)
    {
        _kRenderPass.target->draw(*m_sfDrawable, _kTransformSelf);

        //Stats
        if (_kRenderPass.frameInfos)
            _kRenderPass.frameInfos->statDrawCalls += 1;

        _kRenderPass.statRenderedDrawables += 1;
    }
}

}   // namespace gugu
