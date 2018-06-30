#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
	class BaseGrid;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class GridCell
{
public:

	GridCell();
	virtual ~GridCell();

	virtual void Init(BaseGrid* _pGrid, int _iIndexX, int _iIndexY);

    virtual void RecomputePosition(const sf::Vector2f _kPosition);

    int GetIndexX() const;
    int GetIndexY() const;

    sf::Vector2f GetPosition() const;

protected:

	BaseGrid* m_grid;
	int	m_indexX;
	int	m_indexY;

    sf::Vector2f m_position;
};

}   // namespace gugu
