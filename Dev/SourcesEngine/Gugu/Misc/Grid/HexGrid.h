#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class HexGrid
{
public:

    HexGrid();
    virtual ~HexGrid();

    void InitHexGrid(int _iWidth, int _iHeight, float _hexagonWidth);
    void InitHexGrid(int _iWidth, int _iHeight, float _hexagonWidth, float _hexagonHeight);

    Vector2f GetCellPosition(const Vector2i& coords) const;
    Vector2f GetCellCenter(const Vector2i& coords) const;

    bool PickCoords(const Vector2f& position, Vector2i& pickedCoords) const;

    void GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours) const;

    // TODO: rename as rows/columns or rowWidth/columnHeight to differentiate from graphic size ?
    int GetWidth() const;
    int GetHeight() const;
    int GetCellCount() const;

    Vector2f GetGridSize() const;

    float GetCellWidth() const;
    float GetCellHeight() const;
    Vector2f GetCellSize() const;

protected:

    int m_width;
    int m_height;

    float m_hexagonApothem;
    float m_hexagonRadius;

    float m_cellWidth;
    float m_cellHeight;
};

}   // namespace gugu
