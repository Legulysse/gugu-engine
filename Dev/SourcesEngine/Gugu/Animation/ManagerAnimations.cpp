////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Animation/ManagerAnimations.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerAnimations::ManagerAnimations()
{
}

ManagerAnimations::~ManagerAnimations()
{
}

void ManagerAnimations::Init(const EngineConfig& config)
{
}

void ManagerAnimations::Release()
{
}

ManagerAnimations* GetAnimations()
{
    return GetEngine()->GetManagerAnimations();
}

}   // namespace gugu
