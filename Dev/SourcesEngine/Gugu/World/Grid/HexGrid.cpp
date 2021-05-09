////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/HexGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/GridUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

HexGrid::HexGrid()
    : m_width(0)
    , m_height(0)
{
}

HexGrid::~HexGrid()
{
}

void HexGrid::InitHexGrid(int _iWidth, int _iHeight)
{
    m_width = _iWidth;
    m_height = _iHeight;
}

void HexGrid::GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const
{
    neighbours.reserve(6);
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
