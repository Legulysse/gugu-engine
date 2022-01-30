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
    if (m_scene)
    {
        m_scene->RemoveActor(this);
    }
}

void SceneActor::Step(const DeltaTime& dt)
{
}

void SceneActor::Update(const DeltaTime& dt)
{
}

}   // namespace gugu
