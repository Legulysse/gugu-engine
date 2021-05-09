#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighbours(const TGrid* grid, const TCoords& coords, int depth, std::vector<TCoords>& neighbours);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/World/Grid/GridUtility.tpp"
