////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Engine.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Version.h"

#include "Gugu/Inputs/ManagerInputs.h"
#include "Gugu/Audio/ManagerAudio.h"
#include "Gugu/Network/ManagerNetwork.h"
#include "Gugu/Resources/ManagerResources.h"

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"
#include "Gugu/Misc/Action.h"

#include "Gugu/Debug/Logger.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/Core/Application.h"

#include "Gugu/Window/Renderer.h"
#include "Gugu/Window/Window.h"

#include "Gugu/World/World.h"

#include <SFML/System/Sleep.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Timer::Timer()
{
}

Timer::~Timer()
{
    SafeDelete(action);
}

Engine::Engine()
{
    //This constructor should stay empty.
    //Because it's a singleton, if a GetInstance() is called by someone but the constructor isn't finished,
    //the GetInstance will try to create an other instance (loop).
}

Engine::~Engine()
{
    //Because of the constructor problem, I prefer to let the destructor also empty.
}

void Engine::Init(const EngineConfig& config)
{
    //-- Init low-level stuff

    ResetRandSeed();


    //-- Init engine log and trace group

    m_logEngine = new LoggerEngine();
    m_logEngine->SetFile("Engine.log");

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "Gugu::Engine Start");

    m_traceGroupMain = new TraceGroup;
    m_traceLifetime = 0;


    //-- Pre-compute config parameters

    EngineConfig computedConfig = config;

    NormalizePathSelf(computedConfig.pathAssets, true);
    NormalizePathSelf(computedConfig.pathScreenshots, true);


    //-- Init Engine core & Main Window

    m_stopLoop = false;
    m_stepSpeed = 10;

    m_application = nullptr;
    m_renderer = nullptr;
    m_gameWindow = nullptr;
    m_world = nullptr;


    //-- Init Managers

    m_managerResources = new ManagerResources;
    m_managerResources->Init(computedConfig);

    m_managerInputs = new ManagerInputs;
    m_managerInputs->Init(computedConfig);

    m_managerAudio = new ManagerAudio;
    m_managerAudio->Init(computedConfig);

    m_managerNetwork = new ManagerNetwork;


    //-- Init Default Renderer

    m_renderer = new Renderer;


    //-- Init World

    m_world = new World;   //TODO: Should I really create a default World ?
    m_world->ResetWorld();


    //-- Init Window

    if (computedConfig.gameWindow == EGameWindow::Sfml)
        m_gameWindow = new Window;

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

    ClearStdVector(m_windows);
    //SafeDelete(m_gameWindow);

    SafeDelete(m_world);
    SafeDelete(m_renderer);

    m_managerInputs->Release();
    m_managerAudio->Release();
    m_managerResources->Release();

    SafeDelete(m_managerInputs);
    SafeDelete(m_managerAudio);
    SafeDelete(m_managerNetwork);
    SafeDelete(m_managerResources);

    SafeDelete(m_traceGroupMain);

    ImGui::SFML::Shutdown();

    GetLogEngine()->Print(ELog::Info, ELogEngine::Engine, "Gugu::Engine Stop");
    SafeDelete(m_logEngine);

    DeleteInstance();
}

void Engine::StartLooping()
{
    m_stopLoop = false;

    if (m_application)
        m_application->AppStart();

    //Init loop clock
    sf::Clock oClock;

    //Init loop variables
    sf::Time dtLoop = sf::Time::Zero;   //Time (ms) since last Loop
    m_dtSinceLastStep = DeltaTime(0);   //Time (ms) since last Step

    //Loop !
    while (!m_stopLoop)
    {
        if (m_traceLifetime > 0)
        {
            if (!m_traceGroupMain->IsActive())
            {
                m_traceGroupMain->Start();
            }
            else
            {
                m_traceLifetime -= 1;
                if (m_traceLifetime <= 0)
                {
                    m_traceGroupMain->Stop();
                    m_traceLifetime = 0;
                }
            }
        }

        GUGU_SCOPE_TRACE_MAIN("Engine Loop");

        dtLoop = oClock.restart();
        RunSingleLoop(DeltaTime(dtLoop.asMilliseconds()));

        //Safeguard if there is no render in the loop, to avoid using cpu and risking dt times of zero
        if (m_windows.empty())
            sf::sleep(sf::milliseconds(16));
    }

    m_managerNetwork->StopReceptionThread();

    if (m_application)
        m_application->AppStop();
}

void Engine::RunSingleLoop(const DeltaTime& dt)
{
    // TODO: rework the speed option, to handle values below 1ms/frame (I can use intervals with a dt=0).
    DeltaTime dtConstantStep((20));
    DeltaTime dtSpeedModulatedStep(2 * m_stepSpeed);    // Default step speed multiplier is 10, minimum is 1.

    uint32 statMaxNbValues = 150;
    sf::Clock clockStatLoop;
    sf::Clock clockStatSteps;

    //Network Step
    if (m_managerNetwork->IsListening())
    {
        //m_managerNetwork->StepReception();
        m_managerNetwork->ProcessWaitingPackets();
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Windows Events");
        clockStatSteps.restart();

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
                    GetEngine()->StopLooping();
                    return;
                }
            }
        }
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Step");
        clockStatSteps.restart();

        //Step
        m_dtSinceLastStep += dt;

        //TODO: option to use a while to compensate lagging frames (warning : breakpoints fuck timers, so I need to gracefully recover) (warning : it breaks everything if fps is too low) 
        //TODO: option to ignore constantstep
        if (m_dtSinceLastStep >= dtConstantStep && m_managerNetwork->IsReadyForTurn())
        {
            TickTimers(dtSpeedModulatedStep);

            if (m_application)
                m_application->AppStep(dtSpeedModulatedStep);

            if (m_world)
                m_world->Step(dtSpeedModulatedStep);

            for (size_t i = 0; i < m_windows.size(); ++i)
                m_windows[i]->Step(dtSpeedModulatedStep);

            m_dtSinceLastStep -= dtConstantStep;

            m_managerNetwork->SetTurnPlayed();

            // Step Stats
            m_stats.stepTimes.push_front(clockStatSteps.getElapsedTime().asMilliseconds());
            if (m_stats.stepTimes.size() > statMaxNbValues)
            {
                m_stats.stepTimes.pop_back();
            }
        }
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Update");
        clockStatSteps.restart();

        if (m_gameWindow)
        {
            ImGui::SFML::Update(*m_gameWindow->GetSFRenderWindow(), sf::Time(dt));
        }

        //Update
        if (m_application)
            m_application->AppUpdate(dt);

        if (m_world)
            m_world->Update(dt);

        for (size_t i = 0; i < m_windows.size(); ++i)
            m_windows[i]->Update(dt);

        //Audio
        m_managerAudio->Update(dt);
    }

    //Network
    m_managerNetwork->StartReceptionThread();

    m_stats.isTracing = m_traceGroupMain->IsActive();

    {
        GUGU_SCOPE_TRACE_MAIN("Render");

        //Render (Note for Editor : For now, Render here instead of Qt draw event)
        for (size_t i = 0; i < m_windows.size(); ++i)
            m_windows[i]->Refresh(dt, m_stats);
    }

    //m_managerNetwork->StopReceptionThread();
    //m_managerNetwork->Lock();
    //m_managerNetwork->Unlock();

    // Loop Stats
    m_stats.loopTimes.push_front(clockStatLoop.getElapsedTime().asMilliseconds());
    if (m_stats.loopTimes.size() > statMaxNbValues)
    {
        m_stats.loopTimes.pop_back();
    }
}

void Engine::StopLooping()
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

bool Engine::SetTimer(const std::string& name, uint32 delay, uint32 ticks, bool tickNow, Action* action)
{
    if (!action)
        return false;

    Timer* pNewTimer = new Timer;

    pNewTimer->currentTime = 0;
    pNewTimer->tickDelay = delay;

    pNewTimer->ticks = 0;
    pNewTimer->maxTicks = ticks;

    pNewTimer->action = action;

    if (tickNow)
    {
        pNewTimer->action->Call();

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

            if (pTimer->action)
                pTimer->action->Call();

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

World* Engine::GetWorld() const
{
    return m_world;
}

void Engine::OnLevelReleased(Level* level)
{
    for (size_t i = 0; i < m_windows.size(); ++i)
        m_windows[i]->OnLevelReleased(level);
}

Renderer* Engine::GetDefaultRenderer() const
{
    return m_renderer;
}

ManagerInputs* Engine::GetManagerInputs() const
{
    return m_managerInputs;
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
    return Singleton<Engine>::GetInstance();
}

LoggerEngine* GetLogEngine()
{
    return GetEngine()->GetLogEngine();
}

}   // namespace gugu
