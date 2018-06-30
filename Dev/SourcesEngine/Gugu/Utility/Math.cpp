////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Utility/Math.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

double ToRadians(double _fDegrees)
{
    return _fDegrees * Math::DegreesToRadians;
}

double ToDegrees(double _fRadians)
{
    return _fRadians * Math::RadiansToDegrees;
}

float ToRadiansf(float _fDegrees)
{
    return _fDegrees * Math::DegreesToRadians;
}

float ToDegreesf(float _fRadians)
{
    return _fRadians * Math::RadiansToDegrees;
}

}   // namespace gugu
