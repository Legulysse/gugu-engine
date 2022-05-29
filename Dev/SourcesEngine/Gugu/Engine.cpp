////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Engine.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Version.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Core/DeltaTime.h"
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
    m_logEngine->SetFilePath("Engine.log");

    m_traceGroupMain = new TraceGroup;
    m_traceLifetime = 0;

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "Gugu::Engine Start");

    //-- Pre-compute config parameters --//
    m_engineConfig = config;

    NormalizePathSelf(m_engineConfig.pathAssets, true);
    NormalizePathSelf(m_engineConfig.pathScreenshots, true);

    //-- Init Engine core --//
    m_stopLoop = false;
    m_useSpeedMultiplier = false;
    m_speedMultiplier = 1.f;
    m_pauseLoop = false;
    m_injectTime = sf::Time::Zero;

    m_application = nullptr;
    m_renderer = nullptr;
    m_gameWindow = nullptr;

    //-- Init Managers --//
    m_managerResources = new ManagerResources;
    m_managerResources->Init(m_engineConfig);

    m_managerInputs = new ManagerInputs;
    m_managerInputs->Init(m_engineConfig);

    m_managerAudio = new ManagerAudio;
    m_managerAudio->Init(m_engineConfig);

    m_managerNetwork = new ManagerNetwork;

    m_managerAnimations = new ManagerAnimations;
    m_managerAnimations->Init(m_engineConfig);

    m_managerVisualEffects = new ManagerVisualEffects;
    m_managerVisualEffects->Init(m_engineConfig);

    m_managerScenes = new ManagerScenes;
    m_managerScenes->Init(m_engineConfig);

    //-- Init Default Renderer --//
    m_renderer = new DefaultRenderer;

    //-- Init Window --//
    if (m_engineConfig.gameWindow == EGameWindow::Sfml)
    {
        m_gameWindow = new Window;
    }

    if (m_gameWindow)
    {
        m_gameWindow->Create(m_engineConfig, true);
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
    sf::Time loopTime = sf::Time::Zero;   //Time since last Loop.
    m_timeSinceLastStep = sf::Time::Zero;   //Time since last Step.

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

        {
            GUGU_SCOPE_TRACE_MAIN("Engine Loop");

            loopTime = oClock.restart();
            RunSingleLoop(loopTime);

            // Safeguard if there is no render in the loop, to avoid using cpu and risking dt times of zero.
            if (m_windows.empty())
                sf::sleep(sf::milliseconds(16));
        }
    }

    // Loop stop.
    m_managerNetwork->StopReceptionThread();

    if (m_application)
        m_application->AppStop();
}

void Engine::RunSingleLoop(const sf::Time& loopTime)
{
    // handle speed multiplier if active.
    sf::Time updateTimeScaled = loopTime;
    sf::Time updateTimeUnscaled = loopTime;
    float updateDeltaScale = 1.f;

    if (m_useSpeedMultiplier)
    {
        updateTimeScaled *= m_speedMultiplier;
        updateDeltaScale = m_speedMultiplier;
    }

    // Handle time injection when using the pause mode.
    if (m_pauseLoop)
    {
        updateTimeScaled = m_injectTime;
        m_injectTime = sf::Time::Zero;
    }

    // Compute step time.
    sf::Time stepTimeScaled = updateTimeScaled;
    sf::Time stepTimeUnscaled = updateTimeUnscaled;
    float stepDeltaScale = updateDeltaScale;

    if (m_engineConfig.useConstantStep)
    {
        stepTimeScaled = sf::milliseconds(m_engineConfig.constantStepTime);
        stepTimeUnscaled = stepTimeScaled;
        stepDeltaScale = 1.f;
    }

    // Compute delta times.
    DeltaTime dt_step(stepTimeScaled, stepTimeUnscaled, stepDeltaScale);
    DeltaTime dt_update(updateTimeScaled, updateTimeUnscaled, updateDeltaScale);

    // Prepare clocks for stats.
    sf::Clock clockStatLoop;
    sf::Clock clockStatSection;

    // Log Frame Number
    m_logEngine->IncrementFrameNumber();

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
                if (isMainWindow && (!m_application || m_application->OnMainWindowCloseEvent()))
                {
                    // The main window will be automatically closed by the engine release.
                    GetEngine()->StopMainLoop();
                    return;
                }
            }
        }
    }

    //-- Step --//
    {
        GUGU_SCOPE_TRACE_MAIN("Steps");
        clockStatSection.restart();

        //Step
        bool allowNextStep = true;
        bool allowMultipleSteps = false;
        if (m_engineConfig.useConstantStep)
        {
            m_timeSinceLastStep += updateTimeScaled;
            allowNextStep = m_timeSinceLastStep >= stepTimeScaled;

            // If app is running with modulated speed, we may need to compute several steps per loop.
            // This will also happen if step time is lower than loop time, but for now I prefer to enforce a single step per loop.
            // I could add some settings to allow multiple steps per loop (maybe with a limit setting).
            allowMultipleSteps = m_useSpeedMultiplier;
        }

        bool stepHappened = false;
        bool multipleStepsHappened = false;

        //TODO: check m_managerNetwork->IsReadyForTurn() if running steps based on synchronized network clients.
        int stepCount = 0;
        while (allowNextStep)
        {
            GUGU_SCOPE_TRACE_MAIN_("Step", Step);

            TickTimers(dt_step);

            if (m_application)
                m_application->AppStep(dt_step);

            m_managerScenes->Step(dt_step);
            m_managerAnimations->Step(dt_step);

            m_managerNetwork->SetTurnPlayed();

            // Compute spent time.
            multipleStepsHappened = stepHappened;
            stepHappened = true;
            ++stepCount;

            allowNextStep = false;
            if (m_engineConfig.useConstantStep)
            {
                m_timeSinceLastStep -= stepTimeScaled;

                if (allowMultipleSteps)
                {
                    allowNextStep = m_timeSinceLastStep >= stepTimeScaled;
                }
                else
                {
                    // Since we dont compensate, no need to keep exceeding step lag.
                    m_timeSinceLastStep = Min(m_timeSinceLastStep, stepTimeScaled);
                }
            }

            // Safety in case the loop takes too much time.
            // Either because we took longer than the simulated stepTimeScaled, or because we took way too much time.
            if (allowNextStep
                && (clockStatSection.getElapsedTime() >= stepTimeScaled
                || clockStatSection.getElapsedTime() >= sf::milliseconds(500)))
            {
                m_timeSinceLastStep = sf::Time::Zero;
                allowNextStep = false;
            }
        }

        if (stepHappened)
        {
            // Step Stats
            m_stats.stepTimes.push_front(clockStatSection.getElapsedTime().asMilliseconds());
            if (m_stats.stepTimes.size() > m_stats.maxStatCount)
            {
                m_stats.stepTimes.pop_back();
            }

            m_stats.stepCount.push_front(stepCount);
            if (m_stats.stepCount.size() > m_stats.maxStatCount)
            {
                m_stats.stepCount.pop_back();
            }
            else
            {
                m_stats.stepCount.resize(m_stats.maxStatCount);
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

            m_stats.stepCount.push_front(-1);
            if (m_stats.stepCount.size() > m_stats.maxStatCount)
            {
                m_stats.stepCount.pop_back();
            }
            else
            {
                m_stats.stepCount.resize(m_stats.maxStatCount);
            }
        }
    }

    //-- Update --//
    {
        GUGU_SCOPE_TRACE_MAIN("Update");
        clockStatSection.restart();

        if (m_gameWindow)
        {
            ImGui::SFML::Update(*m_gameWindow->GetSFRenderWindow(), loopTime);
        }

        if (m_application)
            m_application->AppUpdate(dt_update);

        m_managerScenes->Update(dt_update);
        m_managerAnimations->Update(dt_update, m_stats);
        m_managerVisualEffects->Update(dt_update, m_stats);
        m_managerAudio->Update(dt_update);

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
            m_windows[i]->Render(loopTime, m_stats);

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
        else if (strCommand == "ruler")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowRuler();
        }
        else if (strCommand == "trace")
        {
            m_traceLifetime = 10;
            if (!vecTokens.empty())
                FromString(vecTokens[0], m_traceLifetime);
        }
        else if (strCommand == "speed")
        {
            m_useSpeedMultiplier = false;

            if (!vecTokens.empty() && vecTokens[0] != "1")
            {
                float speed = 1;
                if (FromString(vecTokens[0], speed))
                {
                    m_useSpeedMultiplier = true;
                    m_speedMultiplier = Max(0.f, speed);
                }
            }
        }
        else if (strCommand == "pause")
        {
            m_pauseLoop = !m_pauseLoop;
            m_injectTime = sf::Time::Zero;
        }
        else if (strCommand == "time")
        {
            if (!vecTokens.empty())
            {
                int time = 0;
                if (FromString(vecTokens[0], time))
                {
                    m_injectTime = sf::milliseconds(Max(0, time));
                }
            }
        }

        if (GetApplication())
            GetApplication()->ComputeCommandLine(strCommand, vecTokens);
    }
}

bool Engine::SetTimer(const std::string& name, uint32 delay, uint32 ticks, bool tickNow, const Callback& callback)
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
