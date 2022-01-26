#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/2D/ElementSpriteGroup.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class ElementBar : public gugu::ElementSpriteGroup
{
public:

    enum BarColor
    {
        Red,
        Blue,
        Green,
        Yellow,
    };

    ElementBar();
    virtual ~ElementBar();

    void InitBar(BarColor _eColor, float _fBarBorderSizeX);
    void SetValue(float _fCurrent, float _fMax);

protected:

    float m_lastValue;
};

}   //namespace demoproject
