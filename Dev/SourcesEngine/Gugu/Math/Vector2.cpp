////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

std::string ToString(const Vector2f& value)
{
    std::ostringstream os;
    os << "(" << ToString(value.x) << ", " << ToString(value.y) << ")";
    return os.str();
}

bool ApproxEqual(const Vector2f& left, const Vector2f& right, float epsilon)
{
    return gugu::ApproxEqual(left.x, right.x, epsilon) && gugu::ApproxEqual(left.y, right.y, epsilon);
}

}   // namespace gugu
