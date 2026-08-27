////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Audio/AudioUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

float ComputeVolumeCurve(float volume)
{
    // - Use an approximation on the volume curve to try and match the logarithmic natural earing.
    // - Arbitrary x^2 curve to have a curve that is not too hard on edge values (x^3 and x^4 are very sharp).
    // - Arbitrary max value of 2 to limit dramatic upper volumes (matches an in-volume of 1.225).
    // - Arbitrary linear smooth on the lowest edge to keep some value ranges before reaching zero.
    return Clamp(Max(volume * 0.25f, Power2(volume)), 0.f, 2.f);
}

}   // namespace gugu
