#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Vertex.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

using Vertex2 = sf::Vertex;

Vertex2 BuildVertex2(const Vector2f& position);
Vertex2 BuildVertex2(const Vector2f& position, const sf::Color& color);
Vertex2 BuildVertex2(const Vector2f& position, const sf::Color& color, const Vector2f& texCoords);

}   // namespace gugu
