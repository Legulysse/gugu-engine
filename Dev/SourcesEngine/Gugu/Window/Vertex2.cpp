////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Vertex2.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

sf::Vertex BuildVertex2(const Vector2f& position)
{
	return sf::Vertex{ position };
}

sf::Vertex BuildVertex2(const Vector2f& position, const sf::Color& color)
{
	return sf::Vertex{ position, color };
}

sf::Vertex BuildVertex2(const Vector2f& position, const sf::Color& color, const Vector2f& texCoords)
{
	return sf::Vertex{ position, color, texCoords };
}

}   // namespace gugu
