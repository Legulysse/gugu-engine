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

}   // namespace gugu
