////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/SquareGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/GridUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SquareGrid::SquareGrid()
    : m_width(0)
    , m_height(0)
    , m_eightSides(true)
    , m_cellWidth(0.f)
    , m_cellHeight(0.f)
{
}

SquareGrid::~SquareGrid()
{
}

void SquareGrid::InitSquareGrid(int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight, bool eightSides)
{
    m_width = _iWidth;
    m_height = _iHeight;
    m_eightSides = eightSides;

    m_cellWidth = _fCellWidth;
    m_cellHeight = _fCellHeight;
}

sf::Vector2f SquareGrid::GetCellPosition(const sf::Vector2i& coords) const
{
    return sf::Vector2f(coords.x * m_cellWidth, coords.y * m_cellHeight);
}

void SquareGrid::GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const
{
    neighbours.reserve(m_eightSides ? 8 : 4);

    for (int offsetY = -1; offsetY <= 1; ++offsetY)
    {
        for (int offsetX = -1; offsetX <= 1; ++offsetX)
        {
            int testX = coords.x + offsetX;
            int testY = coords.y + offsetY;

            if ((offsetY != 0 || offsetX != 0)
                && (m_eightSides || offsetY == 0 || offsetX == 0)
                && testX >= 0 && testX < m_width
                && testY >= 0 && testY < m_height)
            {
                neighbours.push_back(sf::Vector2i(testX, testY));
            }
        }
    }
}

void SquareGrid::GetNeighboursInRange(const sf::Vector2i& coords, int range, std::vector<sf::Vector2i>& neighbours) const
{
    BreadthFirstSearchNeighbours(this, coords, range, neighbours);
}

int SquareGrid::GetWidth() const
{
    return m_width;
}

int SquareGrid::GetHeight() const
{
    return m_height;
}

float SquareGrid::GetCellWidth() const
{
    return m_cellWidth;
}

float SquareGrid::GetCellHeight() const
{
    return m_cellHeight;
}

}   // namespace gugu
