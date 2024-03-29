#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Grid/SquareGrid.h"
#include "Gugu/Math/Vector2.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Scene;
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
    
    void InitGrid(gugu::Scene* scene, int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight);
    
    void ClampPositionInsideBounds(gugu::Vector2f& position) const;

private:

    gugu::ElementTileMap* m_pTileMap;
};

}   //namespace demoproject
