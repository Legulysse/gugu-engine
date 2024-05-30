#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Misc/Pattern/Singleton.h"
#include "Gugu/System/Callback.h"
#include "Gugu/System/Types.h"
#include "Gugu/Debug/EngineStats.h"

#include <SFML/System/Time.hpp>

#include <string>
#include <vector>
#include <map>
#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ManagerAnimations;
    class ManagerInputs;
    class ManagerAudio;
    class ManagerNetwork;
    class ManagerResources;
    class ManagerVisualEffects;
    class ManagerScenes;
    class Application;
    class Renderer;
    class Window;
    class Scene;
    class LoggerEngine;
    class TraceGroup;
    class DeltaTime;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
struct Timer
{
    float tickDelay;
    uint32 maxTicks;

    float currentTime;
    uint32 ticks;

    Callback callback;
};

class Engine : public Singleton<Engine>
{
public:

    Engine();
    ~Engine();

    void Init(const EngineConfig& config);
    void Release();

    void RunApplication(Application* application);

    void RunMainLoop();
    void RunSingleLoop(const sf::Time& loopTime);
    void StopMainLoop();

    void SetLoopPause(bool pause);
    bool IsLoopPaused() const;
    void SetLoopSpeed(float speed);
    void ResetLoopSpeed();
    float GetLoopSpeed() const;

    void SetShowImGui(bool showImGui);
    bool IsImGuiVisible() const;

    void            SetApplication(Application* application);
    Application*    GetApplication() const;

    void            AddWindow       (Window* window);
    void            RemoveWindow    (Window* window);
    void            SetGameWindow   (Window* window);
    Window*         GetGameWindow   () const;
    
    bool            SetTimer    (const std::string& name, float delayMs, uint32 ticks, bool tickNow, const Callback& callback);
    void            ClearTimer  (const std::string& name);
    const Timer*    GetTimer    (const std::string& name) const;
    void            TickTimers  (const DeltaTime& dt);

    void            ComputeCommandLine(const std::string& commandLine);

    void                OnSceneReleased(Scene* scene);

    Renderer*           GetDefaultRenderer() const;

    ManagerInputs*      GetManagerInputs() const;
    ManagerAudio*       GetManagerAudio() const;
    ManagerNetwork*     GetManagerNetwork() const;
    ManagerResources*   GetManagerResources() const;
    ManagerAnimations*  GetManagerAnimations() const;
    ManagerVisualEffects* GetManagerVisualEffects() const;
    ManagerScenes*      GetManagerScenes() const;

    LoggerEngine*       GetLogEngine() const;
    TraceGroup*         GetTraceGroupMain() const;

    const EngineConfig& GetEngineConfig() const;

private:
    
    EngineConfig m_engineConfig;

    Renderer*               m_defaultRenderer;
    Window*                 m_gameWindow;
    std::vector<Window*>    m_windows;

    std::map<std::string, Timer*> m_timers;

    ManagerInputs*      m_managerInputs;
    ManagerAudio*       m_managerAudio;
    ManagerNetwork*     m_managerNetwork;
    ManagerResources*   m_managerResources;
    ManagerAnimations*  m_managerAnimations;
    ManagerVisualEffects* m_managerVisualEffects;
    ManagerScenes*      m_managerScenes;

    LoggerEngine*       m_logEngine;
    TraceGroup*         m_traceGroupMain;
    int                 m_traceLifetime;

    Application*        m_application;

    bool                m_stopLoop;
    sf::Time            m_timeSinceLastStep;
    bool                m_useSpeedMultiplier;
    float               m_speedMultiplier;
    bool                m_pauseLoop;
    sf::Time            m_injectTime;
    bool                m_showImGui;

    // Stats
    EngineStats         m_stats;
};

Engine* GetEngine();

}   // namespace gugu
