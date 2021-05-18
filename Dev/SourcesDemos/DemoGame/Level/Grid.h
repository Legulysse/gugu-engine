#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Level;
    class ElementTileMap;
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
    
    void ClampPositionInsideBounds(sf::Vector2f& position) const;

private:

    gugu::ElementTileMap* m_pTileMap;
};

}   //namespace demoproject
