////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "DemoGridData.h"

////////////////////////////////////////////////////////////////
// Includes

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

DemoGridData::DemoGridData()
    : m_width(0)
    , m_height(0)
    , m_restrictDiagonals(false)
{
}

void DemoGridData::GenerateCells(int width, int height)
{
    m_width = width;
    m_height = height;

    ResetCells();
}

void DemoGridData::ResetCells()
{
    m_cells.clear();
    m_cells.resize(m_width * m_height);
}

bool DemoGridData::IsBlocked(const sf::Vector2i& coords) const
{
    return m_cells[coords.x + m_width * coords.y].blocked;
}

void DemoGridData::SetBlocked(const sf::Vector2i& coords, bool blocked)
{
    m_cells[coords.x + m_width * coords.y].blocked = blocked;
}

bool DemoGridData::AreDiagonalsRestricted() const
{
    return m_restrictDiagonals;
}

void DemoGridData::SetDiagonalsRestricted(bool restrictDiagonals)
{
    m_restrictDiagonals = restrictDiagonals;
}

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo) const
{
    // Check diagonal move (only works for square-8 grids).
    if (m_restrictDiagonals)
    {
        sf::Vector2i direction = coordsTo - coordsFrom;
        if (direction.x != 0 && direction.y != 0)
        {
            sf::Vector2i testA = coordsFrom + sf::Vector2i(direction.x, 0);
            sf::Vector2i testB = coordsFrom + sf::Vector2i(0, direction.y);
            if (m_cells[testA.x + m_width * testA.y].blocked || m_cells[testB.x + m_width * testB.y].blocked)
            {
                return false;
            }
        }
    }

    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo, float& cost) const
{
    cost = 1.f;
    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

}   //namespace demoproject
