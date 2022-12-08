#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <set>  // for std::less specialization

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
using Vector2f = sf::Vector2f;

namespace Vector2
{
    const Vector2i Zero_i = Vector2i();
    const Vector2u Zero_u = Vector2u();
    const Vector2f Zero_f = Vector2f();
}

template <typename T>
float LengthSquare(const sf::Vector2<T>& _kVector);

template <typename T>
float Length(const sf::Vector2<T>& _kVector);

template <typename T>
sf::Vector2<T> Normalize(const sf::Vector2<T>& _kVector);

template <typename T>
sf::Vector2<T> Rotate(const sf::Vector2<T>& _kVector, float _fRadians);

}   // namespace gugu

//----------------------------------------------
// Comparators for containers

namespace std {

template<>
struct less<gugu::Vector2i>
{
    constexpr bool operator()(const gugu::Vector2i& left, const gugu::Vector2i& right) const noexcept
    {
        return left.y < right.y || (left.y == right.y && left.x < right.x);
    }
};

template<>
struct less<gugu::Vector2u>
{
    constexpr bool operator()(const gugu::Vector2u& left, const gugu::Vector2u& right) const noexcept
    {
        return left.y < right.y || (left.y == right.y && left.x < right.x);
    }
};

}   // namespace std

////////////////////////////////////////////////////////////////
// Template Implementation

namespace gugu {

template <typename T>
float LengthSquare(const sf::Vector2<T>& _kVector)
{
    return (float)(_kVector.x * _kVector.x + _kVector.y * _kVector.y);
}

template <typename T>
float Length(const sf::Vector2<T>& _kVector)
{
    return std::sqrt(LengthSquare(_kVector));
}

template <typename T>
sf::Vector2<T> Normalize(const sf::Vector2<T>& _kVector)
{
    if (_kVector != sf::Vector2<T>(0, 0))
        return _kVector / (Length(_kVector));
    return sf::Vector2<T>(0, 0);
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

}   // namespace gugu
