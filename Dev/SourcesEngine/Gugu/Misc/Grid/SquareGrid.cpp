////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Misc/Grid/SquareGrid.h"

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

sf::Vector2f SquareGrid::GetCellCenter(const sf::Vector2i& coords) const
{
    return sf::Vector2f(m_cellWidth * 0.5f + coords.x * m_cellWidth, m_cellHeight * 0.5f + coords.y * m_cellHeight);
}

bool SquareGrid::PickCoords(const sf::Vector2f& position, sf::Vector2i& pickedCoords) const
{
    if (position.x < 0 || position.x >= m_cellWidth * m_width || position.y < 0 || position.y >= m_cellHeight * m_height)
        return false;

    pickedCoords.x = (int)(position.x / m_cellWidth);
    pickedCoords.y = (int)(position.y / m_cellHeight);

    return true;
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
