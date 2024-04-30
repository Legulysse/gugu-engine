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
    RemoveFromScene();
}

void SceneActor::Step(const DeltaTime& dt)
{
}

void SceneActor::Update(const DeltaTime& dt)
{
}

void SceneActor::LateUpdate(const DeltaTime& dt)
{
}

void SceneActor::UpdateImGui(const DeltaTime& dt)
{
}

void SceneActor::RemoveFromScene()
{
    if (m_scene)
    {
        m_scene->RemoveActor(this);
        m_scene = nullptr;
    }
}

}   // namespace gugu
