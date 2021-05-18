////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/GridCell.h"

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu
{
GridCell::GridCell()
: m_grid (nullptr)
, m_indexX(0)
, m_indexY(0)
{
}

GridCell::~GridCell()
{
}

void GridCell::Init(BaseGrid* _pGrid, int _iIndexX, int _iIndexY)
{
    m_grid = _pGrid;
    m_indexX = _iIndexX;
    m_indexY = _iIndexY;
}

void GridCell::RecomputePosition(const sf::Vector2f _kPosition)
{
    m_position = _kPosition;
}

int GridCell::GetIndexX() const
{
    return m_indexX;
}

int GridCell::GetIndexY() const
{
    return m_indexY;
}

sf::Vector2f GridCell::GetPosition() const
{
    return m_position;
}

}   // namespace gugu
