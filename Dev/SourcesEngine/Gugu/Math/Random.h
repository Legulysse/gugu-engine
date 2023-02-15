#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Reset the seed used by GetRand().
void ResetRandSeed();

// Return a random value in range [0, 1[.    // TODO: Thread safety ?
float GetRandf();

// Return a random size_t value in range [0, max[.
size_t GetRandom(size_t _iMax);

// Return a random float value in range [0, max[.
float GetRandomf(float _fMax);

// Return a random int value in range [min, max].
int GetRandom(int _iMin, int _iMax);

// Return a random float value in range [min, max[.
float GetRandomf(float _fMin, float _fMax);

// Return a random point inside a circle, defined by its center and radius.
Vector2f GetRandomPointInCircle(const Vector2f& center, float radius);

// Return a random point inside an annulus, defined by its center, inner radius and outer radius.
Vector2f GetRandomPointInAnnulus(const Vector2f& center, float minRadius, float maxRadius);

// Return a weighted random index from a list of weights.
// Weights <= 0 will be ignored by the roll.
// Return -1 if the list does not contain any weight > 0.
int GetWeightedRandomIndex(const std::vector<int>& weights);

// Return a weighted random index from a list of items.
// Weights <= 0 will be ignored by the roll.
// Return -1 if the list does not contain any weight > 0.
// Predicate may look like this : auto predicate = [](const ItemType& item) -> int { return item.weight; };
template <typename T, typename P>
int GetWeightedRandomIndex(const std::vector<T>& items, P weightPredicate)
{
    std::vector<int> weights;
    weights.reserve(items.size());
    for (const T& item : items)
    {
        weights.push_back(weightPredicate(item));
    }

    return GetWeightedRandomIndex(weights);
}

}   // namespace gugu
