////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Utility/Random.h"

////////////////////////////////////////////////////////////////
// Includes

#include <time.h>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
void ResetRandSeed()
{
    srand((unsigned int)time(nullptr));
}

double GetRand()
{
    return ((double)rand()) / ((double)RAND_MAX + 1);
}

int GetRandom(int _iMax)
{
    if (_iMax < 0)
        return 0;
    return (int)(GetRand() * _iMax);
}

float GetRandomf(float _fMax)
{
    if (_fMax < 0.f)
        return 0.f;
    return (float)(GetRand() * _fMax);
}

int GetRandom(int _iMin, int _iMax)
{
    return _iMin + GetRandom(_iMax - _iMin + 1);
}

float GetRandomf(float _fMin, float _fMax)
{
    return _fMin + GetRandomf(_fMax - _fMin);
}

}   // namespace gugu
