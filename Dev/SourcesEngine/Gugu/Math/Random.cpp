////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Math/Random.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

#include <time.h>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
void ResetRandSeed()
{
    srand((unsigned int)time(nullptr));
}

float GetRandf()
{
    return ((float)rand()) / ((float)RAND_MAX + 1);
}

size_t GetRandom(size_t _iMax)
{
	return (size_t)(GetRandf() * _iMax);
}

float GetRandomf(float _fMax)
{
    return (GetRandf() * _fMax);
}

int GetRandom(int _iMin, int _iMax)
{
    return _iMin + (int)(GetRandf() * (_iMax - _iMin + 1));
}

float GetRandomf(float _fMin, float _fMax)
{
    return _fMin + (GetRandf() * (_fMax - _fMin));
}

Vector2f GetRandomPointInCircle(const Vector2f& center, float radius)
{
    // Inspired by : https://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
    float r = radius * std::sqrtf(GetRandf());
    float theta = GetRandf() * 2.f * math::Pi;

    return Vector2f(center.x + r * std::cos(theta),
                    center.y + r * std::sin(theta));
}

Vector2f GetRandomPointInAnnulus(const Vector2f& center, float minRadius, float maxRadius)
{
    // Inspired by : https://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
    float r = std::sqrtf(GetRandf() * (maxRadius * maxRadius - minRadius * minRadius) + minRadius * minRadius);
    float theta = GetRandf() * 2.f * math::Pi;

    return Vector2f(center.x + r * std::cos(theta),
                    center.y + r * std::sin(theta));
}

int GetWeightedRandomIndex(const std::vector<int>& weights)
{
	if (weights.size() == 0)
		return -1;

	int totalWeight = 0;
	for (size_t i = 0; i < weights.size(); ++i)
	{
		if (weights[i] > 0)
		{
			totalWeight += weights[i];
		}
	}

	if (totalWeight == 0)
		return -1;

	int rollWeight = GetRandom(1, totalWeight);
	for (size_t i = 0; i < weights.size(); ++i)
	{
		if (weights[i] > 0)
		{
			if (rollWeight <= weights[i])
			{
				return (int)i;
			}

			rollWeight -= weights[i];
		}
	}

	return -1;
}

}   // namespace gugu
