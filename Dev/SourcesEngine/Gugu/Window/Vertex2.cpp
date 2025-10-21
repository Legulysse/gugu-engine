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

sf::Vertex BuildVertex(const Vector2f& position)
{
	return sf::Vertex{ position };
}

sf::Vertex BuildVertex(const Vector2f& position, const sf::Color& color)
{
	return sf::Vertex{ position, color };
}

sf::Vertex BuildVertex(const Vector2f& position, const sf::Color& color, const Vector2f& texCoords)
{
	return sf::Vertex{ position, color, texCoords };
}

void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b)
{
	array.append(BuildVertex(a));
	array.append(BuildVertex(b));
}

void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const sf::Color& color)
{
	array.append(BuildVertex(a, color));
	array.append(BuildVertex(b, color));
}

void AppendTriangleVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c)
{
	array.append(BuildVertex(a));
	array.append(BuildVertex(b));
	array.append(BuildVertex(c));
}

void AppendQuadVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d)
{
	array.append(BuildVertex(a));
	array.append(BuildVertex(b));
	array.append(BuildVertex(c));

	array.append(BuildVertex(c));
	array.append(BuildVertex(b));
	array.append(BuildVertex(d));
}

}   // namespace gugu
