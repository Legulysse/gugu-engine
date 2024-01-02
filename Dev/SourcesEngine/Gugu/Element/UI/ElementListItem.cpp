////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementListItem.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/UI/ElementList.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Debug/Logger.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementListItem::ElementListItem()
    : m_elementImpl(nullptr)
    , m_isSelected(false)
    , m_callbackOnSelected(nullptr)
    , m_callbackOnDeselected(nullptr)
{
}

ElementListItem::~ElementListItem()
{
    SafeDelete(m_elementImpl);
}

void ElementListItem::OnParentChanged()
{
    if (m_parent && dynamic_cast<ElementList*>(m_parent) == nullptr)
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Element, "A ListItem has been attached to an unsupported parent type (should be a List)");
    }
}

void ElementListItem::SetElement(Element* _pElement)
{
    SafeDelete(m_elementImpl);

    _pElement->SetParent(this);
    m_elementImpl = _pElement;
}

Element* ElementListItem::GetElement() const
{
    return m_elementImpl;
}

void ElementListItem::SetSelected(bool _bIsSelected)
{
    if (m_isSelected == _bIsSelected)
        return;

    m_isSelected = _bIsSelected;
    
    if (m_isSelected && m_callbackOnSelected)
        m_callbackOnSelected();

    if (!m_isSelected && m_callbackOnDeselected)
        m_callbackOnDeselected();
}

bool ElementListItem::IsSelected() const
{
    return m_isSelected;
}

bool ElementListItem::IsItemSizedToContent() const
{
    return ApproxEqual(GetSize().y, m_elementImpl->GetSize().y, math::Epsilon3);
}

void ElementListItem::ResizeToContent()
{
    SetSizeY(m_elementImpl->GetSize().y);
}

void ElementListItem::OnListResized(const Vector2f& _oListSize)
{
    //Resize ElementImpl first
    m_elementImpl->SetSizeX(_oListSize.x);

    //Now resize this item by wrapping its content
    SetSize(_oListSize.x, m_elementImpl->GetSize().y);
}

void ElementListItem::SetOnSelected(const Callback& callback)
{
    m_callbackOnSelected = callback;
}

void ElementListItem::SetOnDeselected(const Callback& callback)
{
    m_callbackOnDeselected = callback;
}

void ElementListItem::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (m_isSelected)
    {
        sf::RectangleShape kBackground = sf::RectangleShape(m_size);
        kBackground.setFillColor(sf::Color::Blue);
        _kRenderPass.target->draw(kBackground, _kTransformSelf);

        // Stats
        if (_kRenderPass.frameInfos)
        {
            _kRenderPass.frameInfos->statDrawCalls += 1;
            // TODO: find  away to retrieve triangles count.
        }
    }

    m_elementImpl->Render(_kRenderPass, _kTransformSelf);
}

}
