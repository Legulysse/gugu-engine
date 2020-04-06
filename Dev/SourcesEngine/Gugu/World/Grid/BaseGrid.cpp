////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/World/Grid/BaseGrid.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/GridCell.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

BaseGrid::BaseGrid()
{
    m_Cells = nullptr;
    m_width = 0;
    m_height = 0;
}

BaseGrid::~BaseGrid()
{
    Clear();
}

void BaseGrid::GenerateCells(int _iWidth, int _iHeight)
{
    if (!m_Cells && _iWidth > 0 && _iHeight > 0)
    {
        m_width = _iWidth;
        m_height = _iHeight;

        m_Cells = new GridCell**[m_width];
        for (int x = 0; x < m_width; ++x)
        {
            m_Cells[x] = new GridCell*[m_height];
            for (int y = 0; y < m_height; ++y)
            {
                GridCell* pCell = InstantiateCell();
                m_Cells[x][y] = pCell;
                pCell->Init(this, x, y);
            }
        }
    }
}

void BaseGrid::Clear()
{
    if (m_Cells)
    {
        for (int x = 0; x < m_width; ++x)
        {
            for (int y = 0; y < m_height; ++y)
            {
                delete m_Cells[x][y];
            }
            delete [] m_Cells[x];
        }
        delete [] m_Cells;
    }
}
    
GridCell* BaseGrid::InstantiateCell()
{
    return new GridCell;
}

int BaseGrid::GetGridWidth() const
{
    return m_width;
}

int BaseGrid::GetGridHeight() const
{
    return m_height;
}

}   // namespace gugu
