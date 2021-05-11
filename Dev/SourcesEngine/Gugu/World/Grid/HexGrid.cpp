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
    , m_hexagonWidth(0.f)
    , m_hexagonRadius(0.f)
{
}

HexGrid::~HexGrid()
{
}

void HexGrid::InitHexGrid(int _iWidth, int _iHeight, float _hexagonWidth)
{
    m_width = _iWidth;
    m_height = _iHeight;

    m_hexagonWidth = _hexagonWidth;
    m_hexagonRadius = std::sqrtf(std::pow(m_hexagonWidth, 2) / 3.f);
}

sf::Vector2f HexGrid::GetCellPosition(const sf::Vector2i& coords) const
{
    //float fCenterX = (m_hexagonWidth / 2.f) + ((coords.y % 2) * (m_hexagonWidth / 2.f)) + (coords.x * m_hexagonWidth);
    //float fCenterY = m_hexagonRadius + ((1.5f * m_hexagonRadius) * coords.y);

    return sf::Vector2f(coords.x * m_hexagonWidth + (coords.y % 2) * (m_hexagonWidth / 2.f), coords.y * (1.5f * m_hexagonRadius));
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

}   // namespace gugu
