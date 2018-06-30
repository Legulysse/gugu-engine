#pragma once

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
	class GridCell;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class BaseGrid
{
public:

			BaseGrid();
	virtual ~BaseGrid();

	virtual	void Clear();

    virtual void RecomputePositions() = 0;

protected:

			void GenerateCells(int _iWidth, int _iHeight);

     virtual GridCell* InstantiateCell();
	
protected:

	GridCell*** m_Cells;
	int m_width;
	int m_height;
};

}   // namespace gugu
