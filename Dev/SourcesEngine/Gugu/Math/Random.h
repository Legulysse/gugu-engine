#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Reset the seed used by GetRand().
void ResetRandSeed();

// Get a random value in range [0, 1[.    // TODO: Thread safety ?
double GetRand();

// Get a random int value in range [0, max[.
int GetRandom(int _iMax);

// Get a random float value in range [0, max[.
float GetRandomf(float _fMax);

// Get a random int value in range [min, max].
int GetRandom(int _iMin, int _iMax);

// Get a random float value in range [min, max[.
float GetRandomf(float _fMin, float _fMax);

}   // namespace gugu
