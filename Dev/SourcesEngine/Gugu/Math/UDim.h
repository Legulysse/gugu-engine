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
    UDim(const UDim& right);
    UDim(float rel);
    UDim(float rel, float abs);

    UDim& operator+=(float abs);
    const UDim operator+(float abs) const;

    float GetComputedDimension(float reference) const;

    static const UDim ZERO;
    static const UDim HALF;
    static const UDim FULL;
};

class UDim2
{
public:

    Vector2f relative;
    Vector2f absolute;

    UDim2();
    UDim2(const UDim2& right);
    UDim2(float xRel, float yRel);
    UDim2(const Vector2f& rel);
    UDim2(const Vector2f& rel, const Vector2f& abs);
    
    bool operator==(const UDim2& right) const;

    UDim2& operator+=(const Vector2f& abs);
    const UDim2 operator+(const Vector2f& abs) const;

    Vector2f GetComputedDimension(float referenceX, float referenceY) const;
    Vector2f GetComputedDimension(const Vector2f& reference) const;

    // TODO: Remove this and properly handle pixel alignment in render pipeline
    Vector2f GetPixelAlignedComputedDimension(float referenceX, float referenceY) const;
    Vector2f GetPixelAlignedComputedDimension(const Vector2f& reference) const;

    static const UDim2 ZERO;

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

std::string ToString(UDim2 value);

}   // namespace gugu
