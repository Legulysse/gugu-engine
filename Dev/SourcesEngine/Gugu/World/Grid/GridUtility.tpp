#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <set>
#include <queue>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TGrid, typename TCoords>
void BreadthFirstSearchNeighbours(const TGrid* grid, const TCoords& coords, int depth, std::vector<TCoords>& neighbours)
{
    std::queue<TCoords> currentQueue;
    std::queue<TCoords> pendingQueue;
    std::set<TCoords> exploredSet;

    pendingQueue.push(coords);
    exploredSet.insert(coords);

    int currentDepth = 0;
    while (currentDepth < depth && !pendingQueue.empty())
    {
        ++currentDepth;

        // currentQueue should already be emptied by the previous loop.
        std::swap(currentQueue, pendingQueue);

        while (!currentQueue.empty())
        {
            TCoords currentCoords = currentQueue.front();
            currentQueue.pop();

            // Queue all neighbours for the next iteration.
            std::vector<TCoords> cellNeighbours;
            grid->GetNeighbours(currentCoords, cellNeighbours);

            for (const TCoords& neighbour : cellNeighbours)
            {
                if (exploredSet.find(neighbour) == exploredSet.end())
                {
                    pendingQueue.push(neighbour);
                    exploredSet.insert(neighbour);

                    neighbours.push_back(neighbour);
                }
            }
        }
    }
}

}   // namespace gugu
