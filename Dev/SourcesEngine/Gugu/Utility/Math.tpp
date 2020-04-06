#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

//--------------------------------
// General

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
// Vectors

template <typename T>
float LengthSquare(const sf::Vector2<T>& _kVector)
{
    return (float)(_kVector.x * _kVector.x + _kVector.y * _kVector.y);
}

template <typename T>
float Length(const sf::Vector2<T>& _kVector)
{
    return sqrt(LengthSquare(_kVector));
}

template <typename T>
sf::Vector2<T> Normalize(const sf::Vector2<T>& _kVector)
{
    if (_kVector != sf::Vector2<T>(0,0))
        return _kVector / (Length(_kVector));
    return sf::Vector2<T>(0,0);
}

template <typename T>
sf::Vector2<T> Rotate(const sf::Vector2<T>& _kVector, float _fRadians)
{
    sf::Vector2<T> kResult;
    float fCos = std::cos(_fRadians);
    float fSin = std::sin(_fRadians);
    kResult.x = _kVector.x * fCos - _kVector.y * fSin; 
    kResult.y = _kVector.x * fSin + _kVector.y * fCos;
    return kResult;
}

//--------------------------------
// Interpolations

template <typename T>
T Lerp(const T& min, const T& max, float ratio)
{
    return min * (1.f - ratio) + max * ratio;
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
