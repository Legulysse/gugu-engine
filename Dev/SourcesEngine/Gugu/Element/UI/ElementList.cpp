////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementList.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/UI/ElementListItem.h"
#include "Gugu/System/Container.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementList::ElementList()
    : m_scrollBackground(nullptr)
    , m_scrollSlider(nullptr)
    , m_scrollButtonTop(nullptr)
    , m_scrollButtonBottom(nullptr)
    , m_currentIndexTop(0)
    , m_displayedItemCount(0)
    , m_allowSelection(true)
    , m_multipleSelection(false)
{
    m_scrollBackground = new ElementSprite;
    m_scrollBackground->SetParent(this);
    m_scrollBackground->SetSize(30.f, 30.f);
    m_scrollBackground->SetVisible(false);

    m_scrollSlider = new ElementSprite;
    m_scrollSlider->SetParent(this);
    m_scrollSlider->SetSize(30.f, 30.f);

    m_scrollButtonTop = new ElementSprite;
    m_scrollButtonTop->SetParent(this);

    m_scrollButtonBottom = new ElementSprite;
    m_scrollButtonBottom->SetParent(this);

    GetEvents()->AddCallback(EInteractionEvent::MouseScrolled, STD_BIND_1(&ElementList::OnMouseScrolled, this));
    GetEvents()->AddCallback(EInteractionEvent::MousePressed, STD_BIND_1(&ElementList::OnMousePressed, this));

    m_scrollButtonTop->GetEvents()->AddCallback(EInteractionEvent::MousePressed, std::bind(&ElementList::ScrollItems, this, -1));
    m_scrollButtonBottom->GetEvents()->AddCallback(EInteractionEvent::MousePressed, std::bind(&ElementList::ScrollItems, this, 1));
    m_scrollSlider->GetEvents()->AddCallback(EInteractionEvent::MouseDragMoved, STD_BIND_1(&ElementList::OnSliderDragMoved, this));
}

ElementList::~ElementList()
{
    ClearStdVector(m_items);

    SafeDelete(m_scrollSlider);
    SafeDelete(m_scrollBackground);
    SafeDelete(m_scrollButtonTop);
    SafeDelete(m_scrollButtonBottom);
}

void ElementList::SetImageSet(const std::string& _strImageSetPath)
{
    m_scrollBackground->SetVisible(true);
    m_scrollBackground->SetSubImage(_strImageSetPath, "Background");
    m_scrollSlider->SetSubImage(_strImageSetPath, "Slider");
    m_scrollButtonTop->SetSubImage(_strImageSetPath, "ButtonTop");
    m_scrollButtonBottom->SetSubImage(_strImageSetPath, "ButtonBottom");

    RecomputeScrollBar();
    RecomputeItems();
}

void ElementList::AddItem(ElementListItem* item)
{
    item->SetParent(this);
    m_items.push_back(item);

    Vector2f kListSize(m_size.x - m_scrollSlider->GetSize().x, m_size.y);
    item->OnListResized(kListSize);

    // TODO: I should avoid calling this for every Add/Remove and use a delayed refresh.
    RecomputeItems();
}

void ElementList::RemoveItem(size_t index)
{
    if (index < 0 || index > m_items.size())
        return;

    ElementListItem* item = m_items[index];
    StdVectorRemoveAt(m_items, index);
    item->SetParent(nullptr);

    ClampCurrentIndex();
    RecomputeItems();
}

void ElementList::RemoveItem(ElementListItem* item)
{
    if (item->GetParent() != this)
        return;

    StdVectorRemove(m_items, item);
    item->SetParent(nullptr);

    ClampCurrentIndex();
    RecomputeItems();
}

void ElementList::DeleteItem(size_t index)
{
    if (index < 0 || index > m_items.size())
        return;

    ElementListItem* item = m_items[index];
    StdVectorRemoveAt(m_items, index);
    SafeDelete(item);

    ClampCurrentIndex();
    RecomputeItems();
}

void ElementList::DeleteItem(ElementListItem* item)
{
    if (item->GetParent() != this)
        return;

    StdVectorRemove(m_items, item);
    SafeDelete(item);

    ClampCurrentIndex();
    RecomputeItems();
}

void ElementList::DeleteAllItems()
{
    ClearStdVector(m_items);

    ClampCurrentIndex();
    RecomputeItems();
}

void ElementList::GetItems(std::vector<ElementListItem*>& _vecItems) const
{
    _vecItems = m_items;
}

size_t ElementList::GetItemCount() const
{
    return m_items.size();
}

void ElementList::SetAllowSelection(bool _bAllow)
{
    m_allowSelection = _bAllow;
}

void ElementList::SetMultipleSelection(bool _bMultiple)
{
    m_multipleSelection = _bMultiple;
}

void ElementList::SetItemSelected(size_t _iIndex, bool selected)
{
    if (_iIndex >= 0 && _iIndex < m_items.size())
    {
        SetItemSelected(m_items[_iIndex], selected);
    }
}

void ElementList::SetItemSelected(ElementListItem* _pNewItem, bool selected)
{
    if (m_allowSelection && _pNewItem && _pNewItem->IsSelected() != selected)
    {
        _pNewItem->SetSelected(selected);

        if (!m_multipleSelection && selected)
        {
            for (size_t i = 0; i < m_items.size(); ++i)
            {
                ElementListItem* pItem = m_items[i];
                if (pItem != _pNewItem)
                {
                    pItem->SetSelected(false);
                }
            }
        }
    }
}

void ElementList::ToggleItemSelected(size_t _iIndex)
{
    if (_iIndex >= 0 && _iIndex < m_items.size())
    {
        ToggleItemSelected(m_items[_iIndex]);
    }
}

void ElementList::ToggleItemSelected(ElementListItem* _pNewItem)
{
    if (m_allowSelection)
    {
        _pNewItem->SetSelected(!_pNewItem->IsSelected());

        if (!m_multipleSelection && _pNewItem->IsSelected())
        {
            for (size_t i = 0; i < m_items.size(); ++i)
            {
                ElementListItem* pItem = m_items[i];
                if (pItem != _pNewItem)
                {
                    pItem->SetSelected(false);
                }
            }
        }
    }
}

size_t ElementList::GetSelectedIndex() const
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i]->IsSelected())
            return i;
    }

    return system::InvalidIndex;
}

ElementListItem* ElementList::GetSelectedItem() const
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i]->IsSelected())
            return m_items[i];
    }

    return nullptr;
}

Element* ElementList::GetSelectedElement() const
{
    ElementListItem* pItem = GetSelectedItem();
    if (pItem)
        return pItem->GetElement();
    return nullptr;
}

void ElementList::GetSelectedIndexes(std::vector<size_t>& _vecIndexes) const
{
    _vecIndexes.clear();
    
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i]->IsSelected())
            _vecIndexes.push_back(i);
    }
}

void ElementList::GetSelectedItems(std::vector<ElementListItem*>& _vecItems) const
{
    _vecItems.clear();

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        ElementListItem* pItem = m_items[i];
        if (pItem->IsSelected())
            _vecItems.push_back(pItem);
    }
}

void ElementList::GetSelectedElements(std::vector<Element*>& _vecElements) const
{
    _vecElements.clear();
    
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        ElementListItem* pItem = m_items[i];
        if (pItem->IsSelected())
            _vecElements.push_back(pItem->GetElement());
    }
}

void ElementList::OnMousePressed(const InteractionInfos& interactionInfos)
{
    Vector2f localPickedCoords = interactionInfos.localPickingPosition;

    for (size_t i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        Vector2f itemLocalPickedCoords = m_items[i]->TransformToLocal(localPickedCoords, this);
        if (m_items[i]->IsPickedLocal(itemLocalPickedCoords))
        {
            ToggleItemSelected(i);
            return;
        }
    }
}

void ElementList::OnMouseScrolled(const InteractionInfos& interactionInfos)
{
    ScrollItems(-interactionInfos.scrollDelta);
}

void ElementList::OnSliderDragMoved(const InteractionInfos& interactionInfos)
{
    if (m_items.size() != 0)
    {
        float fScrollAreaSize = Max(0.f, GetSize().y - m_scrollButtonTop->GetSize().y - m_scrollButtonBottom->GetSize().y /* - m_pScrollSlider->GetSize().y*/);
        float fGap = Clamp(m_scrollSlider->GetPosition().y - m_scrollButtonTop->GetSize().y, 0.f, fScrollAreaSize);
        
        int iDesiredIndex = (int)(fGap / (fScrollAreaSize / (float)m_items.size()));
        ScrollItems(iDesiredIndex - (int)m_currentIndexTop);
    }
    else
    {
        //TODO: Maybe disable the drag interaction when items list is empty
        RecomputeItems();
    }
}

void ElementList::ClampCurrentIndex()
{
    m_currentIndexTop = m_items.size() == 0 ? 0 : Clamp<size_t>(m_currentIndexTop, 0, m_items.size() - 1);
}

int ElementList::ScrollItems(int _iDelta)
{
    int iComputedDelta = 0;

    if (m_items.size() == 0)
    {
        m_currentIndexTop = 0;
    }
    else
    {
        size_t iOldIndexTop = m_currentIndexTop;

        if (_iDelta < 0)
        {
            m_currentIndexTop = m_currentIndexTop - Min(m_currentIndexTop, (size_t)Absolute(_iDelta));
        }
        else
        {
            m_currentIndexTop = m_currentIndexTop + Min(m_items.size() - m_currentIndexTop, (size_t)_iDelta);
        }

        RecomputeItems();   //This may change m_iCurrentIndexTop

        iComputedDelta = (int)m_currentIndexTop - (int)iOldIndexTop;
    }

    return iComputedDelta;
}

void ElementList::RecomputeScrollBar()
{
    //TODO : UDim
    m_scrollButtonTop->SetPosition(GetSize().x - m_scrollButtonTop->GetSize().x, 0.f);
    m_scrollButtonBottom->SetPosition(GetSize().x - m_scrollButtonBottom->GetSize().x, GetSize().y - m_scrollButtonBottom->GetSize().y);
    m_scrollSlider->SetPosition(GetSize().x - m_scrollSlider->GetSize().x, m_scrollSlider->GetPosition().y);
    m_scrollBackground->SetPosition(GetSize().x - m_scrollBackground->GetSize().x, m_scrollBackground->GetPosition().y);
    m_scrollBackground->SetSizeY(GetSize().y);
}

void ElementList::RecomputeItems()
{
    Element* pElement = nullptr;
    float fSizeItems = 0.f;

    m_displayedItemCount = 0;

    if (m_items.size() == 0)
    {
        m_currentIndexTop = 0;
    }
    else
    {
        //Compute the number of items displayable from current top
        for (size_t i = m_currentIndexTop; i < m_items.size(); ++i)
        {
            pElement = m_items[i];
            fSizeItems += pElement->GetSize().y;

            if (fSizeItems > GetSize().y)
                break;

            ++m_displayedItemCount;
        }

        //Compute additional number of items by moving the top
        if (m_currentIndexTop > 0)
        {
            size_t i = m_currentIndexTop;
            while (i != 0)
            {
                --i;

                pElement = m_items[i];
                fSizeItems += pElement->GetSize().y;

                if (fSizeItems > GetSize().y)
                    break;

                m_currentIndexTop = i;
                ++m_displayedItemCount;
            }
        }

        //Flag all items as invisible
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            m_items[i]->SetVisible(false);
        }

        //Compute visible items final position
        float x = 0.f;
        float y = 0.f;

        for (size_t i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
        {
            pElement = m_items[i];

            pElement->SetVisible(true);
            pElement->SetPosition(x, y);
            y += pElement->GetSize().y;
        }
    }

    //Compute scroll size/position
    m_scrollSlider->SetPosition(GetSize().x - m_scrollSlider->GetSize().x, m_scrollSlider->GetPosition().y);

    if (m_displayedItemCount < m_items.size())
    {
        float fItemSize = (GetSize().y - m_scrollButtonTop->GetSize().y - m_scrollButtonBottom->GetSize().y) / m_items.size();

        m_scrollSlider->SetSizeY(m_displayedItemCount * fItemSize);
        m_scrollSlider->SetPosition(m_scrollSlider->GetPosition().x, m_scrollButtonTop->GetSize().y + Clamp(m_currentIndexTop * fItemSize, 0.f, GetSize().y - m_scrollButtonTop->GetSize().y - m_scrollButtonBottom->GetSize().y - m_scrollSlider->GetSize().y));
    }
    else
    {
        m_scrollSlider->SetSizeY(GetSize().y - m_scrollButtonTop->GetSize().y - m_scrollButtonBottom->GetSize().y);
        m_scrollSlider->SetPosition(m_scrollSlider->GetPosition().x, m_scrollButtonTop->GetSize().y);
    }
}

void ElementList::OnSizeChanged()
{
    RecomputeScrollBar();

    Vector2f kListSize(m_size.x - m_scrollSlider->GetSize().x, m_size.y);
    
    for (size_t i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        m_items[i]->OnListResized(kListSize);
    }

    RecomputeItems();
}

void ElementList::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    // Check if items changed their size and need to be updated.
    bool updatedItemSize = false;

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (!m_items[i]->IsItemSizedToContent())
        {
            m_items[i]->ResizeToContent();
            updatedItemSize = true;
        }
    }

    if (updatedItemSize)
    {
        RecomputeItems();
    }

    // Render visible items.
    for (size_t i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        m_items[i]->Render(_kRenderPass, _kTransformSelf);
    }

    // Render scroll bar.
    m_scrollBackground->Render(_kRenderPass, _kTransformSelf);
    m_scrollSlider->Render(_kRenderPass, _kTransformSelf);
    m_scrollButtonTop->Render(_kRenderPass, _kTransformSelf);
    m_scrollButtonBottom->Render(_kRenderPass, _kTransformSelf);
}

}   // namespace gugu
