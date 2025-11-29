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

    constexpr UDim() : relative(0.f), absolute(0.f) {}
    constexpr UDim(const UDim& right) : relative(right.relative), absolute(right.absolute) {}
    constexpr UDim(float rel) : relative(rel), absolute(0.f) {}
    constexpr UDim(float rel, float abs) : relative(rel), absolute(abs) {}

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

    constexpr UDim2() : relative(Vector2::Zero_f), absolute(Vector2::Zero_f) {}
    constexpr UDim2(const UDim2 & right) : relative(right.relative), absolute(right.absolute) {}
    constexpr UDim2(float xRel, float yRel) : relative(xRel, yRel), absolute(Vector2::Zero_f) {}
    constexpr UDim2(const Vector2f & rel) : relative(rel), absolute(Vector2::Zero_f) {}
    constexpr UDim2(const Vector2f & rel, const Vector2f & abs) : relative(rel), absolute(abs) {}

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

inline constexpr UDim UDim::ZERO = UDim(0.f);
inline constexpr UDim UDim::HALF = UDim(0.5f);
inline constexpr UDim UDim::FULL = UDim(1.f);

inline constexpr UDim2 UDim2::ZERO = UDim2(0.f, 0.f);

inline constexpr UDim2 UDim2::POSITION_TOP_LEFT = UDim2(0.f, 0.f);
inline constexpr UDim2 UDim2::POSITION_TOP_CENTER = UDim2(0.5f, 0.f);
inline constexpr UDim2 UDim2::POSITION_TOP_RIGHT = UDim2(1.f, 0.f);
inline constexpr UDim2 UDim2::POSITION_CENTER_LEFT = UDim2(0.f, 0.5f);
inline constexpr UDim2 UDim2::POSITION_CENTER = UDim2(0.5f, 0.5f);
inline constexpr UDim2 UDim2::POSITION_CENTER_RIGHT = UDim2(1.f, 0.5f);
inline constexpr UDim2 UDim2::POSITION_BOTTOM_LEFT = UDim2(0.f, 1.f);
inline constexpr UDim2 UDim2::POSITION_BOTTOM_CENTER = UDim2(0.5f, 1.f);
inline constexpr UDim2 UDim2::POSITION_BOTTOM_RIGHT = UDim2(1.f, 1.f);

inline constexpr UDim2 UDim2::SIZE_ZERO = UDim2(0.f, 0.f);
inline constexpr UDim2 UDim2::SIZE_SQUARE_QUARTER = UDim2(0.5f, 0.5f);
inline constexpr UDim2 UDim2::SIZE_HORIZONTAL_HALF = UDim2(1.f, 0.5f);
inline constexpr UDim2 UDim2::SIZE_VERTICAL_HALF = UDim2(0.5f, 1.f);
inline constexpr UDim2 UDim2::SIZE_FULL = UDim2(1.f, 1.f);

}   // namespace gugu
