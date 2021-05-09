#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <Sfml/System/Vector2.hpp>

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class HexGrid
{
public:

    HexGrid();
    virtual ~HexGrid();

    void InitHexGrid(int _iWidth, int _iHeight);

    void GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const;
    void GetNeighboursInRange(const sf::Vector2i& coords, int range, std::vector<sf::Vector2i>& neighbours) const;

    int GetWidth() const;
    int GetHeight() const;

protected:

    int m_width;
    int m_height;
};

}   // namespace gugu
