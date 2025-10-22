#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SquareGrid
{
public:

    // TODO:
    // - Use Vectors for all sizes, getters and ctor.
    // - IndexFromCoords, TryGetIndexFromCoords.
    // - CoordsFromIndex, TryCoordsFromIndex.

    SquareGrid();
    virtual ~SquareGrid();

    void InitSquareGrid(int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight, bool eightSides);

    Vector2f GetCellPosition(const Vector2i& coords) const;
    Vector2f GetCellCenter(const Vector2i& coords) const;

    bool PickCoords(const Vector2f& position, Vector2i& pickedCoords) const;
    bool PickCoordsClamped(const Vector2f& position, Vector2i& pickedCoords) const;

    void GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours) const;
    void GetNeighbours(const Vector2i& coords, std::vector<Vector2i>& neighbours, bool eightSides) const;

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
    bool m_eightSides;

    float m_cellWidth;
    float m_cellHeight;
};

}   // namespace gugu
