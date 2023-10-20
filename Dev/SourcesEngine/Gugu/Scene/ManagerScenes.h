#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    class DeltaTime;
    class Scene;
}

namespace sf
{
    class RenderWindow;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class ManagerScenes
{
public:

    ManagerScenes();
    virtual ~ManagerScenes();

    void Init(const EngineConfig& config);
    void Release();

    void ResetRootScene();
    Scene* GetRootScene() const;

    void Step(const DeltaTime& dt);
    void Update(const DeltaTime& dt);
    void LateUpdate(const DeltaTime& dt);

protected:

    Scene* m_rootScene;
};

ManagerScenes* GetScenes();

}   // namespace gugu
