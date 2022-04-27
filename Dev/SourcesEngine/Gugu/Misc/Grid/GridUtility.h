#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename TCoords>
struct BFSNeighbourInfos
{
    TCoords coords;
    int distance = 0;
};

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighboursByRange(const TGrid& grid, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours);

template<typename TGrid, typename TGridData, typename TCoords>
void BreadthFirstSearchNeighboursByWalkableRange(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours);

//template<typename TGrid, typename TGridData, typename TCoords, typename TCost>
//void BreadthFirstSearchNeighboursByWalkableCost(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, TCost cost, std::vector<TCoords>& neighbours);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/Misc/Grid/GridUtility.tpp"
