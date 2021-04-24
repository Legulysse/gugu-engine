#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/World/Grid/SquareGrid.h"

#include <SFML/System/Vector2.hpp>

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

protected:
    
    virtual gugu::GridCell* InstantiateCell() override;

private:

    gugu::ElementTileMap* m_pTileMap;
};

}   //namespace demoproject
