#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Patterns/Singleton.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/System/Types.h"

#include <SFML/Graphics/Color.hpp>

#include <string>
#include <vector>
#include <map>
#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class ManagerInputs;
    class ManagerAudio;
    class ManagerNetwork;
    class ManagerResources;
    class Application;
    class Renderer;
    class Window;
    class World;
    class Level;
    class LoggerEngine;
    class TraceGroup;
    class Action;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace EGameWindow
{
    enum Type
    {
        None,
        Sfml,
    };
}

struct EngineConfig
{
    // Application
    std::string applicationName;
    std::string applicationIcon;

    // Resources
    bool useAssetsFullPaths;
    std::string pathAssets;
    std::string pathScreenshots;
    std::string defaultFont;
    std::string debugFont;
    bool defaultTextureSmooth;

    // Graphics
    EGameWindow::Type gameWindow;
    int windowWidth;
    int windowHeight;
    bool enableVerticalSync;
    int framerateLimit;
    sf::Color backgroundColor;
    bool maximizeWindow;

    // Audio
    int maxSoundTracks;     // Total tracks should not exceed 256
    int maxMusicTracks;     // Total tracks should not exceed 256

    // Debug
    bool showStats;
    bool showFPS;


    EngineConfig()
    {
        applicationName = "Game Project";
        applicationIcon = "";

        useAssetsFullPaths = false;
        pathAssets = "";
        pathScreenshots = "Screenshots";
        defaultFont = "";
        debugFont = "";
        defaultTextureSmooth = false;

        gameWindow = EGameWindow::Sfml;
        windowWidth = 800;
        windowHeight = 600;
        enableVerticalSync = false;
        framerateLimit = 60;
        backgroundColor = sf::Color(128, 128, 128, 255);
        maximizeWindow = false;

        maxSoundTracks = 240;   // Total tracks should not exceed 256
        maxMusicTracks = 16;    // Total tracks should not exceed 256

        showStats = false;
        showFPS = false;
    }
};

struct EngineStats
{
    std::list<int> loopTimes;
    std::list<int> stepTimes;
    bool isTracing = false;
};

struct Timer
{
    uint32 tickDelay;
    uint32 maxTicks;

    uint32 currentTime;
    uint32 ticks;

    Action* action;

    Timer();
    ~Timer();
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
    void RunSingleLoop(const DeltaTime& dt);
    void StopMainLoop();

    void            SetApplication(Application* application);
    Application*    GetApplication() const;

    void            AddWindow       (Window* window);
    void            RemoveWindow    (Window* window);
    void            SetGameWindow   (Window* window);
    Window*         GetGameWindow   () const;
    
    bool            SetTimer    (const std::string& name, uint32 delay, uint32 ticks, bool tickNow, Action* action);
    void            ClearTimer  (const std::string& name);
    const Timer*    GetTimer    (const std::string& name) const;
    void            TickTimers  (const DeltaTime& dt);

    void            ComputeCommandLine(const std::string& commandLine);

    World*              GetWorld() const;
    void                OnLevelReleased(Level* level);

    Renderer*           GetDefaultRenderer() const;

    ManagerInputs*      GetManagerInputs() const;
    ManagerAudio*       GetManagerAudio() const;
    ManagerNetwork*     GetManagerNetwork() const;
    ManagerResources*   GetManagerResources() const;

    LoggerEngine*       GetLogEngine() const;
    TraceGroup*         GetTraceGroupMain() const;

private:
    
    Renderer*               m_renderer;
    Window*                 m_gameWindow;
    std::vector<Window*>    m_windows;

    std::map<std::string, Timer*> m_timers;

    World*              m_world;

    ManagerInputs*      m_managerInputs;
    ManagerAudio*       m_managerAudio;
    ManagerNetwork*     m_managerNetwork;
    ManagerResources*   m_managerResources;

    LoggerEngine*       m_logEngine;
    TraceGroup*         m_traceGroupMain;
    int                 m_traceLifetime;

    Application*        m_application;

    bool                m_stopLoop;
    DeltaTime           m_dtSinceLastStep;
    int                 m_stepSpeed;        // Default step speed multiplier is 10, minimum is 1.

    // Stats
    EngineStats         m_stats;
};

Engine* GetEngine();
LoggerEngine* GetLogEngine();

}   // namespace gugu
