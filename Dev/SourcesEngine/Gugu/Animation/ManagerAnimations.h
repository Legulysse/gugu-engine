#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerAnimations
{
public:

    ManagerAnimations();
    ~ManagerAnimations();

    void Init(const EngineConfig& config);
    void Release();
};

ManagerAnimations* GetAnimations();

}   // namespace gugu
