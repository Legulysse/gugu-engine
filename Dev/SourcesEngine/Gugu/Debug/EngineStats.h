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
    std::list<float> loopTimes;
    std::list<float> stepTimes;
    std::list<float> updateTimes;
    std::list<float> renderTimes;
    std::list<int> stepCount;
    int animationCount = 0;
    int particleSystemCount = 0;
    bool isTracing = false;
};

}   // namespace gugu
