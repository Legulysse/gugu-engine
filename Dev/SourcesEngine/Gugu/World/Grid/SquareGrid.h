#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/BaseGrid.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SquareGrid : public BaseGrid
{
public:

    SquareGrid();
    virtual ~SquareGrid();

    void InitSquareGrid(int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight);

    virtual void RecomputePositions() override;

    float GetCellWidth() const;
    float GetCellHeight() const;

protected:

    float m_cellWidth;
    float m_cellHeight;
};

}   // namespace gugu
