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
    int maxStatCount = 150;
    std::list<int> loopTimes;
    std::list<int> stepTimes;
    std::list<int> updateTimes;
    std::list<int> renderTimes;
    std::list<int> stepCount;
    int animationCount = 0;
    int particleSystemCount = 0;
    bool isTracing = false;
};

}   // namespace gugu
