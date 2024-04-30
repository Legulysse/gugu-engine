#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Scene;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class SceneActor
{
    friend class Scene;

public:

    SceneActor();
    virtual ~SceneActor();

    virtual void Step(const DeltaTime& dt);
    virtual void Update(const DeltaTime& dt);
    virtual void LateUpdate(const DeltaTime& dt);
    virtual void UpdateImGui(const DeltaTime& dt);

    void RemoveFromScene();

protected:

    virtual void OnAddedToScene() {}
    virtual void OnRemovedFromScene() {}

protected:

    Scene* m_scene;
};

}   // namespace gugu
