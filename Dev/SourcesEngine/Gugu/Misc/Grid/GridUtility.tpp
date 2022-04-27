#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <set>
#include <queue>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighboursByRange(const TGrid& grid, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    std::set<TCoords> explored;

    pendingQueue.push(coordsFrom);
    explored.insert(coordsFrom);

    int distance = 0;
    while ((range == -1 || distance < range) && !pendingQueue.empty())
    {
        ++distance;

        // currentQueue should already be emptied by the previous loop.
        std::swap(currentQueue, pendingQueue);

        while (!currentQueue.empty())
        {
            TCoords currentCoords = currentQueue.front();
            currentQueue.pop();

            // Queue all neighbours for the next iteration.
            std::vector<TCoords> cellNeighbours;
            grid.GetNeighbours(currentCoords, cellNeighbours);

            for (const TCoords& neighbour : cellNeighbours)
            {
                if (explored.find(neighbour) == explored.end())
                {
                    explored.insert(neighbour);
                    pendingQueue.push(neighbour);

                    BFSNeighbourInfos<TCoords> neighbourInfos;
                    neighbourInfos.coords = neighbour;
                    neighbourInfos.distance = distance;
                    neighbours.push_back(neighbourInfos);
                }
            }
        }
    }
}

template<typename TGrid, typename TGridData, typename TCoords>
void BreadthFirstSearchNeighboursByWalkableRange(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    std::set<TCoords> explored;

    pendingQueue.push(coordsFrom);
    explored.insert(coordsFrom);

    int distance = 0;
    while ((range == -1 || distance < range) && !pendingQueue.empty())
    {
        ++distance;

        // currentQueue should already be emptied by the previous loop.
        std::swap(currentQueue, pendingQueue);

        while (!currentQueue.empty())
        {
            TCoords currentCoords = currentQueue.front();
            currentQueue.pop();

            // Queue all neighbours for the next iteration.
            std::vector<TCoords> cellNeighbours;
            grid.GetNeighbours(currentCoords, cellNeighbours);

            for (const TCoords& neighbour : cellNeighbours)
            {
                if (explored.find(neighbour) == explored.end())
                {
                    if (gridData.IsWalkable(currentCoords, neighbour))
                    {
                        explored.insert(neighbour);
                        pendingQueue.push(neighbour);

                        BFSNeighbourInfos<TCoords> neighbourInfos;
                        neighbourInfos.coords = neighbour;
                        neighbourInfos.distance = distance;
                        neighbours.push_back(neighbourInfos);
                    }
                }
            }
        }
    }
}

//template<typename TGrid, typename TGridData, typename TCoords, typename TCost>
//void BreadthFirstSearchNeighboursByWalkableCost(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, TCost cost, std::vector<TCoords>& neighbours)
//{
//}

}   // namespace gugu
