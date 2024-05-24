////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Math/UDim.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/String.h"

#include <sstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

UDim::UDim()
    : relative(0.f)
    , absolute(0.f)
{
}

UDim::UDim(const UDim& right)
    : relative(right.relative)
    , absolute(right.absolute)
{
}

UDim::UDim(float rel)
    : relative(rel)
    , absolute(0.f)
{
}

UDim::UDim(float rel, float abs)
    : relative(rel)
    , absolute(abs)
{
}

UDim& UDim::operator+=(float abs)
{
    absolute += abs;
    return *this;
}

const UDim UDim::operator+(float abs) const
{
    return UDim(*this) += abs;
}

float UDim::GetComputedDimension(float reference) const
{
    return relative * reference + absolute;
}

UDim2::UDim2()
{
}

UDim2::UDim2(const UDim2& right)
    : relative(right.relative)
    , absolute(right.absolute)
{
}

UDim2::UDim2(float xRel, float yRel)
    : relative(xRel, yRel)
    , absolute(Vector2::Zero_f)
{
}

UDim2::UDim2(const Vector2f& rel)
    : relative(rel)
    , absolute(Vector2::Zero_f)
{
}

UDim2::UDim2(const Vector2f& rel, const Vector2f& abs)
    : relative(rel)
    , absolute(abs)
{
}

bool UDim2::operator==(const UDim2& right) const
{
    return relative == right.relative && absolute == right.absolute;
}

UDim2& UDim2::operator+=(const Vector2f& abs)
{
    absolute += abs;
    return *this;
}

const UDim2 UDim2::operator+(const Vector2f& abs) const
{
    return UDim2(*this) += abs;
}

Vector2f UDim2::GetComputedDimension(float referenceX, float referenceY) const
{
    return GetComputedDimension(Vector2f(referenceX, referenceY));
}

Vector2f UDim2::GetComputedDimension(const Vector2f& reference) const
{
    return Vector2f(relative.x * reference.x + absolute.x, relative.y * reference.y + absolute.y);
}

Vector2f UDim2::GetPixelAlignedComputedDimension(float referenceX, float referenceY) const
{
    return GetPixelAlignedComputedDimension(Vector2f(referenceX, referenceY));
}

Vector2f UDim2::GetPixelAlignedComputedDimension(const Vector2f& reference) const
{
    return Vector2f(RoundFloor(relative.x * reference.x + absolute.x), RoundFloor(relative.y * reference.y + absolute.y));
}

std::string ToString(UDim2 value)
{
    std::ostringstream os;
    os << "(rel=" << ToString(value.relative) << ", abs=" << ToString(value.absolute) << ")";
    return os.str();
}

const UDim UDim::ZERO = UDim(0.f);
const UDim UDim::HALF = UDim(0.5f);
const UDim UDim::FULL = UDim(1.f);

const UDim2 UDim2::ZERO = UDim2(0.f, 0.f);

const UDim2 UDim2::POSITION_TOP_LEFT = UDim2(0.f, 0.f);
const UDim2 UDim2::POSITION_TOP_CENTER = UDim2(0.5f, 0.f);
const UDim2 UDim2::POSITION_TOP_RIGHT = UDim2(1.f, 0.f);
const UDim2 UDim2::POSITION_CENTER_LEFT = UDim2(0.f, 0.5f);
const UDim2 UDim2::POSITION_CENTER = UDim2(0.5f, 0.5f);
const UDim2 UDim2::POSITION_CENTER_RIGHT = UDim2(1.f, 0.5f);
const UDim2 UDim2::POSITION_BOTTOM_LEFT = UDim2(0.f, 1.f);
const UDim2 UDim2::POSITION_BOTTOM_CENTER = UDim2(0.5f, 1.f);
const UDim2 UDim2::POSITION_BOTTOM_RIGHT = UDim2(1.f, 1.f);

const UDim2 UDim2::SIZE_ZERO = UDim2(0.f, 0.f);
const UDim2 UDim2::SIZE_SQUARE_QUARTER = UDim2(0.5f, 0.5f);
const UDim2 UDim2::SIZE_HORIZONTAL_HALF = UDim2(1.f, 0.5f);
const UDim2 UDim2::SIZE_VERTICAL_HALF = UDim2(0.5f, 1.f);
const UDim2 UDim2::SIZE_FULL = UDim2(1.f, 1.f);

}   // namespace gugu
