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

    sf::Vector2f GetCellPosition(const sf::Vector2i& coords) const;
    sf::Vector2f GetCellCenter(const sf::Vector2i& coords) const;

    bool PickCoords(const sf::Vector2f& position, sf::Vector2i& pickedCoords) const;

    void GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const;

    // TODO: rename as rows/columns or rowWidth/columnHeight to differentiate from graphic size ?
    int GetWidth() const;
    int GetHeight() const;

    sf::Vector2f GetGridSize() const;

    float GetCellWidth() const;
    float GetCellHeight() const;
    sf::Vector2f GetCellSize() const;

protected:

    int m_width;
    int m_height;

    float m_hexagonApothem;
    float m_hexagonRadius;

    float m_cellWidth;
    float m_cellHeight;
};

}   // namespace gugu
