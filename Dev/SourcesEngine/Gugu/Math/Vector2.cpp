////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool ApproxEqual(const Vector2f& left, const Vector2f& right, float epsilon)
{
    return gugu::ApproxEqual(left.x, right.x, epsilon) && gugu::ApproxEqual(left.y, right.y, epsilon);
}

}   // namespace gugu
