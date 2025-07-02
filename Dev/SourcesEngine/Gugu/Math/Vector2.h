#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <set>  // for std::less specialization
#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename T>
using Vector2_t = sf::Vector2<T>;
using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
using Vector2f = sf::Vector2f;

namespace Vector2
{
    const Vector2i Zero_i = Vector2i();
    const Vector2u Zero_u = Vector2u();
    const Vector2f Zero_f = Vector2f();
}

std::string ToString(const Vector2f& value);
std::string ToString(const Vector2i& value);
std::string ToString(const Vector2u& value);

bool ApproxEqual(const Vector2f& left, const Vector2f& right, float epsilon);

template <typename T>
float LengthSquare(const Vector2_t<T>& vector);

template <typename T>
float Length(const Vector2_t<T>& vector);

template <typename T>
Vector2_t<T> Normalize(const Vector2_t<T>& _kVector);

template <typename T>
Vector2_t<T> Rotate(const Vector2_t<T>& _kVector, float _fRadians);

Vector2f VectorFromAngle(float radians, float length);  // Zero angle is the +x axis, Pi/2 angle is the +y axis.

template <typename T>
bool IsInBounds(const Vector2_t<T>& value, const Vector2_t<T>& min, const Vector2_t<T>& max);

bool ApproxIsInBounds(const Vector2f& value, const Vector2f& min, const Vector2f& max, float epsilon);

template <typename T>
bool DistanceCheck(const Vector2_t<T>& left, const Vector2_t<T>& right, float distance);

bool ApproxDistanceCheck(const Vector2f& left, const Vector2f& right, float distance, float epsilon);

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
float LengthSquare(const Vector2_t<T>& vector)
{
    return (float)(vector.x * vector.x + vector.y * vector.y);
}

template <typename T>
float Length(const Vector2_t<T>& vector)
{
    return std::sqrt(LengthSquare(vector));
}

template <typename T>
Vector2_t<T> Normalize(const Vector2_t<T>& _kVector)
{
    if (_kVector != Vector2_t<T>(0, 0))
        return _kVector / (Length(_kVector));
    return Vector2_t<T>(0, 0);
}

template <typename T>
Vector2_t<T> Rotate(const Vector2_t<T>& _kVector, float _fRadians)
{
    Vector2_t<T> kResult;
    float fCos = std::cos(_fRadians);
    float fSin = std::sin(_fRadians);
    kResult.x = _kVector.x * fCos - _kVector.y * fSin;
    kResult.y = _kVector.x * fSin + _kVector.y * fCos;
    return kResult;
}

template <typename T>
bool IsInBounds(const Vector2_t<T>& value, const Vector2_t<T>& min, const Vector2_t<T>& max)
{
    return (value.x >= min.x && value.x <= max.x) && (value.y >= min.y && value.y <= max.y);
}

template <typename T>
bool DistanceCheck(const Vector2_t<T>& left, const Vector2_t<T>& right, float distance)
{
    if (std::abs(left.x - right.x) <= distance && std::abs(left.y - right.y) <= distance)
    {
        return LengthSquare(right - left) <= distance * distance;
    }

    return false;
}

}   // namespace gugu
