////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Engine.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Version.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Window/Window.h"

#include <SFML/System/Sleep.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Engine::Engine()
{
    // This constructor should stay empty.
    // Because it's a singleton, if a GetInstance() is called inside by another system but the constructor isn't finished,
    // the GetInstance will try to create an other instance (loop).
}

Engine::~Engine()
{
    // Because of the constructor problem, I prefer to let the destructor also empty.
}

void Engine::Init(const EngineConfig& config)
{
    //-- Init low-level stuff --//
    ResetRandSeed();

    //-- Init engine log and trace group --//
    m_logEngine = new LoggerEngine();
    m_logEngine->SetConsoleOutput(true, false);
    m_logEngine->SetFile("Engine.log");

    m_traceGroupMain = new TraceGroup;
    m_traceLifetime = 0;

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "Gugu::Engine Start");

    //-- Pre-compute config parameters --//
    EngineConfig computedConfig = config;

    NormalizePathSelf(computedConfig.pathAssets, true);
    NormalizePathSelf(computedConfig.pathScreenshots, true);

    //-- Init Engine core --//
    m_stopLoop = false;
    m_stepSpeed = 10;

    m_application = nullptr;
    m_renderer = nullptr;
    m_gameWindow = nullptr;

    //-- Init Managers --//
    m_managerResources = new ManagerResources;
    m_managerResources->Init(computedConfig);

    m_managerInputs = new ManagerInputs;
    m_managerInputs->Init(computedConfig);

    m_managerAudio = new ManagerAudio;
    m_managerAudio->Init(computedConfig);

    m_managerNetwork = new ManagerNetwork;

    m_managerAnimations = new ManagerAnimations;
    m_managerAnimations->Init(computedConfig);

    m_managerVisualEffects = new ManagerVisualEffects;
    m_managerVisualEffects->Init(computedConfig);

    m_managerScenes = new ManagerScenes;
    m_managerScenes->Init(computedConfig);

    //-- Init Default Renderer --//
    m_renderer = new Renderer;

    //-- Init Window --//
    if (computedConfig.gameWindow == EGameWindow::Sfml)
    {
        m_gameWindow = new Window;
    }

    if (m_gameWindow)
    {
        m_gameWindow->Create(computedConfig, true);
        m_gameWindow->SetRenderer(m_renderer);

        AddWindow(m_gameWindow);

        ImGui::SFML::Init(*m_gameWindow->GetSFRenderWindow());
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    }
}

void Engine::Release()
{
    ClearStdMap(m_timers);

    SafeDelete(m_application);

    if (m_gameWindow)
    {
        m_gameWindow = nullptr;

        ImGui::SFML::Shutdown();
    }

    ClearStdVector(m_windows);
    //SafeDelete(m_gameWindow);

    SafeDelete(m_renderer);

    m_managerScenes->Release();
    m_managerInputs->Release();
    m_managerVisualEffects->Release();
    m_managerAnimations->Release();
    m_managerAudio->Release();
    m_managerResources->Release();

    SafeDelete(m_managerScenes);
    SafeDelete(m_managerInputs);
    SafeDelete(m_managerVisualEffects);
    SafeDelete(m_managerAnimations);
    SafeDelete(m_managerAudio);
    SafeDelete(m_managerNetwork);
    SafeDelete(m_managerResources);

    SafeDelete(m_traceGroupMain);

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "Gugu::Engine Stop");
    SafeDelete(m_logEngine);

    Engine::DeleteInstance();
}

void Engine::RunApplication(Application* application)
{
    SetApplication(application);
    RunMainLoop();
    Release();
}

void Engine::RunMainLoop()
{
    m_stopLoop = false;

    if (m_application)
        m_application->AppStart();

    // Init loop clock.
    sf::Clock oClock;

    // Init loop variables.
    sf::Time dtLoop = sf::Time::Zero;   //Time (ms) since last Loop.
    m_dtSinceLastStep = DeltaTime(0);   //Time (ms) since last Step.

    // Loop !
    while (!m_stopLoop)
    {
        if (m_traceLifetime > 0)
        {
            if (!m_traceGroupMain->IsActive())
            {
                m_traceGroupMain->Start();
                m_stats.isTracing = true;
            }
            else
            {
                m_traceLifetime -= 1;
                if (m_traceLifetime <= 0)
                {
                    m_traceGroupMain->Stop();
                    m_traceLifetime = 0;
                    m_stats.isTracing = false;
                }
            }
        }

        GUGU_SCOPE_TRACE_MAIN("Engine Loop");

        // TODO: use microseconds as a base instead of milliseconds.
        dtLoop = oClock.restart();
        RunSingleLoop(DeltaTime(dtLoop.asMilliseconds()));

        // Safeguard if there is no render in the loop, to avoid using cpu and risking dt times of zero.
        if (m_windows.empty())
            sf::sleep(sf::milliseconds(16));
    }

    // Loop stop.
    m_managerNetwork->StopReceptionThread();

    if (m_application)
        m_application->AppStop();
}

void Engine::RunSingleLoop(const DeltaTime& dt)
{
    // TODO: rework the speed option, to handle values below 1ms/frame (I can use intervals with a dt=0).
    DeltaTime dtConstantStep((20));
    DeltaTime dtSpeedModulatedStep(2 * m_stepSpeed);    // Default step speed multiplier is 10, minimum is 1.

    sf::Clock clockStatLoop;
    sf::Clock clockStatSection;

    //-- Network Reception --//
    if (m_managerNetwork->IsListening())
    {
        //m_managerNetwork->StepReception();
        m_managerNetwork->ProcessWaitingPackets();
    }

    //-- Events --//
    {
        GUGU_SCOPE_TRACE_MAIN("Windows Events");
        clockStatSection.restart();

        //Window Events (will abort the Step if main window is closed !)
        for (size_t i = 0; i < m_windows.size(); ++i)
        {
            Window* pWindow = m_windows[i];

            bool bClosed = pWindow->ProcessEvents();
            if (bClosed)
            {
                //TODO: I currently avoid to delete Windows for crash safety (events, Elements), maybe I could handle this better
                //RemoveWindow(pWindow); //if used, need manual handling of ++i
                //SafeDelete(pWindow);

                bool isMainWindow = (pWindow == m_gameWindow);
                if (isMainWindow)
                {
                    GetEngine()->StopMainLoop();
                    return;
                }
            }
        }
    }

    //-- Step --//
    {
        GUGU_SCOPE_TRACE_MAIN("Step");
        clockStatSection.restart();

        //Step
        m_dtSinceLastStep += dt;

        //TODO: option to use a while to compensate lagging frames (warning : breakpoints fuck timers, so I need to gracefully recover) (warning : it breaks everything if fps is too low) 
        //TODO: option to ignore constantstep
        if (m_dtSinceLastStep >= dtConstantStep && m_managerNetwork->IsReadyForTurn())
        {
            TickTimers(dtSpeedModulatedStep);

            if (m_application)
                m_application->AppStep(dtSpeedModulatedStep);

            if (m_managerScenes)
                m_managerScenes->Step(dtSpeedModulatedStep);

            m_managerAnimations->Step(dtSpeedModulatedStep);

            for (size_t i = 0; i < m_windows.size(); ++i)
                m_windows[i]->Step(dtSpeedModulatedStep);

            m_dtSinceLastStep -= dtConstantStep;

            m_managerNetwork->SetTurnPlayed();

            // Step Stats
            m_stats.stepTimes.push_front(clockStatSection.getElapsedTime().asMilliseconds());
            if (m_stats.stepTimes.size() > m_stats.maxStatCount)
            {
                m_stats.stepTimes.pop_back();
            }
        }
        else
        {
            // Step Stats
            m_stats.stepTimes.push_front(-1);
            if (m_stats.stepTimes.size() > m_stats.maxStatCount)
            {
                m_stats.stepTimes.pop_back();
            }
        }
    }

    //-- Update --//
    {
        GUGU_SCOPE_TRACE_MAIN("Update");
        clockStatSection.restart();

        if (m_gameWindow)
        {
            ImGui::SFML::Update(*m_gameWindow->GetSFRenderWindow(), sf::Time(dt.GetSFTime()));
        }

        if (m_application)
            m_application->AppUpdate(dt);

        if (m_managerScenes)
            m_managerScenes->Update(dt);

        m_managerAnimations->Update(dt, m_stats);
        m_managerVisualEffects->Update(dt, m_stats);

        for (size_t i = 0; i < m_windows.size(); ++i)
            m_windows[i]->Update(dt);

        m_managerAudio->Update(dt);

        // Update Stats
        m_stats.updateTimes.push_front(clockStatSection.getElapsedTime().asMilliseconds());
        if (m_stats.updateTimes.size() > m_stats.maxStatCount)
        {
            m_stats.updateTimes.pop_back();
        }
    }

    //-- Network Thread --//
    m_managerNetwork->StartReceptionThread();

    //-- Render --//
    {
        GUGU_SCOPE_TRACE_MAIN("Render");
        clockStatSection.restart();

        // Render
        for (size_t i = 0; i < m_windows.size(); ++i)
            m_windows[i]->Render(dt, m_stats);

        // Render Stats
        m_stats.renderTimes.push_front(clockStatSection.getElapsedTime().asMilliseconds());
        if (m_stats.renderTimes.size() > m_stats.maxStatCount)
        {
            m_stats.renderTimes.pop_back();
        }

        // Display
        for (size_t i = 0; i < m_windows.size(); ++i)
            m_windows[i]->Display();
    }

    //m_managerNetwork->StopReceptionThread();
    //m_managerNetwork->Lock();
    //m_managerNetwork->Unlock();

    // Loop Stats
    m_stats.loopTimes.push_front(clockStatLoop.getElapsedTime().asMilliseconds());
    if (m_stats.loopTimes.size() > m_stats.maxStatCount)
    {
        m_stats.loopTimes.pop_back();
    }
}

void Engine::StopMainLoop()
{
    m_stopLoop = true;
}

void Engine::SetApplication(Application* application)
{
    m_application = application;
}

Application* Engine::GetApplication() const
{
    return m_application;
}

void Engine::AddWindow(Window* window)
{
    if (!window)
        return;

    m_windows.push_back(window);
}

void Engine::RemoveWindow(Window* window)
{
    if (!window)
        return;

    if (m_gameWindow == window)
        m_gameWindow = nullptr;

    auto iteFind = StdVectorFind(m_windows, window);
    if (iteFind != m_windows.end())
        m_windows.erase(iteFind);
}

void Engine::SetGameWindow(Window* window)
{
    m_gameWindow = window;
}

Window* Engine::GetGameWindow() const
{
    return m_gameWindow;
}

void Engine::ComputeCommandLine(const std::string& commandLine)
{
    std::string lowerCommandLine = commandLine;
    StdStringToLowerSelf(lowerCommandLine);

    //TODO: store commands history

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, StringFormat("CommandLine : {0}", lowerCommandLine));

    std::vector<std::string> vecTokens;
    StdStringSplit(lowerCommandLine, " ", vecTokens);

    if (!vecTokens.empty())
    {
        std::string strCommand = vecTokens[0];
        vecTokens.erase(vecTokens.begin());

        if (strCommand == "fps")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowFPS();
        }
        else if (strCommand == "stats")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowStats();
        }
        else if (strCommand == "bounds")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowBounds();
        }
        else if (strCommand == "trace")
        {
            m_traceLifetime = 10;
            if (!vecTokens.empty())
                FromString(vecTokens[0], m_traceLifetime);
        }
        else if (strCommand == "speed")
        {
            // Default step speed multiplier is 10, minimum is 1.
            if (!vecTokens.empty())
            {
                int speed = 10;
                if (FromString(vecTokens[0], speed))
                    m_stepSpeed = Max(1, speed);
            }
        }

        if (GetApplication())
            GetApplication()->ComputeCommandLine(strCommand, vecTokens);
    }
}

bool Engine::SetTimer(const std::string& name, uint32 delay, uint32 ticks, bool tickNow, Callback callback)
{
    if (!callback)
        return false;

    Timer* pNewTimer = new Timer;

    pNewTimer->currentTime = 0;
    pNewTimer->tickDelay = delay;

    pNewTimer->ticks = 0;
    pNewTimer->maxTicks = ticks;

    pNewTimer->callback = callback;

    if (tickNow)
    {
        pNewTimer->callback();

        if (ticks == 1)
        {
            SafeDelete(pNewTimer);
            return true;
        }

        if (ticks > 0)
            pNewTimer->ticks += 1;
    }

    ClearTimer(name);
    m_timers[name] = pNewTimer;

    return true;
}

void Engine::ClearTimer(const std::string& name)
{
   auto kvp = m_timers.find(name);
    if (kvp != m_timers.end())
    {
        SafeDelete(kvp->second);
        m_timers.erase(kvp);
    }
}

const Timer* Engine::GetTimer(const std::string& name) const
{
    auto kvp = m_timers.find(name);
    if (kvp != m_timers.end())
    {
        return kvp->second;
    }

    return nullptr;
}

void Engine::TickTimers(const DeltaTime& dt)
{
    for (auto kvp = m_timers.begin(); kvp != m_timers.end();)
    {
        Timer* pTimer = kvp->second;

        pTimer->currentTime += dt.ms();

        while (pTimer->currentTime >= pTimer->tickDelay)
        {
            pTimer->currentTime -= pTimer->tickDelay;

            if (pTimer->callback)
                pTimer->callback();

            if (pTimer->maxTicks == 0)  //infinite
                continue;

            pTimer->ticks += 1;
            if (pTimer->ticks >= pTimer->maxTicks)
            {
                SafeDelete(pTimer);
                break;
            }
        }

        if (!pTimer)
        {
            kvp = m_timers.erase(kvp);
        }
        else
        {
            ++kvp;
        }
    }
}

ManagerScenes* Engine::GetManagerScenes() const
{
    return m_managerScenes;
}

void Engine::OnSceneReleased(Scene* scene)
{
    for (size_t i = 0; i < m_windows.size(); ++i)
    {
        m_windows[i]->OnSceneReleased(scene);
    }
}

Renderer* Engine::GetDefaultRenderer() const
{
    return m_renderer;
}

ManagerInputs* Engine::GetManagerInputs() const
{
    return m_managerInputs;
}

ManagerAnimations* Engine::GetManagerAnimations() const
{
    return m_managerAnimations;
}

ManagerVisualEffects* Engine::GetManagerVisualEffects() const
{
    return m_managerVisualEffects;
}

ManagerAudio* Engine::GetManagerAudio() const
{
    return m_managerAudio;
}

ManagerNetwork* Engine::GetManagerNetwork() const
{
    return m_managerNetwork;
}

ManagerResources* Engine::GetManagerResources() const
{
    return m_managerResources;
}

LoggerEngine* Engine::GetLogEngine() const
{
    return m_logEngine;
}

TraceGroup* Engine::GetTraceGroupMain() const
{
    return m_traceGroupMain;
}

Engine* GetEngine()
{
    return Engine::GetInstance();
}

}   // namespace gugu
