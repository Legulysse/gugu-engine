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

void AppendQuadVertices(sf::VertexArray& array)
{
    array.append(BuildVertex(Vector2::Zero_f));
    array.append(BuildVertex(Vector2::Zero_f));
    array.append(BuildVertex(Vector2::Zero_f));
    array.append(BuildVertex(Vector2::Zero_f));
    array.append(BuildVertex(Vector2::Zero_f));
    array.append(BuildVertex(Vector2::Zero_f));
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

void UpdateQuadVertexPositions(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d)
{
    assert(array.getVertexCount() >= 6);
    array[0].position = a;
    array[1].position = b;
    array[2].position = c;
    array[3].position = c;
    array[4].position = b;
    array[5].position = d;
}

void UpdateQuadVertexTexCoords(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d)
{
    assert(array.getVertexCount() >= 6);
    array[0].texCoords = a;
    array[1].texCoords = b;
    array[2].texCoords = c;
    array[3].texCoords = c;
    array[4].texCoords = b;
    array[5].texCoords = d;
}

}   // namespace gugu
