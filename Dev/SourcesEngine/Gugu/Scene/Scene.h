#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class DeltaTime;
    class Element;
    class SceneActor;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Scene
{
public:

    Scene();
    virtual ~Scene();

    virtual void Step(const DeltaTime& dt);
    virtual void Update(const DeltaTime& dt);
    virtual void LateUpdate(const DeltaTime& dt);
    virtual void UpdateImGui(const DeltaTime& dt);
    
    //Render
    Element* GetRootNode() const;
    
    //Sub Scenes
    Scene* AddChildScene();
    void AddChildScene(Scene* scene);
    void RemoveChildScene(Scene* scene);
    void DeleteChildScene(Scene* scene);
    void DeleteAllChildScenes();

    bool HasChildScene(Scene* scene) const;
    Scene* GetChildScene(size_t index) const;
    size_t GetChildSceneCount() const;

    //Actors
    void AddActor(SceneActor* actor);
    void RemoveActor(SceneActor* actor);
    void DeleteActor(SceneActor* actor);
    void DeleteAllActors();

    bool HasActor(SceneActor* actor) const;
    SceneActor* GetActor(size_t index) const;
    size_t GetActorCount() const;

protected:

    Scene* m_parentScene;
    std::vector<Scene*> m_childScenes;
    std::vector<SceneActor*> m_actors;
    
    Element* m_rootNode;
};

}   // namespace gugu
