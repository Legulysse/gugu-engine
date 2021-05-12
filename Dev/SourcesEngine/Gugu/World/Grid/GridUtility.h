#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighboursByRange(const TGrid& grid, const TCoords& coordsFrom, int range, std::vector<TCoords>& neighbours);

template<typename TGrid, typename TGridData, typename TCoords>
void BreadthFirstSearchNeighboursByWalkableRange(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, int range, std::vector<TCoords>& neighbours);

//template<typename TGrid, typename TGridData, typename TCoords, typename TCost>
//void BreadthFirstSearchNeighboursByWalkableCost(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, TCost cost, std::vector<TCoords>& neighbours);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/World/Grid/GridUtility.tpp"
