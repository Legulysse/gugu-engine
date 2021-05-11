#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <Gugu/Utility/Vector2.h>

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
    void GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const;
    void GetNeighboursInRange(const sf::Vector2i& coords, int range, std::vector<sf::Vector2i>& neighbours) const;

    int GetWidth() const;
    int GetHeight() const;

protected:

    int m_width;
    int m_height;

    float m_hexagonWidth;
    float m_hexagonRadius;
};

}   // namespace gugu
