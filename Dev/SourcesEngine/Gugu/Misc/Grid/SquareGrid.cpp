////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Misc/Grid/SquareGrid.h"

#include <array>

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

Vector2f SquareGrid::GetCellPosition(const Vector2i& coords) const
{
    return Vector2f(coords.x * m_cellWidth, coords.y * m_cellHeight);
}

Vector2f SquareGrid::GetCellCenter(const Vector2i& coords) const
{
    return Vector2f(m_cellWidth * 0.5f + coords.x * m_cellWidth, m_cellHeight * 0.5f + coords.y * m_cellHeight);
}

bool SquareGrid::PickCoords(const Vector2f& position, Vector2i& pickedCoords) const
{
    if (position.x < 0 || position.x >= m_cellWidth * m_width || position.y < 0 || position.y >= m_cellHeight * m_height)
        return false;

    pickedCoords.x = (int)(position.x / m_cellWidth);
    pickedCoords.y = (int)(position.y / m_cellHeight);

    return true;
}

void SquareGrid::GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours) const
{
    GetNeighbours(coords, neighbours, m_eightSides);
}

void SquareGrid::GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours, bool eightSides) const
{
    static const std::array<Vector2i, 12> neighbourDirections = {
        // Four sides.
        Vector2i(-1, 0),
        Vector2i(0, -1),
        Vector2i(1, 0),
        Vector2i(0, 1),

        // Eight sides.
        Vector2i(-1, 0),
        Vector2i(-1, -1),
        Vector2i(0, -1),
        Vector2i(1, -1),
        Vector2i(1, 0),
        Vector2i(1, 1),
        Vector2i(0, 1),
        Vector2i(-1, 1),
    };

    neighbours.reserve(eightSides ? 8 : 4);

    size_t from = eightSides ? 4 : 0;
    size_t to = eightSides ? 12 : 4;
    for (size_t i = from; i < to; ++i)
    {
        Vector2i neighbour = coords + neighbourDirections[i];

        if (neighbour.x >= 0 && neighbour.x < m_width
            && neighbour.y >= 0 && neighbour.y < m_height)
        {
            neighbours.push_back(neighbour);
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

int SquareGrid::GetCellCount() const
{
    return m_width * m_height;
}

Vector2f SquareGrid::GetGridSize() const
{
    return Vector2f(m_width * m_cellWidth, m_height * m_cellHeight);
}

float SquareGrid::GetCellWidth() const
{
    return m_cellWidth;
}

float SquareGrid::GetCellHeight() const
{
    return m_cellHeight;
}

Vector2f SquareGrid::GetCellSize() const
{
    return Vector2f(m_cellWidth, m_cellHeight);
}

}   // namespace gugu
