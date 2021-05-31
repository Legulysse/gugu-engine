////////////////////////////////////////////////////////////////
// Header

#include "UI/ElementBar.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

ElementBar::ElementBar()
    : m_lastValue(1.f)
{
}

ElementBar::~ElementBar()
{
}

void ElementBar::InitBar(BarColor _eColor, float _fBarBorderSizeX)
{
    std::string strTemplate = "";
    switch (_eColor)
    {
    default:
    case BarColor::Red:
        strTemplate = "RedBar.xml";
        break;
    case BarColor::Blue:
        strTemplate = "BlueBar.xml";
        break;
    case BarColor::Green:
        strTemplate = "GreenBar.xml";
        break;
    case BarColor::Yellow:
        strTemplate = "YellowBar.xml";
        break;
    }

    LoadFromXml(strTemplate);

    float fBarBorderSizeX = _fBarBorderSizeX;

    ElementSpriteGroupItem* pSpriteBackLeft = GetItem(0);
    ElementSpriteGroupItem* pSpriteBackMid = GetItem(1);
    ElementSpriteGroupItem* pSpriteBackRight = GetItem(2);
    ElementSpriteGroupItem* pSpriteLeft = GetItem(3);
    ElementSpriteGroupItem* pSpriteMid = GetItem(4);
    ElementSpriteGroupItem* pSpriteRight = GetItem(5);

    pSpriteBackLeft->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    pSpriteBackLeft->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT);
    pSpriteBackLeft->SetUnifiedSize(UDim2(0.f, fBarBorderSizeX, 1.f, 0.f));

    pSpriteBackMid->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    pSpriteBackMid->SetUnifiedPosition(UDim2(0.f, fBarBorderSizeX, 0.f, 0.f));
    pSpriteBackMid->SetUnifiedSize(UDim2(1.f, -2.f * fBarBorderSizeX, 1.f, 0.f));

    pSpriteBackRight->SetUnifiedOrigin(UDim2::POSITION_TOP_RIGHT);
    pSpriteBackRight->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT);
    pSpriteBackRight->SetUnifiedSize(UDim2(0.f, fBarBorderSizeX, 1.f, 0.f));

    pSpriteLeft->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    pSpriteLeft->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT);
    pSpriteLeft->SetUnifiedSize(UDim2(0.f, fBarBorderSizeX, 1.f, 0.f));

    pSpriteMid->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    pSpriteMid->SetUnifiedPosition(UDim2(0.f, fBarBorderSizeX, 0.f, 0.f));
    pSpriteMid->SetUnifiedSize(UDim2(1.f, -2.f * fBarBorderSizeX, 1.f, 0.f));

    pSpriteRight->SetUnifiedOrigin(UDim2::POSITION_TOP_RIGHT);
    pSpriteRight->SetUnifiedPosition(UDim2::POSITION_TOP_RIGHT);
    pSpriteRight->SetUnifiedSize(UDim2(0.f, fBarBorderSizeX, 1.f, 0.f));
}

void ElementBar::SetValue(float _fCurrent, float _fMax)
{
    ElementSpriteGroupItem* pSpriteLeft = GetItem(3);
    ElementSpriteGroupItem* pSpriteMid = GetItem(4);
    ElementSpriteGroupItem* pSpriteRight = GetItem(5);

    float fValue = 1.f;
    if (_fMax > 0.f)
    {
        fValue = Clamp(_fCurrent, 0.f, _fMax) / _fMax;
    }

    if (fValue != m_lastValue)
    {
        m_lastValue = fValue;

        float fSizeX = (GetSize().x - (2.f * pSpriteLeft->GetSize().x));
        fSizeX *= fValue;
        fSizeX = Max(0.f, fSizeX);
        pSpriteMid->SetUnifiedSize(UDim2(0.f, fSizeX, 1.f, 0.f));

        bool leftVisible = fValue - gugu::Math::Epsilon >= 0.f;
        bool rightVisible = fValue + gugu::Math::Epsilon >= 1.f;

        // If we want to hide/show some items, we need a full recompute.
        if (leftVisible != pSpriteLeft->IsVisible(false) || rightVisible != pSpriteRight->IsVisible(false))
        {
            m_needRecompute = true;

            pSpriteLeft->SetVisible(leftVisible);
            pSpriteRight->SetVisible(rightVisible);

            RecomputeItemVertices(3);
            RecomputeItemVertices(5);
        }

        // Only recompute the center item vertices when we can.
        RecomputeItemVertices(4);
    }
}

}   //namespace demoproject
