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

}   // namespace gugu


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
