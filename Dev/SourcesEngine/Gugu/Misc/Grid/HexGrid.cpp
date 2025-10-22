////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Misc/Grid/HexGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/MathUtility.h"

#include <array>
#include <assert.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

HexGrid::HexGrid()
    : m_width(0)
    , m_height(0)
    , m_hexagonApothem(0.f)
    , m_hexagonRadius(0.f)
    , m_cellWidth(0.f)
    , m_cellHeight(0.f)
{
}

HexGrid::~HexGrid()
{
}

void HexGrid::InitHexGrid(int _iWidth, int _iHeight, float _hexagonWidth)
{
    m_width = _iWidth;
    m_height = _iHeight;

    m_hexagonApothem = _hexagonWidth * 0.5f;
    m_hexagonRadius = m_hexagonApothem / (std::sqrtf(3) / 2.f);

    m_cellWidth = m_hexagonApothem * 2.f;
    m_cellHeight = m_hexagonRadius * 2.f;
}

void HexGrid::InitHexGrid(int _iWidth, int _iHeight, float _hexagonWidth, float _hexagonHeight)
{
	m_width = _iWidth;
	m_height = _iHeight;

	m_hexagonApothem = _hexagonWidth * 0.5f;
	m_hexagonRadius = _hexagonHeight * 0.5f;

	m_cellWidth = m_hexagonApothem * 2.f;
	m_cellHeight = m_hexagonRadius * 2.f;
}

Vector2f HexGrid::GetCellPosition(const Vector2i& coords) const
{
    return Vector2f(coords.x * m_cellWidth + (coords.y % 2) * m_hexagonApothem, coords.y * (1.5f * m_hexagonRadius));
}

Vector2f HexGrid::GetCellCenter(const Vector2i& coords) const
{
    return Vector2f(m_hexagonApothem + coords.x * m_cellWidth + (coords.y % 2) * m_hexagonApothem, m_hexagonRadius + coords.y * (1.5f * m_hexagonRadius));
}

bool HexGrid::PickCoords(const Vector2f& position, Vector2i& pickedCoords) const
{
	if (m_width <= 0 || m_height <= 0 || m_cellWidth <= 0.f || m_cellHeight <= 0.f)
	{
		assert(false);  // Invalid cell dimensions.

		pickedCoords = Vector2::Zero_i;
		return false;
	}

	// Picking formula based on Sam Hocevar sample :
	// https://gamedev.stackexchange.com/questions/20742/how-can-i-implement-hexagonal-tilemap-picking-in-xna

	// Convert picking position to the algorithm coordinates system.
	float posY = position.y - m_hexagonRadius;
	float posX = position.x - m_hexagonApothem;

	float a = m_hexagonRadius;
	float b = m_hexagonApothem;
	float c = m_hexagonRadius / 2.f;

	// Find out which major row and column we are on:
	int pickedX = (int)std::floor(posX / b);
	int pickedY = (int)std::floor(posY / (a + c));

	// Compute the offset into these row and column:
	float dy = posX - (float)pickedX * b;
	float dx = posY - (float)pickedY * (a + c);

	// Are we on the left of the hexagon edge, or on the right?
	dy = ((pickedX ^ pickedY) & 1) == 0 ? b - dy : dy;
	int right = dy * (a - c) < b * (dx - c) ? 1 : 0;

	// Now we have all the information we need, just fine-tune row and column.
	pickedX += (pickedY ^ pickedX ^ right) & 1;
	pickedY += right;

	// Convert result to my own coordinates system.
	pickedCoords = Vector2i((pickedX - (pickedX & 1)) / 2, pickedY);

	return (pickedCoords.x >= 0 && pickedCoords.x < m_width
		&& pickedCoords.y >= 0 && pickedCoords.y < m_height);
}

bool HexGrid::PickCoordsClamped(const Vector2f& position, Vector2i& pickedCoords) const
{
	if (!PickCoords(position, pickedCoords))
	{
		pickedCoords.x = Clamp(pickedCoords.x, 0, m_width - 1);
		pickedCoords.y = Clamp(pickedCoords.y, 0, m_height - 1);
		return false;
	}

	return true;
}

void HexGrid::GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours) const
{
	static const std::array<Vector2i, 12> neighbourDirections = {
        // Even row.
		Vector2i(-1, 0),
		Vector2i(-1, -1),
		Vector2i(0, -1),
		Vector2i(1, 0),
		Vector2i(0, 1),
		Vector2i(-1, 1),

        // Odd row.
		Vector2i(-1, 0),
		Vector2i(0, -1),
		Vector2i(1, -1),
		Vector2i(1, 0),
		Vector2i(1, 1),
		Vector2i(0, 1),
	};

    neighbours.reserve(6);

    size_t offset = (coords.y % 2 == 0) ? 0 : 6;
    for (size_t i = offset; i < offset + 6; ++i)
    {
        Vector2i neighbour = coords + neighbourDirections[i];

        if (neighbour.x >= 0 && neighbour.x < m_width
            && neighbour.y >= 0 && neighbour.y < m_height)
        {
            neighbours.push_back(neighbour);
        }
    }
}

int HexGrid::GetWidth() const
{
    return m_width;
}

int HexGrid::GetHeight() const
{
    return m_height;
}

int HexGrid::GetCellCount() const
{
	return m_width * m_height;
}

Vector2f HexGrid::GetGridSize() const
{
	return Vector2f((m_width + 0.5f) * m_cellWidth, ((m_height - 1) * 1.5f + 2.f) * m_hexagonRadius);
}

float HexGrid::GetCellWidth() const
{
    return m_cellWidth;
}

float HexGrid::GetCellHeight() const
{
    return m_cellHeight;
}

Vector2f HexGrid::GetCellSize() const
{
	return Vector2f(m_cellWidth, m_cellHeight);
}

}   // namespace gugu
