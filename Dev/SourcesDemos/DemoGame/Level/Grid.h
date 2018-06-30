#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/SquareGrid.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Level;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class Grid : public gugu::SquareGrid
{
public:

    Grid();
    virtual ~Grid();
    
    void InitGrid(gugu::Level* _pLevel, int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight);

protected:
    
    virtual gugu::GridCell* InstantiateCell() override;
};

}   //namespace demoproject
