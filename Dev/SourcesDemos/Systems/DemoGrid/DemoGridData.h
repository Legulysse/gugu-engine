#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

class DemoGridData
{
public:

    DemoGridData();

    void GenerateCells(int width, int height);
    void ResetCells();

    bool IsBlocked(const gugu::Vector2i& coords) const;
    void SetBlocked(const gugu::Vector2i& coords, bool blocked);

    // Check diagonal move (only works for square-8 grids).
    bool AreDiagonalsRestricted() const;
    void SetDiagonalsRestricted(bool restrictDiagonals);

    bool IsTraversable(const int& agent, const gugu::Vector2i& coordsFrom, const gugu::Vector2i& coordsTo) const;
    //bool IsTraversable(const int& agent, const gugu::Vector2i& coordsFrom, const gugu::Vector2i& coordsTo, float& cost) const;

private:

    struct DemoCellData
    {
        bool blocked = false;
    };

    int m_width;
    int m_height;
    std::vector<DemoCellData> m_cells;

    bool m_restrictDiagonals;
};

}   //namespace demoproject
