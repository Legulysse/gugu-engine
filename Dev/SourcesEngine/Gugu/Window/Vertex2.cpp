////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Vertex2.h"

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/Graphics/VertexArray.hpp>

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

void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b)
{
	array.append(BuildVertex2(a));
	array.append(BuildVertex2(b));
}

void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const sf::Color& color)
{
	array.append(BuildVertex2(a, color));
	array.append(BuildVertex2(b, color));
}

void AppendTriangleVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c)
{
	array.append(BuildVertex2(a));
	array.append(BuildVertex2(b));
	array.append(BuildVertex2(c));
}

void AppendQuadVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d)
{
	array.append(BuildVertex2(a));
	array.append(BuildVertex2(b));
	array.append(BuildVertex2(c));

	array.append(BuildVertex2(c));
	array.append(BuildVertex2(b));
	array.append(BuildVertex2(d));
}

}   // namespace gugu
