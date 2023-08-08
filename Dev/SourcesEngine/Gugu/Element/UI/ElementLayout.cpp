////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementLayout.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementLayout::ElementLayout()
    : m_direction(ELayoutDirection::Vertical)
    , m_autoResize(true)
    , m_wrap(false)
    , m_needRecompute(false)
{
}

ElementLayout::~ElementLayout()
{
    ClearStdVector(m_items);
}

void ElementLayout::SetLayoutDirection(ELayoutDirection::Type direction)
{
    m_direction = direction;
    m_needRecompute = true;
}

void ElementLayout::AddItem(Element* item)
{
    item->SetParent(this);
    m_items.push_back(item);
    m_needRecompute = true;
}

ELayoutDirection::Type ElementLayout::GetLayoutDirection() const
{
    return m_direction;
}

void ElementLayout::SetAutoResize(bool autoResize)
{
    m_autoResize = autoResize;
    m_needRecompute = true;
}

void ElementLayout::SetWrapItems(bool wrap)
{
    m_wrap = wrap;
    m_needRecompute = true;
}

void ElementLayout::SetItemSpacing(float spacing)
{
    m_spacing = Vector2f(spacing, spacing);
    m_needRecompute = true;
}

void ElementLayout::SetItemSpacing(Vector2f spacing)
{
    m_spacing = spacing;
    m_needRecompute = true;
}

Vector2f ElementLayout::GetItemSpacing() const
{
    return m_spacing;
}

void ElementLayout::Recompute()
{
    m_needRecompute = false;

    Vector2f itemPosition = Vector2::Zero_f;
    Vector2f layoutSize = Vector2::Zero_f;

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        Vector2f itemSize = m_items[i]->GetSize();

        if (m_wrap && i != 0)
        {
            if (m_direction == ELayoutDirection::Vertical && itemPosition.y + itemSize.y > GetSize().y)
            {
                itemPosition.x += m_spacing.x + itemSize.x;
                itemPosition.y = 0.f;
            }
            else if (m_direction == ELayoutDirection::Horizontal && itemPosition.x + itemSize.x > GetSize().x)
            {
                itemPosition.x = 0.f;
                itemPosition.y += m_spacing.y + itemSize.y;
            }
        }

        m_items[i]->SetPosition(itemPosition);

        layoutSize.x = Max(layoutSize.x, itemPosition.x + itemSize.x);
        layoutSize.y = Max(layoutSize.y, itemPosition.y + itemSize.y);

        itemPosition += (m_direction == ELayoutDirection::Vertical)
            ? Vector2f(0.f, m_spacing.y + itemSize.y)
            : Vector2f(m_spacing.x + itemSize.x, 0.f);
    }

    if (m_autoResize)
    {
        SetSize(layoutSize);
    }
}

void ElementLayout::OnSizeChanged()
{
    m_needRecompute = true;
}

void ElementLayout::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    if (m_needRecompute)
    {
        Recompute();
    }

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->Render(_kRenderPass, _kTransformSelf);
    }
}

}   // namespace gugu
