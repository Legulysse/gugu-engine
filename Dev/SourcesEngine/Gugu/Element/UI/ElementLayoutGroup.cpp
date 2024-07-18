////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementLayoutGroup.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Container.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementLayoutGroup::ElementLayoutGroup()
    : m_mainDirection(ELayoutDirection::LeftToRight)
    , m_wrapDirection(ELayoutDirection::Auto)
    , m_mainDirectionBehaviour(ELayoutBehaviour::ResizeContainer)
    , m_wrapDirectionBehaviour(ELayoutBehaviour::ResizeContainer)
{
}

ElementLayoutGroup::~ElementLayoutGroup()
{
    ClearStdVector(m_items);
}

void ElementLayoutGroup::SetMainDirection(ELayoutDirection::Type direction)
{
    if (direction == ELayoutDirection::Auto)
        return;

    m_mainDirection = direction;
    RaiseNeedRecompute();
}

void ElementLayoutGroup::SetWrapDirection(ELayoutDirection::Type direction)
{
    m_wrapDirection = direction;
    RaiseNeedRecompute();
}

void ElementLayoutGroup::SetMainDirectionBehaviour(ELayoutBehaviour::Type behaviour)
{
    m_mainDirectionBehaviour = behaviour;
    RaiseNeedRecompute();
}

void ElementLayoutGroup::SetWrapDirectionBehaviour(ELayoutBehaviour::Type behaviour)
{
    if (behaviour == ELayoutBehaviour::WrapItems)
        return;

    m_wrapDirectionBehaviour = behaviour;
    RaiseNeedRecompute();
}

void ElementLayoutGroup::AddItem(Element* item)
{
    item->SetParent(this);
    m_items.push_back(item);

    RaiseNeedRecompute();
}

void ElementLayoutGroup::RemoveItem(Element* item)
{
    if (item->GetParent() != this)
        return;

    StdVectorRemove(m_items, item);
    item->SetParent(nullptr);

    RaiseNeedRecompute();
}

void ElementLayoutGroup::DeleteItem(Element* item)
{
    if (item->GetParent() != this)
        return;

    StdVectorRemove(m_items, item);
    SafeDelete(item);

    RaiseNeedRecompute();
}

void ElementLayoutGroup::DeleteAllItems()
{
    ClearStdVector(m_items);

    RaiseNeedRecompute();
}

size_t ElementLayoutGroup::GetItemCount() const
{
    return m_items.size();
}

const std::vector<Element*>& ElementLayoutGroup::GetItems() const
{
    return m_items;
}

ELayoutDirection::Type ElementLayoutGroup::GetMainDirection() const
{
    return m_mainDirection;
}

ELayoutDirection::Type ElementLayoutGroup::GetWrapDirection() const
{
    return m_wrapDirection;
}

ELayoutBehaviour::Type ElementLayoutGroup::GetMainDirectionBehaviour() const
{
    return m_mainDirectionBehaviour;
}

ELayoutBehaviour::Type ElementLayoutGroup::GetWrapDirectionBehaviour() const
{
    return m_wrapDirectionBehaviour;
}

void ElementLayoutGroup::SetItemSpacing(float spacing)
{
    SetItemSpacing(Vector2f(spacing, spacing));
}

void ElementLayoutGroup::SetItemSpacing(Vector2f spacing)
{
    m_spacing = spacing;
    RaiseNeedRecompute();
}

Vector2f ElementLayoutGroup::GetItemSpacing() const
{
    return m_spacing;
}

void ElementLayoutGroup::RecomputeImpl()
{
    ELayoutDirection::Type mainDirection = m_mainDirection;
    ELayoutDirection::Type wrapDirection = m_wrapDirection;

    if (wrapDirection == ELayoutDirection::Auto)
    {
        switch (mainDirection)
        {
            case ELayoutDirection::LeftToRight:
            case ELayoutDirection::RightToLeft:
                wrapDirection = ELayoutDirection::TopToBottom;
                break;
            case ELayoutDirection::TopToBottom:
            case ELayoutDirection::BottomToTop:
                wrapDirection = ELayoutDirection::LeftToRight;
                break;
        }
    }

    Vector2f referencePosition = Vector2::Zero_f;

    switch (mainDirection)
    {
        case ELayoutDirection::LeftToRight: referencePosition.x = 0.f; break;
        case ELayoutDirection::RightToLeft: referencePosition.x = GetSize().x; break;
        case ELayoutDirection::TopToBottom: referencePosition.y = 0.f; break;
        case ELayoutDirection::BottomToTop: referencePosition.y = GetSize().y; break;
    }

    switch (wrapDirection)
    {
        case ELayoutDirection::LeftToRight: referencePosition.x = 0.f; break;
        case ELayoutDirection::RightToLeft: referencePosition.x = GetSize().x; break;
        case ELayoutDirection::TopToBottom: referencePosition.y = 0.f; break;
        case ELayoutDirection::BottomToTop: referencePosition.y = GetSize().y; break;
    }

    Vector2f minPosition = referencePosition;
    Vector2f maxPosition = referencePosition;
    Vector2f currentGroupMinPosition = referencePosition;
    Vector2f currentGroupMaxPosition = referencePosition;

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        // TODO: Handle hidden items ?

        Vector2f itemSize = m_items[i]->GetSize();

        Vector2f currentGroupSize = Vector2::Zero_f;
        currentGroupSize.x = currentGroupMaxPosition.x - currentGroupMinPosition.x;
        currentGroupSize.y = currentGroupMaxPosition.y - currentGroupMinPosition.y;

        if (i != 0 && m_mainDirectionBehaviour == ELayoutBehaviour::WrapItems)
        {
            bool resetMainDirection = false;

            switch (mainDirection)
            {
                case ELayoutDirection::LeftToRight:
                case ELayoutDirection::RightToLeft:
                    if (currentGroupSize.x + itemSize.x > GetSize().x)
                    {
                        resetMainDirection = true;
                    }
                    break;
                case ELayoutDirection::TopToBottom:
                case ELayoutDirection::BottomToTop:
                    if (currentGroupSize.y + itemSize.y > GetSize().y)
                    {
                        resetMainDirection = true;
                    }
                    break;
            }

            if (resetMainDirection)
            {
                switch (mainDirection)
                {
                    case ELayoutDirection::LeftToRight: referencePosition.x = 0.f; break;
                    case ELayoutDirection::RightToLeft: referencePosition.x = GetSize().x; break;
                    case ELayoutDirection::TopToBottom: referencePosition.y = 0.f; break;
                    case ELayoutDirection::BottomToTop: referencePosition.y = GetSize().y; break;
                }

                switch (wrapDirection)
                {
                    case ELayoutDirection::LeftToRight: referencePosition += Vector2f(currentGroupSize.x + m_spacing.x, 0.f); break;
                    case ELayoutDirection::RightToLeft: referencePosition -= Vector2f(currentGroupSize.x + m_spacing.x, 0.f); break;
                    case ELayoutDirection::TopToBottom: referencePosition += Vector2f(0.f, currentGroupSize.y + m_spacing.y); break;
                    case ELayoutDirection::BottomToTop: referencePosition -= Vector2f(0.f, currentGroupSize.y + m_spacing.y); break;
                }

                currentGroupMinPosition = referencePosition;
                currentGroupMaxPosition = referencePosition;
            }
        }

        Vector2f itemPosition = referencePosition;

        switch (mainDirection)
        {
            case ELayoutDirection::LeftToRight: itemPosition.x += 0.f; break;
            case ELayoutDirection::RightToLeft: itemPosition.x -= itemSize.x; break;
            case ELayoutDirection::TopToBottom: itemPosition.y += 0.f; break;
            case ELayoutDirection::BottomToTop: itemPosition.y -= itemSize.y; break;
        }

        switch (wrapDirection)
        {
            case ELayoutDirection::LeftToRight: itemPosition.x += 0.f; break;
            case ELayoutDirection::RightToLeft: itemPosition.x -= itemSize.x; break;
            case ELayoutDirection::TopToBottom: itemPosition.y += 0.f; break;
            case ELayoutDirection::BottomToTop: itemPosition.y -= itemSize.y; break;
        }

        m_items[i]->SetPosition(itemPosition);

        minPosition.x = Min(minPosition.x, itemPosition.x);
        minPosition.y = Min(minPosition.y, itemPosition.y);
        maxPosition.x = Max(maxPosition.x, itemPosition.x + itemSize.x);
        maxPosition.y = Max(maxPosition.y, itemPosition.y + itemSize.y);

        currentGroupMinPosition.x = Min(currentGroupMinPosition.x, itemPosition.x);
        currentGroupMinPosition.y = Min(currentGroupMinPosition.y, itemPosition.y);
        currentGroupMaxPosition.x = Max(currentGroupMaxPosition.x, itemPosition.x + itemSize.x);
        currentGroupMaxPosition.y = Max(currentGroupMaxPosition.y, itemPosition.y + itemSize.y);

        switch (mainDirection)
        {
            case ELayoutDirection::LeftToRight: referencePosition += Vector2f(m_spacing.x + itemSize.x, 0.f); break;
            case ELayoutDirection::RightToLeft: referencePosition -= Vector2f(m_spacing.x + itemSize.x, 0.f); break;
            case ELayoutDirection::TopToBottom: referencePosition += Vector2f(0.f, m_spacing.y + itemSize.y); break;
            case ELayoutDirection::BottomToTop: referencePosition -= Vector2f(0.f, m_spacing.y + itemSize.y); break;
        }
    }

    Vector2f layoutSize = GetSize();

    if (m_mainDirectionBehaviour == ELayoutBehaviour::ResizeContainer)
    {
        switch (mainDirection)
        {
            case ELayoutDirection::LeftToRight:
            case ELayoutDirection::RightToLeft:
                layoutSize.x = maxPosition.x - minPosition.x;
                break;
            case ELayoutDirection::TopToBottom:
            case ELayoutDirection::BottomToTop:
                layoutSize.y = maxPosition.y - minPosition.y;
                break;
        }
    }

    if (m_wrapDirectionBehaviour == ELayoutBehaviour::ResizeContainer)
    {
        switch (wrapDirection)
        {
            case ELayoutDirection::LeftToRight:
            case ELayoutDirection::RightToLeft:
                layoutSize.x = maxPosition.x - minPosition.x;
                break;
            case ELayoutDirection::TopToBottom:
            case ELayoutDirection::BottomToTop:
                layoutSize.y = maxPosition.y - minPosition.y;
                break;
        }
    }

    SetSize(layoutSize);
}

void ElementLayoutGroup::OnSizeChanged()
{
    RaiseNeedRecompute();

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->ComputeUnifiedDimensionsFromParent();
    }
}

void ElementLayoutGroup::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_items[i]->Render(_kRenderPass, _kTransformSelf);
    }
}

}   // namespace gugu
