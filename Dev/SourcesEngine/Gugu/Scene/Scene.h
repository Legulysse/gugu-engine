#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class SceneActor;
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Scene
{
    friend class SceneActor;

public:

    Scene();
    virtual ~Scene();

    virtual void Step(const DeltaTime& dt);
    virtual void Update(const DeltaTime& dt);
    
    //Render
    Element* GetRootNode() const;
    
    //Sub Scenes
    Scene* CreateChildScene();
    Scene* AddChildScene(Scene* scene);
    void RemoveChildScene(Scene* scene);
    void DeleteChildScene(Scene* scene);
    void DeleteAllChildScenes();

    bool HasChildScene(Scene* scene) const;
    Scene* GetChildScene(int _iIndex = 0) const;

    //Actors
    SceneActor* AddActor(SceneActor* _pActor);
    void RemoveActor(SceneActor* _pActor);
    void DeleteActor(SceneActor* _pActor);
    void DeleteAllActors();

    bool HasActor(SceneActor* _pActor) const;
    SceneActor* GetActor(int _iIndex = 0) const;
    int GetActorCount() const;

protected:

    void OnChildSceneReleased(Scene* scene);    //Called by a ChildScene just released, to ensure the Scene is not referencing it.
    void OnActorReleased(SceneActor* _pActor);  //Called by an Actor just released, to ensure the Scene is not referencing it.

protected:

    Scene* m_parentScene;
    std::vector<Scene*> m_childScenes;
    std::vector<SceneActor*> m_actors;
    
    Element* m_rootNode;
};

}   // namespace gugu
