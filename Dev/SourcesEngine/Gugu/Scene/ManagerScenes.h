#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
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

    void ResetDefaultScenes();

    Scene* GetMainScene() const;
    Scene* GetPersistentScene() const;

    void Step(const DeltaTime& dt);
    void Update(const DeltaTime& dt);

protected:

    Scene* m_mainScene;
    Scene* m_persistentScene;
};

ManagerScenes* GetScenes();

}   // namespace gugu
