////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Engine.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/EngineVersion.h"
#include "Gugu/Core/Application.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/VisualEffects/ManagerVisualEffects.h"
#include "Gugu/Scene/ManagerScenes.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Path.h"
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
    : m_managerInputs(nullptr)
    , m_managerAudio(nullptr)
    , m_managerNetwork(nullptr)
    , m_managerResources(nullptr)
    , m_managerAnimations(nullptr)
    , m_managerVisualEffects(nullptr)
    , m_managerScenes(nullptr)
    , m_logEngine(nullptr)
    , m_traceGroupMain(nullptr)
    , m_traceLifetime(0)
    , m_application(nullptr)
    , m_gameWindow(nullptr)
    , m_defaultRenderer(nullptr)
    , m_stopLoop(false)
    , m_timeSinceLastStep(sf::Time::Zero)
    , m_useSpeedMultiplier(false)
    , m_speedMultiplier(1.f)
    , m_pauseLoop(false)
    , m_injectTime(sf::Time::Zero)
    , m_showImGui(false)
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

    NormalizePathSelf(m_engineConfig.pathAssets);
    NormalizePathSelf(m_engineConfig.pathScreenshots);

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
    m_defaultRenderer = new DefaultRenderer;

    //-- Init Window --//
    if (m_engineConfig.gameWindow == EGameWindow::Sfml)
    {
        m_gameWindow = new Window;
    }

    if (m_gameWindow)
    {
        bool hostImGui = true;

        m_gameWindow->Create(m_engineConfig, hostImGui);
        m_gameWindow->SetRenderer(m_defaultRenderer);

        AddWindow(m_gameWindow);

        if (hostImGui)
        {
            ImGui::SFML::Init(*m_gameWindow->GetSFRenderWindow());

            // ImGui config flags :
            // - ImGuiConfigFlags_NoMouseCursorChange : I handle cursors in Window Update.
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

            m_showImGui = m_engineConfig.showImGui;
        }
    }
}

void Engine::Release()
{
    ClearStdMap(m_timers);

    SafeDelete(m_application);

    if (m_gameWindow && m_gameWindow->IsHostingImGui())
    {
        ImGui::SFML::Shutdown();
    }

    m_gameWindow = nullptr;
    ClearStdVector(m_windows);

    SafeDelete(m_defaultRenderer);

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
    sf::Clock loopClock;

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

            loopTime = loopClock.restart();
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
    sf::Time updateTimeUnscaled = sf::microseconds(Min<int64>(m_engineConfig.maxUpdateDeltaTimeMs * 1000, loopTime.asMicroseconds()));
    sf::Time updateTimeScaled = updateTimeUnscaled;
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
    sf::Time stepTimeUnscaled = updateTimeUnscaled;
    sf::Time stepTimeScaled = updateTimeScaled;
    float stepDeltaScale = updateDeltaScale;

    if (m_engineConfig.useConstantStep)
    {
        stepTimeUnscaled = sf::milliseconds(m_engineConfig.constantStepTimeMs);
        stepTimeScaled = stepTimeUnscaled;
        stepDeltaScale = 1.f;
    }

    // Compute delta times.
    DeltaTime dt_update(updateTimeScaled, updateTimeUnscaled, updateDeltaScale);
    DeltaTime dt_step(stepTimeScaled, stepTimeUnscaled, stepDeltaScale);

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
            Window* window = m_windows[i];

            bool closed = window->ProcessEvents();
            if (closed)
            {
                //TODO: I currently avoid to delete Windows for crash safety (events, Elements), maybe I could handle this better
                //RemoveWindow(window); //if used, need manual handling of ++i
                //SafeDelete(window);

                bool isMainWindow = (window == m_gameWindow);
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
            m_stats.stepTimes.push_front(static_cast<float>(static_cast<double>(clockStatSection.getElapsedTime().asMicroseconds()) / 1000.0));
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
            m_stats.stepTimes.push_front(-1.f);
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

        if (m_application)
            m_application->AppUpdate(dt_update);

        m_managerScenes->Update(dt_update);
        m_managerAnimations->Update(dt_update, m_stats);
        m_managerVisualEffects->Update(dt_update, m_stats);
        m_managerAudio->Update(dt_update);

        if (m_application)
            m_application->AppLateUpdate(dt_update);

        m_managerScenes->LateUpdate(dt_update);

        if (m_gameWindow && m_gameWindow->IsHostingImGui())
        {
            GUGU_SCOPE_TRACE_MAIN_("Update ImGui", IMGUI);

            ImGui::SFML::Update(*m_gameWindow->GetSFRenderWindow(), loopTime);

            if (m_showImGui)
            {
                if (m_application)
                    m_application->AppUpdateImGui(dt_update);

                m_managerScenes->UpdateImGui(dt_update);
            }
        }

        // Update Stats
        m_stats.updateTimes.push_front(static_cast<float>(static_cast<double>(clockStatSection.getElapsedTime().asMicroseconds()) / 1000.0));
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
        m_stats.renderTimes.push_front(static_cast<float>(static_cast<double>(clockStatSection.getElapsedTime().asMicroseconds()) / 1000.0));
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
    m_stats.loopTimes.push_front(static_cast<float>(static_cast<double>(clockStatLoop.getElapsedTime().asMicroseconds()) / 1000.0));
    if (m_stats.loopTimes.size() > m_stats.maxStatCount)
    {
        m_stats.loopTimes.pop_back();
    }
}

void Engine::StopMainLoop()
{
    m_stopLoop = true;
}

void Engine::SetLoopPause(bool pause)
{
    m_pauseLoop = pause;
    m_injectTime = sf::Time::Zero;
}

bool Engine::IsLoopPaused() const
{
    return m_pauseLoop;
}

void Engine::SetLoopSpeed(float speed)
{
    if (!ApproxEqual(speed, 1.f, math::Epsilon6))
    {
        m_useSpeedMultiplier = true;
        m_speedMultiplier = Max(0.f, speed);
    }
    else
    {
        ResetLoopSpeed();
    }
}

void Engine::ResetLoopSpeed()
{
    m_useSpeedMultiplier = false;
    m_speedMultiplier = 1.f;
}

float Engine::GetLoopSpeed() const
{
    return m_speedMultiplier;
}

void Engine::SetShowImGui(bool showImGui)
{
    m_showImGui = showImGui;
}

bool Engine::IsImGuiVisible() const
{
    return m_showImGui;
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

    auto it = StdVectorFind(m_windows, window);
    if (it != m_windows.end())
        m_windows.erase(it);
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

    std::vector<std::string> tokens;
    StdStringSplit(lowerCommandLine, " ", tokens);

    if (!tokens.empty())
    {
        std::string command = tokens[0];
        tokens.erase(tokens.begin());

        if (command == "fps")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowFPS();
        }
        else if (command == "stats")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowStats();
        }
        else if (command == "bounds")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowBounds();
        }
        else if (command == "ruler")
        {
            if (m_gameWindow)
                m_gameWindow->ToggleShowRuler();
        }
        else if (command == "trace")
        {
            m_traceLifetime = 10;
            if (!tokens.empty())
                FromString(tokens[0], m_traceLifetime);
        }
        else if (command == "speed")
        {
            bool reset = true;
            if (!tokens.empty() && tokens[0] != "1")
            {
                float speed = 1.f;
                if (FromString(tokens[0], speed))
                {
                    SetLoopSpeed(speed);
                    reset = false;
                }
            }

            if (reset)
            {
                ResetLoopSpeed();
            }
        }
        else if (command == "pause")
        {
            SetLoopPause(!m_pauseLoop);
        }
        else if (command == "time")
        {
            if (!tokens.empty())
            {
                int time = 0;
                if (FromString(tokens[0], time))
                {
                    m_injectTime = sf::milliseconds(Max(0, time));
                }
            }
        }
        else if (command == "imgui")
        {
            SetShowImGui(!m_showImGui);
        }

        if (GetApplication())
            GetApplication()->ComputeCommandLine(command, tokens);
    }
}

bool Engine::SetTimer(const std::string& name, float delayMs, uint32 ticks, bool tickNow, const Callback& callback)
{
    if (!callback)
        return false;

    Timer* newTimer = new Timer;

    newTimer->currentTime = 0;
    newTimer->tickDelay = delayMs;

    newTimer->ticks = 0;
    newTimer->maxTicks = ticks;

    newTimer->callback = callback;

    if (tickNow)
    {
        newTimer->callback();

        if (ticks == 1)
        {
            SafeDelete(newTimer);
            return true;
        }

        if (ticks > 0)
            newTimer->ticks += 1;
    }

    ClearTimer(name);
    m_timers[name] = newTimer;

    return true;
}

void Engine::ClearTimer(const std::string& name)
{
    auto it = m_timers.find(name);
    if (it != m_timers.end())
    {
        SafeDelete(it->second);
        m_timers.erase(it);
    }
}

const Timer* Engine::GetTimer(const std::string& name) const
{
    auto it = m_timers.find(name);
    if (it != m_timers.end())
    {
        return it->second;
    }

    return nullptr;
}

void Engine::TickTimers(const DeltaTime& dt)
{
    for (auto it = m_timers.begin(); it != m_timers.end();)
    {
        Timer* timer = it->second;

        timer->currentTime += dt.ms();

        while (ApproxSuperiorOrEqual(timer->currentTime, timer->tickDelay, math::Epsilon6))
        {
            timer->currentTime -= timer->tickDelay;

            if (timer->callback)
                timer->callback();

            if (timer->maxTicks == 0)  //infinite
                continue;

            timer->ticks += 1;
            if (timer->ticks >= timer->maxTicks)
            {
                SafeDelete(timer);
                break;
            }
        }

        if (!timer)
        {
            it = m_timers.erase(it);
        }
        else
        {
            ++it;
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
    return m_defaultRenderer;
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

const EngineConfig& Engine::GetEngineConfig() const
{
    return m_engineConfig;
}

Engine* GetEngine()
{
    return Engine::GetInstance();
}

}   // namespace gugu
