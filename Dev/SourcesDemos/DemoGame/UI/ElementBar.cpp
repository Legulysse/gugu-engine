////////////////////////////////////////////////////////////////
// Header

#include "UI/ElementBar.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/Math.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

ElementBar::ElementBar()
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

    Element* pSpriteLeft = GetItem(0);
    Element* pSpriteMid = GetItem(1);
    Element* pSpriteRight = GetItem(2);

    float fBarSizeX = GetSize().x;
    float fBarSizeY = GetSize().y;
    float fBarBorderSizeX = _fBarBorderSizeX;

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
    Element* pSpriteLeft = GetItem(0);
    Element* pSpriteMid = GetItem(1);

    if (!pSpriteLeft || !pSpriteMid)
        return;

    float fValue = 1.f;
    if (_fMax > 0.f)
        fValue = Clamp(_fCurrent, 0.f, _fMax) / _fMax;

    float fSizeX = (GetSize().x - (2.f * pSpriteLeft->GetSize().x));
    fSizeX *= fValue;
    fSizeX = Max(0.f, fSizeX);
    pSpriteMid->SetUnifiedSize(UDim2(0.f, fSizeX, 1.f, 0.f));

    RecomputeItemVertices(1);
}

}   //namespace demoproject
