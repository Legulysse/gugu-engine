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

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo) const
{
    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

bool DemoGridData::IsWalkable(const sf::Vector2i& coordsFrom, const sf::Vector2i& coordsTo, float& cost) const
{
    cost = 1.f;
    return !m_cells[coordsTo.x + m_width * coordsTo.y].blocked;
}

}   //namespace demoproject
