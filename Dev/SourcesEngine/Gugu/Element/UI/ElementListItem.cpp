////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementListItem.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/UI/ElementList.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/System/SystemUtility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementListItem::ElementListItem()
{
    m_list = nullptr;
    m_elementImpl = nullptr;
    m_isSelected = false;
    
    m_callbackOnSelected = nullptr;
    m_callbackOnDeselected = nullptr;
}

ElementListItem::~ElementListItem()
{
    SafeDelete(m_elementImpl);
}

void ElementListItem::SetList(ElementList* _pList)
{
    //TODO : remove from previous List
    SetParent(_pList);

    m_list = _pList;
    
    AddInteractionFlag(EInteraction::Focus);
    AddInteractionFlag(EInteraction::Click);
}

void ElementListItem::SetElement(Element* _pElement)
{
    SafeDelete(m_elementImpl);
    m_elementImpl = _pElement;
}

Element* ElementListItem::GetElement() const
{
    return m_elementImpl;
}

void ElementListItem::SetSelected(bool _bIsSelected)
{
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

void ElementListItem::OnListResized(Vector2f _oListSize)
{
    //Resize ElementImpl first
    m_elementImpl->SetSizeX(_oListSize.x);

    //Now resize this item by wrapping its content
    SetSize(_oListSize.x, m_elementImpl->GetSize().y);
}

void ElementListItem::SetOnSelected(Callback callback)
{
    m_callbackOnSelected = callback;
}

void ElementListItem::SetOnDeselected(Callback callback)
{
    m_callbackOnDeselected = callback;
}

void ElementListItem::OnMouseEnter()
{
}

void ElementListItem::OnMouseLeave()
{
}

bool ElementListItem::OnMousePressed()
{
    m_list->SetSelectedItem(this);
    return false;
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
