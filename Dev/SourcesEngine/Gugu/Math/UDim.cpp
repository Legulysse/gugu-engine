////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Misc/UDim.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

UDim::UDim()
{
    relative = 0.f;
    absolute = 0.f;
}

UDim::UDim(const UDim& _kRight)
{
    relative = _kRight.relative;
    absolute = _kRight.absolute;
}

UDim::UDim(float _fRel, float _fAbs)
{
    relative = _fRel;
    absolute = _fAbs;
}

UDim& UDim::operator+=(float _fAbs)
{
    absolute += _fAbs;
    return *this;
}

const UDim UDim::operator+(float _fAbs) const
{
    return UDim(*this) += _fAbs;
}

float UDim::GetComputedDimension(float _fReference) const
{
    return relative * _fReference + absolute;
}

UDim2::UDim2()
{
}

UDim2::UDim2(const UDim2& _kRight)
{
    x = _kRight.x;
    y = _kRight.y;
}

UDim2::UDim2(UDim _X, UDim _Y)
{
    x = _X;
    y = _Y;
}

UDim2::UDim2(float _fRelX, float _fRelY)
{
    x = UDim(_fRelX, 0.f);
    y = UDim(_fRelY, 0.f);
}

UDim2::UDim2(float _fRelX, float _fAbsX, float _fRelY, float _fAbsY)
{
    x = UDim(_fRelX, _fAbsX);
    y = UDim(_fRelY, _fAbsY);
}

UDim2& UDim2::operator+=(const sf::Vector2f& _kAbs)
{
    x.absolute += _kAbs.x;
    y.absolute += _kAbs.y;
    return *this;
}

const UDim2 UDim2::operator+(const sf::Vector2f& _kAbs) const
{
    return UDim2(*this) += _kAbs;
}

sf::Vector2f UDim2::GetComputedDimension(float _fReferenceX, float _fReferenceY) const
{
    return GetComputedDimension(sf::Vector2f(_fReferenceX, _fReferenceY));
}

sf::Vector2f UDim2::GetComputedDimension(sf::Vector2f _kReferenceSize) const
{
    return sf::Vector2f(x.relative * _kReferenceSize.x + x.absolute, y.relative * _kReferenceSize.y + y.absolute);
}

const UDim UDim::ZERO = UDim(0.f, 0.f);
const UDim UDim::HALF = UDim(0.5f, 0.f);
const UDim UDim::FULL = UDim(1.f, 0.f);

const UDim2 UDim2::POSITION_TOP_LEFT = UDim2(0.f  , 0.f , 0.f  , 0.f);
const UDim2 UDim2::POSITION_TOP_CENTER = UDim2(0.5f , 0.f , 0.f  , 0.f);
const UDim2 UDim2::POSITION_TOP_RIGHT = UDim2(1.f, 0.f, 0.f, 0.f);
const UDim2 UDim2::POSITION_CENTER_LEFT = UDim2(0.f, 0.f, 0.5f, 0.f);
const UDim2 UDim2::POSITION_CENTER = UDim2(0.5f, 0.f, 0.5f, 0.f);
const UDim2 UDim2::POSITION_CENTER_RIGHT = UDim2(1.f, 0.f, 0.5f, 0.f);
const UDim2 UDim2::POSITION_BOTTOM_LEFT = UDim2(0.f, 0.f, 1.f, 0.f);
const UDim2 UDim2::POSITION_BOTTOM_CENTER = UDim2(0.5f, 0.f, 1.f, 0.f);
const UDim2 UDim2::POSITION_BOTTOM_RIGHT = UDim2(1.f, 0.f, 1.f, 0.f);

const UDim2 UDim2::SIZE_ZERO = UDim2(0.f, 0.f, 0.f, 0.f);
const UDim2 UDim2::SIZE_SQUARE_QUARTER = UDim2(0.5f, 0.f, 0.5f, 0.f);
const UDim2 UDim2::SIZE_HORIZONTAL_HALF = UDim2(1.f, 0.f, 0.5f, 0.f);
const UDim2 UDim2::SIZE_VERTICAL_HALF = UDim2(0.5f, 0.f, 1.f, 0.f);
const UDim2 UDim2::SIZE_FULL = UDim2(1.f, 0.f, 1.f, 0.f);

}   // namespace gugu
