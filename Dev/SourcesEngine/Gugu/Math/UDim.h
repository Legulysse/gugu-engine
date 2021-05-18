#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class UDim
{
public:

    float relative;
    float absolute;

    UDim();
    UDim(const UDim& _kRight);
    UDim(float _fRel, float _fAbs);

    UDim& operator+=(float _fAbs);
    const UDim operator+(float _fAbs) const;

    float GetComputedDimension(float _fReference) const;

    static const UDim ZERO;
    static const UDim HALF;
    static const UDim FULL;
};

class UDim2
{
public:

    UDim x;
    UDim y;

    UDim2();
    UDim2(const UDim2& _kRight);
    UDim2(UDim _X, UDim _Y);
    UDim2(float _fRelX, float _fRelY);
    UDim2(float _fRelX, float _fAbsX, float _fRelY, float _fAbsY);

    UDim2& operator+=(const sf::Vector2f& _kAbs);
    const UDim2 operator+(const sf::Vector2f& _kAbs) const;

    sf::Vector2f GetComputedDimension(float _fReferenceX, float _fReferenceY) const;
    sf::Vector2f GetComputedDimension(sf::Vector2f _kReferenceSize) const;

    static const UDim2 POSITION_TOP_LEFT;
    static const UDim2 POSITION_TOP_CENTER;
    static const UDim2 POSITION_TOP_RIGHT;
    static const UDim2 POSITION_CENTER_LEFT;
    static const UDim2 POSITION_CENTER;
    static const UDim2 POSITION_CENTER_RIGHT;
    static const UDim2 POSITION_BOTTOM_LEFT;
    static const UDim2 POSITION_BOTTOM_CENTER;
    static const UDim2 POSITION_BOTTOM_RIGHT;

    static const UDim2 SIZE_ZERO;
    static const UDim2 SIZE_SQUARE_QUARTER;
    static const UDim2 SIZE_HORIZONTAL_HALF;
    static const UDim2 SIZE_VERTICAL_HALF;
    static const UDim2 SIZE_FULL;
};

}   // namespace gugu
