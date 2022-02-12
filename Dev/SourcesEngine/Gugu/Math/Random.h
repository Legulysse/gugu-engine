#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Reset the seed used by GetRand().
void ResetRandSeed();

// Get a random value in range [0, 1[.    // TODO: Thread safety ?
float GetRandf();

// Get a random int value in range [0, max[.
int GetRandom(int _iMax);

// Get a random float value in range [0, max[.
float GetRandomf(float _fMax);

// Get a random int value in range [min, max].
int GetRandom(int _iMin, int _iMax);

// Get a random float value in range [min, max[.
float GetRandomf(float _fMin, float _fMax);

Vector2f GetRandomPointInCircle(const Vector2f& center, float radius);
Vector2f GetRandomPointInAnnulus(const Vector2f& center, float minRadius, float maxRadius);

}   // namespace gugu
