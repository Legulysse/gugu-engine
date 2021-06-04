#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

#include <set>  // for std::less specialization

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

typedef sf::Vector2i Vector2i;
typedef sf::Vector2u Vector2u;
typedef sf::Vector2f Vector2f;

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
