#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighbours(const TGrid* grid, const TCoords& coords, int depth, std::vector<TCoords>& neighbours)
{
    grid->GetNeighbours(coords, neighbours);
}

}   // namespace gugu
