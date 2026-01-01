#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

//--------------------------------
// General

template <typename T>
bool IsInBounds(T value, T min, T max)
{
    return (value >= min && value <= max);
}

template <typename T>
bool ApproxIsInBounds(T value, T min, T max, float epsilon)
{
    return ApproxSuperiorOrEqual(value, min, epsilon) && ApproxInferiorOrEqual(value, max, epsilon);
}

template <typename T>
bool IsInBoundsUnordered(T value, T limitA, T limitB)
{
    return (value >= limitA && value <= limitB) || (value >= limitB && value <= limitA);
}

template <typename T>
bool ApproxIsInBoundsUnordered(T value, T limitA, T limitB, float epsilon)
{
    return (ApproxSuperiorOrEqual(value, limitA, epsilon) && ApproxInferiorOrEqual(value, limitB, epsilon))
        || (ApproxSuperiorOrEqual(value, limitB, epsilon) && ApproxInferiorOrEqual(value, limitA, epsilon));
}

template <typename T>
T Max(T _tValue, T _tMax)
{
    return std::max(_tValue, _tMax);
}

template <typename T>
T Min(T _tValue, T _tMin)
{
    return std::min(_tValue, _tMin);
}

template <typename T>
T Clamp(T _tValue, T _tMin, T _tMax)
{
    return std::min(_tMax, std::max(_tMin, _tValue));
}

template <typename T>
T ClampUnordered(T _tValue, T _tLimitA, T _tLimitB)
{
    if (_tLimitA < _tLimitB)
        return std::min(_tLimitB, std::max(_tLimitA, _tValue));
    else
        return std::min(_tLimitA, std::max(_tLimitB, _tValue));
}

template <typename T>
T Clamp01(T _tValue)
{
    return std::min((T)1, std::max((T)0, _tValue));
}

template <typename T>
T Distance(T valueA, T valueB)
{
    return std::abs(valueA - valueB);
}

template <typename T>
T Absolute(T _tValue)
{
    // Note: we need implementations from both cstdlib and cmath.
    return std::abs(_tValue);
}

template <typename T>
T Sign(T value)
{
    if (value > (T)0) return (T)1;
    if (value < (T)0) return (T)-1;
    return (T)0;
}

template <typename T>
int SignInt(T value)
{
    if (value > (T)0) return 1;
    if (value < (T)0) return -1;
    return 0;
}

template <typename T>
T Power(T n, int p)
{
    return (T)std::pow(n, p);
}

template <typename T>
T Power2(T n)
{
    return n * n;
}

template <typename T>
T TruncatedModulo(T value, T modulus)
{
    // Truncated : result has the same sign as the value.
    // - https://en.wikipedia.org/wiki/Modulo
    return value % modulus;
}

template <typename T>
T FlooredModulo(T value, T modulus)
{
    // Floored : result has the same sign as the modulus.
    // - https://en.wikipedia.org/wiki/Modulo
    return (value % modulus + modulus) % modulus;
}

template <typename T>
T EuclideanModulo(T value, T modulus)
{
    // Euclidean : result is always positive.
    // - https://en.wikipedia.org/wiki/Modulo
    T temp = value % modulus;
    return temp >= 0 ? temp : (modulus >= 0 ? temp + modulus : temp - modulus);
}

//--------------------------------
// Rounding

template <typename T>
int RoundNearestInt(const T& n)
{
    return std::lround(n);
}

template <typename T>
T RoundNearest(const T& n)
{
    return std::round(n);
}

template <typename T>
int RoundFloorInt(const T& n)
{
    return std::lround(std::floor(n));
}

template <typename T>
T RoundFloor(const T& n)
{
    return std::floor(n);
}

template <typename T>
int RoundCeilInt(const T& n)
{
    return std::lround(std::ceil(n));
}

template <typename T>
T RoundCeil(const T& n)
{
    return std::ceil(n);
}

template <typename T>
int RoundTowardZeroInt(const T& n)
{
    return std::lround(std::trunc(n));
}

template <typename T>
T RoundTowardZero(const T& n)
{
    return std::trunc(n);
}

template <typename T>
int RoundAwayFromZeroInt(const T& n)
{
    return std::lround((n >= (T)0) ? std::ceil(n) : std::floor(n));
}

template <typename T>
T RoundAwayFromZero(const T& n)
{
    return (n >= (T)0) ? std::ceil(n) : std::floor(n);
}

//--------------------------------
// Interpolations

template <typename T>
T Lerp(const T& min, const T& max, float ratio)
{
    return (T)(min * (1.f - ratio) + max * ratio);
}

template <typename T>
float InverseLerp(const T& min, const T& max, const T& value)
{
    return (float)(value - min) / (float)(max - min);
}

template <typename TFrom, typename TTo>
TTo RemapLerp(const TFrom& minA, const TFrom& maxA, const TTo& minB, const TTo& maxB, const TFrom& value)
{
    return Lerp(minB, maxB, InverseLerp(minA, maxA, value));
}

}   // namespace gugu
