#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

//--------------------------------
// General

template <typename T>
bool NearlyEquals(T left, T right, T epsilon)
{
    return std::abs(left - right) <= epsilon;
}

template <typename T>
bool IsInRange(T _tValue, T _tMin, T _tMax)
{
    return (_tValue >= _tMin && _tValue <= _tMax);
}

template <typename T>
bool IsInRangeUnordered(T _tValue, T _tLimitA, T _tLimitB)
{
    return (_tValue >= _tLimitA && _tValue <= _tLimitB) || (_tValue >= _tLimitB && _tValue <= _tLimitA);
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
T Distance(T _tValue1, T _tValue2)
{
    return std::max(_tValue1, _tValue2) - std::min(_tValue1, _tValue2);
}

template <typename T>
T Absolute(T _tValue)    //Visual has a good abs() templated for int, float, etc... but CodeBlocks don't...
{
    return (_tValue >= (T)0) ? _tValue : (_tValue * (T)-1);
}

template <typename T>
T Power(const T& n, const int& p)
{
    if (p >= 0)
    {
        return (T)std::pow((double)n, p);
    }
    else
    {
        return (T)(1.f / std::pow((double)n, -p));
    }
}

//--------------------------------
// Rounding

template <typename T>
T RoundNearest(const T& n)
{
    return std::round(n);
}

template <typename T>
T RoundFloor(const T& n)
{
    return std::floor(n);
}

template <typename T>
T RoundCeil(const T& n)
{
    return std::ceil(n);
}

template <typename T>
T RoundTowardZero(const T& n)
{
    return std::trunc(n);
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

template <typename T>
T RemapLerp(const T& minA, const T& maxA, const T& minB, const T& maxB, const T& value)
{
    return Lerp(minB, maxB, InverseLerp(minA, maxA, value));
}

}   // namespace gugu
