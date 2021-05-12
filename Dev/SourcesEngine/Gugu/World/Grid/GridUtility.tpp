#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <set>
#include <queue>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighboursByRange(const TGrid& grid, const TCoords& coordsFrom, int range, std::vector<TCoords>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    std::set<TCoords> explored;

    pendingQueue.push(coordsFrom);
    explored.insert(coordsFrom);

    int distance = 0;
    while (distance < range && !pendingQueue.empty())
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
                    neighbours.push_back(neighbour);
                }
            }
        }
    }
}

template<typename TGrid, typename TGridData, typename TCoords>
void BreadthFirstSearchNeighboursByWalkableRange(const TGrid& grid, const TGridData& gridData, const TCoords& coordsFrom, int range, std::vector<TCoords>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    std::set<TCoords> explored;

    pendingQueue.push(coordsFrom);
    explored.insert(coordsFrom);

    int distance = 0;
    while (distance < range && !pendingQueue.empty())
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

                    if (gridData.IsWalkable(currentCoords, neighbour))
                    {
                        pendingQueue.push(neighbour);
                        neighbours.push_back(neighbour);
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
