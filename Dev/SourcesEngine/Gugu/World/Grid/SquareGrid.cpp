////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/SquareGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/GridCell.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

SquareGrid::SquareGrid()
: m_cellWidth(0.f)
, m_cellHeight(0.f)
{
}

SquareGrid::~SquareGrid()
{
}

void SquareGrid::InitSquareGrid(int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight)
{
    GenerateCells(_iWidth, _iHeight);

    m_cellWidth = _fCellWidth;
    m_cellHeight = _fCellHeight;

    RecomputePositions();
}

void SquareGrid::RecomputePositions()
{
    if (m_Cells && m_width > 0 && m_height > 0 && m_cellWidth > 0.f && m_cellHeight > 0.f)
	{
		for (int x = 0; x < m_width; ++x)
		{
			for (int y = 0; y < m_height; ++y)
			{
                sf::Vector2f kPosition;
				kPosition.x = x * m_cellWidth;
				kPosition.y = y * m_cellHeight;

				m_Cells[x][y]->RecomputePosition(kPosition);
			}
		}
	}
}

}   // namespace gugu
