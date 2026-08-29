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
    // - Arbitrary max in-value of 1.25 to limit dramatic upper volumes (matches an out-volume of approximately 1.5).
    // - Arbitrary linear smooth on the lowest edge to keep some value ranges before reaching zero.
    volume = Clamp(volume, 0.f, 1.25f);
    return Max(volume * 0.25f, Power2(volume));
}

}   // namespace gugu
