#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Vertex.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace sf
{
	class VertexArray;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

using Vertex2 = sf::Vertex;

Vertex2 BuildVertex(const Vector2f& position);
Vertex2 BuildVertex(const Vector2f& position, const sf::Color& color);
Vertex2 BuildVertex(const Vector2f& position, const sf::Color& color, const Vector2f& texCoords);

// Append 2 vertices to a given array, forming a line.
void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b);
void AppendLineVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const sf::Color& color);

// Append 3 vertices to a given array, forming a triangle.
void AppendTriangleVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c);

// Append 6 vertices to a given array, forming a quad (two triangles).
void AppendQuadVertices(sf::VertexArray& array);

// Append 6 vertices to a given array, forming a quad (abcd positions should form a "Z" shape).
void AppendQuadVertices(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d);

// Update vertices of the given array/quad (abcd positions should form a "Z" shape).
void UpdateQuadVertexPositions(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d);
void UpdateQuadVertexTexCoords(sf::VertexArray& array, const Vector2f& a, const Vector2f& b, const Vector2f& c, const Vector2f& d);

}   // namespace gugu
