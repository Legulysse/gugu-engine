////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Scene/ManagerScenes.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ManagerScenes::ManagerScenes()
    : m_rootScene(nullptr)
{
}

ManagerScenes::~ManagerScenes()
{
}

void ManagerScenes::Init(const EngineConfig& config)
{
    ResetRootScene();
}

void ManagerScenes::Release()
{
    SafeDelete(m_rootScene);
}

void ManagerScenes::ResetRootScene()
{
    SafeDelete(m_rootScene);

    m_rootScene = new Scene;
}

Scene* ManagerScenes::GetRootScene() const
{
    return m_rootScene;
}

void ManagerScenes::Step(const DeltaTime& dt)
{
    if (m_rootScene)
    {
        m_rootScene->Step(dt);
    }
}

void ManagerScenes::Update(const DeltaTime& dt)
{
    if (m_rootScene)
    {
        m_rootScene->Update(dt);
    }
}

ManagerScenes* GetScenes()
{
    return GetEngine()->GetManagerScenes();
}

}   // namespace gugu
