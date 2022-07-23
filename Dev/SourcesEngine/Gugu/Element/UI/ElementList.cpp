////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/UI/ElementList.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Renderer.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/UI/ElementListItem.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
ElementList::ElementList()
{
    m_currentIndexTop = 0;
    m_displayedItemCount = 0;
    m_allowSelection = true;
    m_multipleSelection = false;
    
    ElementSprite* pSprite;

    pSprite = new ElementSprite;
    pSprite->SetParent(this);
    pSprite->SetSize(30.f, 30.f);
    pSprite->SetVisible(false);
    m_scrollBackground = pSprite;

    pSprite = new ElementSprite;
    pSprite->SetParent(this);
    pSprite->SetSize(30.f, 30.f);
    m_scrollSlider = pSprite;

    m_scrollButtonTop = new ElementSprite;
    m_scrollButtonTop->SetParent(this);

    m_scrollButtonBottom = new ElementSprite;
    m_scrollButtonBottom->SetParent(this);

    GetInteractions()->AddCallback(EElementInteractionEvent::MouseScrolled, std::bind(&ElementList::OnMouseScrolled, this, std::placeholders::_1));

    m_scrollButtonTop->GetInteractions()->SetDependsOnPropagationList();
    m_scrollButtonBottom->GetInteractions()->SetDependsOnPropagationList();
    m_scrollSlider->GetInteractions()->SetDependsOnPropagationList();
    

    m_scrollButtonTop->GetInteractions()->AddCallback(EElementInteractionEvent::MousePressed, std::bind(&ElementList::ScrollItems, this, -1));
    m_scrollButtonBottom->GetInteractions()->AddCallback(EElementInteractionEvent::MouseReleased, std::bind(&ElementList::ScrollItems, this, 1));
    m_scrollSlider->GetInteractions()->AddCallback(EElementInteractionEvent::MouseDragMove, std::bind(&ElementList::OnSliderDragMove, this, std::placeholders::_1));
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

void ElementList::SetAllowSelection(bool _bAllow)
{
    m_allowSelection = _bAllow;
}

void ElementList::SetMultipleSelection(bool _bMultiple)
{
    m_multipleSelection = _bMultiple;
}

void ElementList::AddItem(ElementListItem* _pNewItem)
{
    _pNewItem->SetList(this);

    Vector2f kListSize(m_size.x - m_scrollSlider->GetSize().x, m_size.y);
    _pNewItem->OnListResized(kListSize);

    m_items.push_back(_pNewItem);

    RecomputeItems();
}

void ElementList::RemoveItem(int _iIndex)
{
    ElementListItem* pItem = m_items[_iIndex];
    RemoveItem(pItem);
}

void ElementList::RemoveItem(ElementListItem* _pItem)
{
    for (auto iteItem = m_items.begin(); iteItem != m_items.end(); ++iteItem)
    {
        if (*iteItem == _pItem)
        {
            SafeDelete(_pItem);
            m_items.erase(iteItem);
            break;
        }
    }

    m_currentIndexTop = Clamp(m_currentIndexTop, 0, (int)m_items.size() - 1);

    RecomputeItems();
}

void ElementList::RemoveAllItems()
{
    ClearStdVector(m_items);
    m_currentIndexTop = 0;

    RecomputeItems();
}

void ElementList::GetItems(std::vector<ElementListItem*>& _vecItems) const
{
    _vecItems = m_items;
}

int ElementList::GetItemCount() const
{
    return m_items.size();
}

void ElementList::SetSelectedItem(int _iIndex)
{
    if (_iIndex >= 0 && _iIndex < (int)m_items.size())
        SetSelectedItem(m_items[_iIndex]);
}

void ElementList::SetSelectedItem(ElementListItem* _pNewItem)
{
    if (m_allowSelection)
    {
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            ElementListItem* pItem = m_items[i];
            if (pItem == _pNewItem)
                pItem->SetSelected(!pItem->IsSelected());
            else if (!m_multipleSelection)
                pItem->SetSelected(false);
        }
    }
}

int ElementList::GetSelectedIndex() const
{
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i]->IsSelected())
            return (int)i;
    }

    return -1;
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

void ElementList::GetSelectedIndexes(std::vector<int>& _vecIndexes) const
{
    _vecIndexes.clear();
    
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i]->IsSelected())
            _vecIndexes.push_back((int)i);
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

void ElementList::OnMouseScrolled(const ElementInteractionInfos& interactionInfos)
{
    ScrollItems(-interactionInfos.scrollDelta);
}

void ElementList::OnSliderDragMove(const ElementInteractionInfos& interactionInfos)
{
    if (m_items.size() != 0)
    {
        float fScrollAreaSize = Max(0.f, GetSize().y - m_scrollButtonTop->GetSize().y - m_scrollButtonBottom->GetSize().y /* - m_pScrollSlider->GetSize().y*/);
        float fGap = Clamp(m_scrollSlider->GetPosition().y - m_scrollButtonTop->GetSize().y, 0.f, fScrollAreaSize);
        
        int iDesiredIndex = (int)(fGap / (fScrollAreaSize / (float)m_items.size()));
        ScrollItems(iDesiredIndex - m_currentIndexTop);
    }
    else
    {
        //TODO: Maybe disable the drag interaction when items list is empty
        RecomputeItems();
    }
}

int ElementList::ScrollItems(int _iDelta)
{
    int iComputedDelta = 0;

    if (m_items.size() > 0)
    {
        int iOldIndexTop    = m_currentIndexTop;
        m_currentIndexTop  = Clamp(m_currentIndexTop + _iDelta, 0, (int)m_items.size() - 1);

        RecomputeItems();   //This may change m_iCurrentIndexTop

        iComputedDelta = m_currentIndexTop - iOldIndexTop;
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
        for (int i = m_currentIndexTop; i < (int)m_items.size(); ++i)
        {
            pElement = m_items[i];
            fSizeItems += pElement->GetSize().y;

            if (fSizeItems > GetSize().y)
                break;

            ++m_displayedItemCount;
        }

        //Compute additional number of items by moving the top
        for (int i = m_currentIndexTop - 1; i >= 0; --i)
        {
            pElement = m_items[i];
            fSizeItems += pElement->GetSize().y;

            if (fSizeItems > GetSize().y)
                break;

            m_currentIndexTop = i;
            ++m_displayedItemCount;
        }

        //Flag all items as invisible
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            m_items[i]->SetVisible(false);
        }

        //Compute visible items final position
        float x = 0.f;
        float y = 0.f;

        for (int i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
        {
            pElement = m_items[i];

            pElement->SetVisible(true);
            pElement->SetPosition(x, y);
            y += pElement->GetSize().y;
        }
    }

    //Compute scroll size/position
    m_scrollSlider->SetPosition(GetSize().x - m_scrollSlider->GetSize().x, m_scrollSlider->GetPosition().y);

    if (m_displayedItemCount < (int)m_items.size())
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

void ElementList::GetPropagationList(std::vector<ElementEvents*>& _vecPropagationList)
{
    _vecPropagationList.push_back(m_scrollButtonTop->GetInteractions());
    _vecPropagationList.push_back(m_scrollSlider->GetInteractions());
    _vecPropagationList.push_back(m_scrollButtonBottom->GetInteractions());

    for (int i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        _vecPropagationList.push_back(m_items[i]->GetInteractions());
    }
}

void ElementList::OnSizeChanged()
{
    RecomputeScrollBar();

    Vector2f kListSize(m_size.x - m_scrollSlider->GetSize().x, m_size.y);
    
    for (int i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        m_items[i]->OnListResized(kListSize);
    }

    RecomputeItems();
}

void ElementList::RenderImpl(RenderPass& _kRenderPass, const sf::Transform& _kTransformSelf)
{
    for (int i = m_currentIndexTop; i < m_currentIndexTop + m_displayedItemCount; ++i)
    {
        m_items[i]->Render(_kRenderPass, _kTransformSelf);
    }

    m_scrollBackground->Render(_kRenderPass, _kTransformSelf);
    m_scrollSlider->Render(_kRenderPass, _kTransformSelf);
    m_scrollButtonTop->Render(_kRenderPass, _kTransformSelf);
    m_scrollButtonBottom->Render(_kRenderPass, _kTransformSelf);
}

}   // namespace gugu
