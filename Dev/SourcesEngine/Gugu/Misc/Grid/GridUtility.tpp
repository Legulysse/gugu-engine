#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <set>
#include <queue>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TGrid, typename TCoords>
void BFSNeighboursByCellRange(const TGrid& grid, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    static std::set<TCoords> explored;          // Cleared at end of function.
    static std::vector<TCoords> cellNeighbours; // Cleared at end of function.

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
            cellNeighbours.clear();
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

    explored.clear();
    cellNeighbours.clear();
}

template<typename TGrid, typename TGridData, typename TAgent, typename TCoords>
void BFSNeighboursByTraversableRange(const TGrid& grid, const TGridData& gridData, const TAgent& agent, const TCoords& coordsFrom, int range, std::vector<BFSNeighbourInfos<TCoords>>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    static std::set<TCoords> explored;          // Cleared at end of function.
    static std::vector<TCoords> cellNeighbours; // Cleared at end of function.

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
            cellNeighbours.clear();
            grid.GetNeighbours(currentCoords, cellNeighbours);

            for (const TCoords& neighbour : cellNeighbours)
            {
                if (explored.find(neighbour) == explored.end())
                {
                    if (gridData.IsTraversable(agent, currentCoords, neighbour))
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

    explored.clear();
    cellNeighbours.clear();
}

//template<typename TGrid, typename TGridData, typename TCoords, typename TCost>
//void BFSNeighboursByTraversableCost(const TGrid& grid, const TGridData& gridData, const TAgent& agent, const TCoords& coordsFrom, TCost cost, std::vector<TCoords>& neighbours)
//{
//}

}   // namespace gugu
