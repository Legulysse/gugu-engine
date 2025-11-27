////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Math/Random.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Container.h"
#include "Gugu/Math/MathUtility.h"

#include <random>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

namespace impl
{
	static std::mt19937 generator;
}

void ResetRandSeed()
{
	std::random_device rd;
	impl::generator.seed(rd());
}

float GetRandf()
{
#if 1
	// There is a bug with real distribution in some implementations that may occasionally provide the max value, instead of keeping a [min, max[ range.
	// In this case, we dont care, its a desirable result.
	static const std::uniform_real_distribution<float> distribution(0.f, 1.f);
	return distribution(impl::generator);

	// Note: On platforms without the bug, if we really want [0, 1], this alternative call could be used :
	// std::uniform_real_distribution<float> dis(0.f, std::nextafter(1.f, std::numeric_limits<RealType>::max()))
#else
	// This version could be used for a proper [0, 1[ range on platforms where the rounding bug occurs.
	static const std::uniform_real_distribution<float> distribution(0.f, 1.f);
	float result = distribution(impl::generator);
	if (result == 1.f)
	{
		// There is a bug with real distribution in some implementations that may occasionally provide the max value, instead of keeping a [min, max[ range.
		result = std::nextafter(result, -std::numeric_limits<float>::infinity());
	}
	return result;
#endif
}

size_t GetRandom(size_t size)
{
	// Note: If max value is zero, we will end up with a random between zero and the max size_t value (size_t(-1)).
	// - Returning a zero here if called for an empty container is probably not something we want.
	// - Could be replaced by an early return with InvalidIndex ?
	//   - But it would mean that the return value should always be checked instead of being meant as a one-liner.
	assert(size > 0);

	std::uniform_int_distribution<size_t> distribution(0, size - 1);
	return distribution(impl::generator);
}

float GetRandomf(float max)
{
	// uniform_real_distribution with max < min is undefined behaviour.
	assert(max >= 0.f);

	// There is a bug with real distribution in some implementations that may occasionally provide the max value, instead of keeping a [min, max[ range.
	// In this case, we dont care, its a desirable result.
	std::uniform_real_distribution<float> distribution(0.f, max);
	return distribution(impl::generator);
}

int GetRandom(int min, int max)
{
	// uniform_int_distribution with max < min is undefined behaviour.
	assert(min <= max);

	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(impl::generator);
}

float GetRandomf(float min, float max)
{
	// uniform_real_distribution with max < min is undefined behaviour.
	assert(min <= max);

	// There is a bug with real distribution in some implementations that may occasionally provide the max value, instead of keeping a [min, max[ range.
	// In this case, we dont care, its a desirable result.
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(impl::generator);
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

Vector2f GetRandomPointOnCircleEdge(const Vector2f& center, float radius)
{
	float theta = GetRandf() * 2.f * math::Pi;

	return Vector2f(center.x + radius * std::cos(theta),
		center.y + radius * std::sin(theta));
}

size_t GetWeightedRandomIndex(const std::vector<int>& weights)
{
	if (weights.size() == 0)
		return system::InvalidIndex;

	int totalWeight = 0;
	for (size_t i = 0; i < weights.size(); ++i)
	{
		if (weights[i] > 0)
		{
			totalWeight += weights[i];
		}
	}

	if (totalWeight == 0)
		return system::InvalidIndex;

	int rollWeight = GetRandom(1, totalWeight);
	for (size_t i = 0; i < weights.size(); ++i)
	{
		if (weights[i] > 0)
		{
			if (rollWeight <= weights[i])
			{
				return i;
			}

			rollWeight -= weights[i];
		}
	}

	return system::InvalidIndex;
}

}   // namespace gugu
