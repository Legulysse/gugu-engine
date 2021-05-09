#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <Sfml/System/Vector2.hpp>

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class SquareGrid
{
public:

    SquareGrid();
    virtual ~SquareGrid();

    void InitSquareGrid(int _iWidth, int _iHeight, float _fCellWidth, float _fCellHeight, bool eightSides);

    //void RecomputePositions();

    sf::Vector2f GetCellPosition(const sf::Vector2i& coords) const;
    void GetNeighbours(const sf::Vector2i& coords, std::vector<sf::Vector2i>& neighbours) const;
    void GetNeighboursInRange(const sf::Vector2i& coords, int range, std::vector<sf::Vector2i>& neighbours) const;

    int GetWidth() const;
    int GetHeight() const;

    float GetCellWidth() const;
    float GetCellHeight() const;

protected:

    int m_width;
    int m_height;
    bool m_eightSides;

    float m_cellWidth;
    float m_cellHeight;
};

}   // namespace gugu
