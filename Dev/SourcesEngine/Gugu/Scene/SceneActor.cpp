////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Scene/SceneActor.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Scene/Scene.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
SceneActor::SceneActor()
    : m_scene(nullptr)
{
}

SceneActor::~SceneActor()
{
    //Inform the Scene if released from gameplay (If released from Scene, this reference is already null).
    if (m_scene)
    {
        m_scene->OnActorReleased(this);
        m_scene = nullptr;
    }
}

void SceneActor::Step(const DeltaTime& dt)
{
}

void SceneActor::Update(const DeltaTime& dt)
{
}

}   // namespace gugu
