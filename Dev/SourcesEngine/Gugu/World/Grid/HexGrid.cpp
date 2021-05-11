////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/HexGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/GridUtility.h"
#include "Gugu/Utility/Math.h"

#include <array>

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

sf::Vector2f HexGrid::GetCellPosition(const sf::Vector2i& coords) const
{
    return sf::Vector2f(coords.x * m_cellWidth + (coords.y % 2) * m_hexagonApothem, coords.y * (1.5f * m_hexagonRadius));
}

sf::Vector2f HexGrid::GetCellCenter(const sf::Vector2i& coords) const
{
    return sf::Vector2f(m_hexagonApothem + coords.x * m_cellWidth + (coords.y % 2) * m_hexagonApothem, m_hexagonRadius + coords.y * (1.5f * m_hexagonRadius));
}

void HexGrid::GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const
{
	static const std::array<sf::Vector2i, 12> neighbourDirections = {
            // Even row.
			sf::Vector2i(-1, 0),
			sf::Vector2i(-1, -1),
			sf::Vector2i(0, -1),
			sf::Vector2i(1, 0),
			sf::Vector2i(0, 1),
			sf::Vector2i(-1, 1),

            // Odd row.
			sf::Vector2i(-1, 0),
			sf::Vector2i(0, -1),
			sf::Vector2i(1, -1),
			sf::Vector2i(1, 0),
			sf::Vector2i(1, 1),
			sf::Vector2i(0, 1),
	};

    neighbours.reserve(6);

    size_t offset = (coords.y % 2 == 0) ? 0 : 6;
    for (size_t i = offset; i < offset + 6; ++i)
    {
        sf::Vector2i neighbour = coords + neighbourDirections[i];

        if (neighbour.x >= 0 && neighbour.x < m_width
            && neighbour.y >= 0 && neighbour.y < m_height)
        {
            neighbours.push_back(neighbour);
        }
    }
}

void HexGrid::GetNeighboursInRange(const sf::Vector2i& coords, int range, std::vector<sf::Vector2i>& neighbours) const
{
    BreadthFirstSearchNeighbours(this, coords, range, neighbours);
}

int HexGrid::GetWidth() const
{
    return m_width;
}

int HexGrid::GetHeight() const
{
    return m_height;
}

float HexGrid::GetCellWidth() const
{
    return m_cellWidth;
}

float HexGrid::GetCellHeight() const
{
    return m_cellHeight;
}

}   // namespace gugu
