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
    : m_mainScene(nullptr)
    , m_persistentScene(nullptr)
{
}

ManagerScenes::~ManagerScenes()
{
}

void ManagerScenes::Init(const EngineConfig& config)
{
    ResetDefaultScenes();
}

void ManagerScenes::Release()
{
    SafeDelete(m_persistentScene);
    SafeDelete(m_mainScene);
}

void ManagerScenes::ResetDefaultScenes()
{
    SafeDelete(m_persistentScene);
    SafeDelete(m_mainScene);

    m_mainScene = new Scene;
    m_persistentScene = new Scene;
}

Scene* ManagerScenes::GetMainScene() const
{
    return m_mainScene;
}

Scene* ManagerScenes::GetPersistentScene() const
{
    return m_persistentScene;
}

void ManagerScenes::Step(const DeltaTime& dt)
{
    if (m_mainScene)
    {
        m_mainScene->Step(dt);
    }

    if (m_persistentScene)
    {
        m_persistentScene->Step(dt);
    }
}

void ManagerScenes::Update(const DeltaTime& dt)
{
    if (m_mainScene)
    {
        m_mainScene->Update(dt);
    }

    if (m_persistentScene)
    {
        m_persistentScene->Update(dt);
    }
}

ManagerScenes* GetScenes()
{
    return GetEngine()->GetManagerScenes();
}

}   // namespace gugu
