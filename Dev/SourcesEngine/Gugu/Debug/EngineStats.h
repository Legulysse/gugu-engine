#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <list>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
struct EngineStats
{
    uint32 maxStatCount = 150;
    std::list<int> loopTimes;
    std::list<int> stepTimes;
    std::list<int> updateTimes;
    std::list<int> renderTimes;
    uint32 animationCount = 0;
    uint32 particleSystemCount = 0;
    bool isTracing = false;
};

}   // namespace gugu