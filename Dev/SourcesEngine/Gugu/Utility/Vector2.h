#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

typedef sf::Vector2i Vector2i;
typedef sf::Vector2u Vector2u;
typedef sf::Vector2f Vector2f;

}   // namespace gugu

namespace sf {

template <typename T>
bool operator<(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
    return left.y < right.y || (left.y == right.y && left.x < right.x);
}

}   // namespace sf
