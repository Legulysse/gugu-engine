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
        m_parentScene->RemoveChildScene(this);
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
}

void Scene::LateUpdate(const DeltaTime& dt)
{
    for (size_t i = 0; i < m_childScenes.size(); ++i)
    {
        if (m_childScenes[i])
            m_childScenes[i]->LateUpdate(dt);
    }

    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
            m_actors[i]->LateUpdate(dt);
    }
}

Element* Scene::GetRootNode() const
{
    return m_rootNode;
}

Scene* Scene::AddChildScene()
{
    Scene* scene = new Scene;
    AddChildScene(scene);
    return scene;
}

void Scene::AddChildScene(Scene* scene)
{
    if (scene->m_parentScene)
    {
        scene->m_parentScene->RemoveChildScene(scene);
    }

    m_childScenes.push_back(scene);
    scene->m_parentScene = this;
}

void Scene::RemoveChildScene(Scene* scene)
{
    if (scene->m_parentScene == this)
    {
        StdVectorRemove(m_childScenes, scene);
        scene->m_parentScene = nullptr;
    }
}

void Scene::DeleteChildScene(Scene* scene)
{
    if (scene->m_parentScene == this)
    {
        RemoveChildScene(scene);
        SafeDelete(scene);
    }
}

void Scene::DeleteAllChildScenes()
{
    for (size_t i = 0; i < m_childScenes.size(); ++i)
    {
        if (m_childScenes[i])
        {
            m_childScenes[i]->m_parentScene = nullptr;
            SafeDelete(m_childScenes[i]);
        }
    }

    m_childScenes.clear();
}

bool Scene::HasChildScene(Scene* scene) const
{
    return scene != nullptr && StdVectorContains(m_childScenes, scene);
}

Scene* Scene::GetChildScene(int index) const
{
    if (index < 0 || index >= (int)m_childScenes.size())
        return nullptr;
    return m_childScenes[index];
}

void Scene::AddActor(SceneActor* actor)
{
    if (actor->m_scene)
    {
        actor->m_scene->RemoveActor(actor);
    }

    m_actors.push_back(actor);
    actor->m_scene = this;

    actor->OnAddedToScene();
}

void Scene::RemoveActor(SceneActor* actor)
{
    if (actor->m_scene == this)
    {
        actor->OnRemovedFromScene();

        StdVectorRemove(m_actors, actor);
        actor->m_scene = nullptr;
    }
}

void Scene::DeleteActor(SceneActor* actor)
{
    if (actor->m_scene == this)
    {
        RemoveActor(actor);
        SafeDelete(actor);
    }
}

void Scene::DeleteAllActors()
{
    for (size_t i = 0; i < m_actors.size(); ++i)
    {
        if (m_actors[i])
        {
            m_actors[i]->OnRemovedFromScene();

            m_actors[i]->m_scene = nullptr;
            SafeDelete(m_actors[i]);
        }
    }

    m_actors.clear();
}

bool Scene::HasActor(SceneActor* actor) const
{
    return actor != nullptr && StdVectorContains(m_actors, actor);
}

SceneActor* Scene::GetActor(int index) const
{
    if (index < 0 || index >= (int)m_actors.size())
        return nullptr;
    return m_actors[index];
}

int Scene::GetActorCount() const
{
    return (int)m_actors.size();
}

}   // namespace gugu
