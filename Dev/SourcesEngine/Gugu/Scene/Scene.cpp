////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Scene/Scene.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Scene/SceneActor.h"
#include "Gugu/Element/Element.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Scene::Scene()
    : m_parentScene(nullptr)
{
    m_rootNode = new Element;
}

Scene::~Scene()
{
    // This will purge all rendering bindings using this scene.
    GetEngine()->OnSceneReleased(this);

    if (m_parentScene)
    {
        m_parentScene->OnChildSceneReleased(this);
        m_parentScene = nullptr;
    }

    DeleteAllActors();
    DeleteAllChildScenes();

    SafeDelete(m_rootNode);
}

void Scene::Step(const DeltaTime& dt)
{
    StdVectorRemove<Scene*>(m_childScenes, nullptr);
    StdVectorRemove<SceneActor*>(m_actors, nullptr);

    for (size_t i = 0; i < m_childScenes.size(); ++i)
    {
        if (m_childScenes[i])
            m_childScenes[i]->Step(dt);
    }

    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
            m_actors[i]->Step(dt);
    }
    
    m_rootNode->Step(dt);
}

void Scene::Update(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_childScenes.size(); ++i)
    {
        if (m_childScenes[i])
            m_childScenes[i]->Update(dt);
    }

    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
            m_actors[i]->Update(dt);
    }
    
    m_rootNode->Update(dt);
}

Element* Scene::GetRootNode() const
{
    return m_rootNode;
}

Scene* Scene::CreateChildScene()
{
    return AddChildScene(new Scene);
}

Scene* Scene::AddChildScene(Scene* scene)
{
    if (!scene)
        return nullptr;

    if (!StdVectorContains(m_childScenes, scene))
    {
        m_childScenes.push_back(scene);
        scene->m_parentScene = this;
        return scene;
    }

    return nullptr;
}

void Scene::RemoveChildScene(Scene* scene)
{
    if (!scene)
        return;

    auto it = StdVectorFind(m_childScenes, scene);
    if (it != m_childScenes.end())
    {
        scene->m_parentScene = nullptr;
        *it = nullptr;
    }
}

void Scene::DeleteChildScene(Scene* scene)
{
    if (!scene)
        return;

    RemoveChildScene(scene);
    SafeDelete(scene);
}

void Scene::DeleteAllChildScenes()
{
    for (size_t i = 0; i < m_childScenes.size(); ++i)
    {
        if (m_childScenes[i])
        {
            m_childScenes[i]->m_parentScene = nullptr;   //Prevent the call to OnChildSceneReleased
            SafeDelete(m_childScenes[i]);
        }
    }
    m_childScenes.clear();
}

bool Scene::HasChildScene(Scene* scene) const
{
    if (!scene)
        return false;

    return StdVectorContains(m_childScenes, scene);
}

Scene* Scene::GetChildScene(int _iIndex) const
{
    if (_iIndex < 0 || _iIndex >= (int)m_childScenes.size())
        return nullptr;
    return m_childScenes[_iIndex];
}

void Scene::OnChildSceneReleased(Scene* scene)
{
    if (!scene)
        return;

    auto it = StdVectorFind(m_childScenes, scene);
    if (it != m_childScenes.end())
    {
        *it = nullptr;
    }
}

SceneActor* Scene::AddActor(SceneActor* _pActor)
{
    if (!_pActor)
        return nullptr;

    if (!StdVectorContains(m_actors, _pActor))
    {
        m_actors.push_back(_pActor);
        _pActor->m_scene = this;
        return _pActor;
    }

    return nullptr;
}

void Scene::RemoveActor(SceneActor* _pActor)
{
    if (!_pActor)
        return;

    auto iteActor = StdVectorFind(m_actors, _pActor);
    if (iteActor != m_actors.end())
    {
        _pActor->m_scene = nullptr;
        *iteActor = nullptr;
    }
}

void Scene::DeleteActor(SceneActor* _pActor)
{
    if (!_pActor)
        return;

    RemoveActor(_pActor);
    SafeDelete(_pActor);
}

void Scene::DeleteAllActors()
{
    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
        {
            m_actors[i]->m_scene = nullptr;    //Prevent the call to OnActorReleased
            SafeDelete(m_actors[i]);
        }
    }
    m_actors.clear();
}

bool Scene::HasActor(SceneActor* _pActor) const
{
    if (!_pActor)
        return false;

    return StdVectorContains(m_actors, _pActor);
}

SceneActor* Scene::GetActor(int _iIndex) const
{
    if (_iIndex < 0 || _iIndex >= (int)m_actors.size())
        return nullptr;
    return m_actors[_iIndex];
}

int Scene::GetActorCount() const
{
    return (int)m_actors.size();
}

void Scene::OnActorReleased(SceneActor* _pActor)
{
    if (!_pActor)
        return;

    auto iteActor = StdVectorFind(m_actors, _pActor);
    if (iteActor != m_actors.end())
    {
        *iteActor = nullptr;
    }
}

}   // namespace gugu
