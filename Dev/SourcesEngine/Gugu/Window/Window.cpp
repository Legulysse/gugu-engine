////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Window.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Resources/ManagerResources.h"

#include "Gugu/Window/Renderer.h"
#include "Gugu/Events/HandlerEvents.h"
#include "Gugu/Window/Camera.h"

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"

#include "Gugu/World/Level.h"

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include "Gugu/Debug/Trace.h"

#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <thread>           // Used for async screenshot save.
#if defined(GUGU_OS_WIN32)
    #include <windows.h>    // Used for maximizing the window.
#endif

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Window::Window()
{
    m_sfWindow     = nullptr;
    m_renderer     = nullptr;

    m_hostImGui = false;

    m_mainCamera   = nullptr;
    m_handlerEvents = nullptr;

    m_rootNode     = nullptr;
    m_rootUINode        = nullptr;
    m_consoleNode  = nullptr;
    m_consoleTextEntry = nullptr;
    m_mouseNode    = nullptr;

    m_systemMouseVisible = true;
    m_mouseVisible = false;
    m_windowFocused = false;

    m_showStats        = false;
    m_showFPS           = false;

    m_backgroundColor = sf::Color(128,128,128,255);

    m_showBounds = false;
}

Window::~Window()
{
    SafeDelete(m_handlerEvents);

    m_statFrameTimes.clear();
    m_sfWindow->close();

    m_levelBindings.clear();

    ClearStdVector(m_cameras);
    SafeDelete(m_mainCamera);
    SafeDelete(m_rootNode);
    SafeDelete(m_sfWindow);
}

sf::RenderWindow* Window::Create(const EngineConfig& config, bool hostImGui)
{
    m_sfWindow = new sf::RenderWindow;
    m_hostImGui = hostImGui;

    //Windows Settings
    sf::ContextSettings Settings;
    Settings.depthBits         = 24; // Request a 24 bits depth buffer
    Settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
    Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing

    //Create main window
    m_sfWindow->create(sf::VideoMode(config.windowWidth, config.windowHeight, 32), config.applicationName,/* sf::Style::Default */ sf::Style::Resize | sf::Style::Close, Settings);
    m_sfWindow->setFramerateLimit(config.framerateLimit);
    m_sfWindow->setVerticalSyncEnabled(config.enableVerticalSync);

#if defined(GUGU_OS_WIN32)
    if (config.maximizeWindow)
    {
        ::ShowWindow(m_sfWindow->getSystemHandle(), SW_MAXIMIZE);
    }
#endif

    if (GetResources()->HasResource(config.applicationIcon))
    {
        sf::Image oImgIcon;
        oImgIcon.loadFromFile(GetResources()->GetResourcePathName(config.applicationIcon));

        // TODO: maybe I should use the actual texture size ?
        m_sfWindow->setIcon(16, 16, oImgIcon.getPixelsPtr());
    }

    Init(m_sfWindow, config);

    return m_sfWindow;
}

void Window::Init(sf::RenderWindow* _pSFWindow, const EngineConfig& config)
{
    m_sfWindow = _pSFWindow;
    m_backgroundColor = config.backgroundColor;

    m_sfWindow->setKeyRepeatEnabled(false);
    m_sfWindow->resetGLStates();

    //Rendering
    m_mainCamera = new Camera;
    m_mainCamera->SetWindow(this);

    //Handlers
    m_handlerEvents = new HandlerEvents;

    //Root node
    m_rootNode = new Element;
    m_rootNode->SetSize((float)config.windowWidth, (float)config.windowHeight);

    //Gui Node
    m_rootUINode = m_rootNode->AddChild<Element>();
    //m_pUINode->SetExtendViewOnResize(true);
    //m_pUINode->SetSize((float)config.windowWidth, (float)config.windowHeight);
    m_rootUINode->SetUnifiedSize(UDim2::SIZE_FULL);

    //Mouse Node
    m_mouseNode = m_rootNode->AddChild<ElementSprite>();
    //m_pMouseNode->SetUnifiedSize(UDim2::USIZE_FULL);
    m_mouseNode->SetVisible(false);

    //Console Node
    m_consoleNode = m_rootNode->AddChild<Element>();
    //m_pConsoleNode->SetExtendViewOnResize(true);
    //m_pConsoleNode->SetSize((float)config.windowWidth, (float)config.windowHeight);
    m_consoleNode->SetUnifiedSize(UDim2::SIZE_FULL);
    m_consoleNode->SetVisible(false);

    sf::Image oImgConsole;
    oImgConsole.create(8, 8, sf::Color(0, 0, 0, 150));

    Texture* pTextureConsole = GetResources()->GetCustomTexture("GuguConsoleBackground");   //TODO: Prepare custom textures in the manager beforehand
    sf::Texture* pSFTextureConsole = pTextureConsole->GetSFTexture();
    pSFTextureConsole->create(8, 8);
    pSFTextureConsole->update(oImgConsole);

    Font* pFont = GetResources()->GetDebugFont();

    ElementSprite* pConsoleBackground = m_consoleNode->AddChild<ElementSprite>();
    pConsoleBackground->SetTexture(pTextureConsole);
    pConsoleBackground->SetUnifiedSize(UDim2(1.f, 0.f, 0.f, 200.f));

    m_consoleTextEntry = pConsoleBackground->AddChild<ElementText>();
    m_consoleTextEntry->SetResizeRule(ETextResizeRule::FixedSize);
    m_consoleTextEntry->SetFont(pFont);
    m_consoleTextEntry->SetText("");
    m_consoleTextEntry->SetMultiline(false);
    //m_pConsoleTextEntry->SetEditable(true);
    m_consoleTextEntry->SetFontColor(sf::Color(220, 220, 220));
    m_consoleTextEntry->SetUnifiedPosition(UDim2::POSITION_BOTTOM_LEFT);
    m_consoleTextEntry->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    m_consoleTextEntry->SetUnifiedSize(UDim2(1.f, 0.f, 0.f, 30.f));
    m_consoleTextEntry->SetOnValidate(std::bind(&Window::OnConsoleCommandValidated, this));

    // Stats
    m_showStats = config.showStats;
    m_showFPS = config.showFPS;

    // Finalize
    ComputeSize(m_sfWindow->getSize().x, m_sfWindow->getSize().y);
}

void Window::Step(const DeltaTime& dt)
{
    m_rootNode->Step(dt);
}

void Window::Update(const DeltaTime& dt)
{
    m_rootNode->Update(dt);
}

void Window::SetRenderer(Renderer* _pRenderer)
{
    m_renderer = _pRenderer;
}

Renderer* Window::GetRenderer() const
{
    return m_renderer;
}

Camera* Window::CreateCamera()
{
    return AddCamera(new Camera);
}

Camera* Window::AddCamera(Camera* _pCamera)
{
    if (!_pCamera || StdVectorContains(m_cameras, _pCamera))
        return nullptr;

    _pCamera->SetWindow(this);
    _pCamera->ComputeViewSize();
    m_cameras.push_back(_pCamera);

    return _pCamera;
}

void Window::RemoveCamera(Camera* _pCamera)
{
    if (!_pCamera)
        return;

    StdVectorRemove(m_cameras, _pCamera);
}

void Window::DeleteCamera(Camera* _pCamera)
{
    if (!_pCamera)
        return;

    RemoveCamera(_pCamera);
    SafeDelete(_pCamera);
}

void Window::DeleteAllCameras()
{
    ClearStdVector(m_cameras);
}

Camera* Window::GetCamera(size_t _iIndex) const
{
    if (_iIndex >= 0 && _iIndex < m_cameras.size())
        return m_cameras[_iIndex];
    return nullptr;
}

Camera* Window::GetMainCamera() const
{
    return m_mainCamera;
}

void Window::BindLevel(Level* _pLevel)
{
    BindLevel(_pLevel, m_mainCamera, m_renderer);
}

void Window::BindLevel(Level* _pLevel, Camera* _pCamera)
{
    BindLevel(_pLevel, _pCamera, m_renderer);
}

void Window::BindLevel(Level* _pLevel, Renderer* _pRenderer)
{
    BindLevel(_pLevel, m_mainCamera, _pRenderer);
}

void Window::BindLevel(Level* _pLevel, Camera* _pCamera, Renderer* _pRenderer)
{
    if (!_pLevel || !_pCamera || !_pRenderer)
        return;

    _pCamera->SetLevel(_pLevel);

    LevelBinding kBinding;
    kBinding.level = _pLevel;
    kBinding.camera = _pCamera;
    kBinding.renderer = _pRenderer;
    m_levelBindings.push_back(kBinding);
}

void Window::OnLevelReleased(Level* _pLevel)
{
    auto iteBinding = m_levelBindings.begin();
    while (iteBinding != m_levelBindings.end())
    {
        if (iteBinding->level == _pLevel)
        {
            //TODO: flag auto-delete on Camera ?
            // I cant delete cameras like that, I have to check if they are in used and if its not the MainCamera
            //Camera* pCamera = iteBinding->m_pCamera;
            //StdVectorRemove(m_vecCameras, pCamera);
            //SafeDelete(pCamera);

            iteBinding = m_levelBindings.erase(iteBinding);
        }
        else
        {
            ++iteBinding;
        }
    }
}

void Window::Render(const DeltaTime& dt, const EngineStats& engineStats)
{
    FrameInfos kFrameInfos;
    kFrameInfos.showBounds = m_showBounds;
    kFrameInfos.defaultBoundsShape.setOutlineThickness(1.f);
    kFrameInfos.defaultBoundsShape.setOutlineColor(sf::Color::Magenta);
    kFrameInfos.defaultBoundsShape.setFillColor(sf::Color::Transparent);

    {
        GUGU_SCOPE_TRACE_MAIN("Clear");

        m_sfWindow->setActive();
        m_sfWindow->clear(m_backgroundColor);
    }

    sf::Clock kRenderClock;

    {
        GUGU_SCOPE_TRACE_MAIN("Levels");

        //Render Levels
        for (size_t i = 0; i < m_levelBindings.size(); ++i)
        {
            m_levelBindings[i].renderer->RenderLevel(kFrameInfos, this, m_levelBindings[i].camera, m_levelBindings[i].level);
        }
    }

    {
        GUGU_SCOPE_TRACE_MAIN("UI");

        //Handle Mouse visibility
        bool isSystemMouseWantedVisible = m_systemMouseVisible;
        bool isMouseWantedVisible = m_mouseVisible;

        if (!m_windowFocused)
        {
            isSystemMouseWantedVisible = true;
            isMouseWantedVisible = false;
        }

        if (m_hostImGui && ImGui::GetIO().WantCaptureMouse)
        {
            isSystemMouseWantedVisible = false;
            isMouseWantedVisible = false;
            ImGui::GetIO().MouseDrawCursor = true;
        }
        else
        {
            ImGui::GetIO().MouseDrawCursor = false;
        }

        m_sfWindow->setMouseCursorVisible(isSystemMouseWantedVisible);
        m_mouseNode->SetVisible(isMouseWantedVisible);

        //Render Window UI
        m_rootNode->SortOnZIndex();

        if (m_renderer)
            m_renderer->RenderWindow(kFrameInfos, this, m_mainCamera);
    }

    if (m_hostImGui)
    {
        GUGU_SCOPE_TRACE_MAIN("ImGui");

        ImGui::SFML::Render(*m_sfWindow);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Stats");

        //Stats
        if (m_showStats)
            DrawStats(kFrameInfos, DeltaTime(kRenderClock.getElapsedTime()), dt, engineStats);
        else if (m_showFPS)
            DrawFPS(dt);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Console");

        //Console
        sf::FloatRect kViewport;
        kViewport.left = m_sfWindow->getView().getCenter().x - m_sfWindow->getView().getSize().x / 2.f;
        kViewport.top = m_sfWindow->getView().getCenter().y - m_sfWindow->getView().getSize().y / 2.f;
        kViewport.width = m_sfWindow->getView().getSize().x;
        kViewport.height = m_sfWindow->getView().getSize().y;

        RenderPass kRenderPassConsole;
        kRenderPassConsole.pass = GUGU_RENDERPASS_DEFAULT;
        kRenderPassConsole.target = m_sfWindow;
        kRenderPassConsole.rectViewport = kViewport;

        m_consoleNode->Render(kRenderPassConsole, sf::Transform());
    }
}

void Window::Display()
{
    GUGU_SCOPE_TRACE_MAIN("Display");

    m_sfWindow->setActive();
    m_sfWindow->display();
}

void Window::DrawStats(const FrameInfos& kFrameInfos, const DeltaTime& _kFrameTime, const DeltaTime& _kTimeSinceLastFrame, const EngineStats& engineStats)
{
    if (!GetResources()->GetDebugFont())
        return;

    sf::Font* pFont = GetResources()->GetDebugFont()->GetSFFont();

    // Compute Values
    uint32 iNbCurvePointsMax = engineStats.maxStatCount;

    // Frame Times
    int iFrameTime = _kFrameTime.ms();
    int iMinFrameTime = 9999999;
    int iMaxFrameTime = 0;
    float fAvgFrameTime = 0.f;
    int iFPS = 1000 / ((_kTimeSinceLastFrame.ms() > 0) ? _kTimeSinceLastFrame.ms() : 1);

    // Draw Calls
    int iMaxDrawCalls = 1;

    // Step Times
    int minStepTime = 9999999;
    int maxStepTime = 0;
    int lastStepTime = -1;

    // Update Times
    int minUpdateTime = 9999999;
    int maxUpdateTime = 0;
    int lastUpdateTime = -1;

    // Render Times
    int minRenderTime = 9999999;
    int maxRenderTime = 0;
    int lastRenderTime = -1;

    {
        GUGU_SCOPE_TRACE_MAIN("Compute Values");

        // Frame Times
        m_statFrameTimes.push_front(iFrameTime);
        if (m_statFrameTimes.size() > iNbCurvePointsMax)
        {
            m_statFrameTimes.pop_back();
        }

        int iTotalFrameTime = 0;
        for (int iValue : m_statFrameTimes)
        {
            iMaxFrameTime = Max(iMaxFrameTime, iValue);
            iMinFrameTime = Min(iMinFrameTime, iValue);
            iTotalFrameTime += iValue;
        }

        fAvgFrameTime = (float)iTotalFrameTime / m_statFrameTimes.size();

        // Draw Calls
        m_statDrawCalls.push_front(kFrameInfos.statDrawCalls);
        if (m_statDrawCalls.size() > iNbCurvePointsMax)
        {
            m_statDrawCalls.pop_back();
        }

        for (int iValue : m_statDrawCalls)
        {
            iMaxDrawCalls = Max(iMaxDrawCalls, iValue);
        }

        // Step Times
        for (int value : engineStats.stepTimes)
        {
            maxStepTime = value >= 0 ? Max(maxStepTime, value) : maxStepTime;
            minStepTime = value >= 0 ? Min(minStepTime, value) : minStepTime;
            lastStepTime = lastStepTime == -1 && value >= 0 ? value : lastStepTime;
        }

        lastStepTime = Max(0, lastStepTime);

        // Update Times
        for (int value : engineStats.updateTimes)
        {
            maxUpdateTime = value >= 0 ? Max(maxUpdateTime, value) : maxUpdateTime;
            minUpdateTime = value >= 0 ? Min(minUpdateTime, value) : minUpdateTime;
            lastUpdateTime = lastUpdateTime == -1 && value >= 0 ? value : lastUpdateTime;
        }

        lastUpdateTime = Max(0, lastUpdateTime);

        // Render Times
        for (int value : engineStats.renderTimes)
        {
            maxRenderTime = value >= 0 ? Max(maxRenderTime, value) : maxRenderTime;
            minRenderTime = value >= 0 ? Min(minRenderTime, value) : minRenderTime;
            lastRenderTime = lastRenderTime == -1 && value >= 0 ? value : lastRenderTime;
        }

        lastRenderTime = Max(0, lastRenderTime);
    }

    // Draw Curves
    float fPositionX = 5.f;
    float fPositionY = 5.f;
    float fCurveWidth = Min(600.f, GetSize().x - 250.f);
    float fCurveHeight = 100.f;
    float fMarginTop = 10.f;

    sf::Color kColorCurveBox = sf::Color(255, 255, 255, 255);
    sf::Color kColorCurveFrameTimes = sf::Color(255, 255, 255, 255);
    sf::Color kColorCurveDrawCalls = sf::Color(255, 255, 70, 255);
    sf::Color colorCurveStepTimes = sf::Color(70, 200, 255, 255);
    sf::Color colorCurveUpdateTimes = sf::Color(255, 140, 50, 255);
    sf::Color colorCurveRenderTimes = sf::Color(255, 255, 255, 255);

    {
        GUGU_SCOPE_TRACE_MAIN("Curves");

        // Background
        Texture* textureBackground = GetResources()->GetCustomTexture("GuguConsoleBackground");
        m_statsBackground.setTexture(*textureBackground->GetSFTexture());
        m_statsBackground.setScale((fCurveWidth + fPositionX + 200.f) / textureBackground->GetSize().x, (fCurveHeight + fPositionY + 66.f) / textureBackground->GetSize().y);
        m_sfWindow->draw(m_statsBackground);

        // Step Times
        {
            int curveTopValue = Max(4, maxStepTime);
            float curvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float curvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int index = 0;
            sf::VertexArray curve(sf::PrimitiveType::Lines);
            curve.resize(engineStats.stepTimes.size() * 2);
            for (int value : engineStats.stepTimes)
            {
                value = Max(0, value);
                curve[index * 2] = (sf::Vertex(Vector2f(fPositionX + index * curvePointScaleX, fPositionY + fCurveHeight /* - value * curvePointScaleY*/), colorCurveStepTimes));
                curve[index * 2 + 1] = (sf::Vertex(Vector2f(fPositionX + index * curvePointScaleX, fPositionY + fCurveHeight - value * curvePointScaleY), colorCurveStepTimes));
                ++index;
            }

            m_sfWindow->draw(curve);
        }

        // Update Times
        {
            int curveTopValue = Max(4, maxUpdateTime);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(engineStats.updateTimes.size());
            for (int iValue : engineStats.updateTimes)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), colorCurveUpdateTimes));
                ++iIndex;
            }

            m_sfWindow->draw(kCurve);
        }

        // Frame Times
        {
            int curveTopValue = Max(4, iMaxFrameTime);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statFrameTimes.size());
            for (int iValue : m_statFrameTimes)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveFrameTimes));
                ++iIndex;
            }

            m_sfWindow->draw(kCurve);
        }

        // Draw Calls
        {
            int curveTopValue = Max(4, iMaxDrawCalls);
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / curveTopValue;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statDrawCalls.size());
            for (int iValue : m_statDrawCalls)
            {
                kCurve[iIndex] = (sf::Vertex(Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveDrawCalls));
                ++iIndex;
            }

            m_sfWindow->draw(kCurve);
        }

        // Borders
        sf::VertexArray kBorder(sf::PrimitiveType::LineStrip);
        kBorder.resize(4);
        kBorder[0] = (sf::Vertex(Vector2f(fPositionX, fPositionY), kColorCurveBox));
        kBorder[1] = (sf::Vertex(Vector2f(fPositionX, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[2] = (sf::Vertex(Vector2f(fPositionX + fCurveWidth, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[3] = (sf::Vertex(Vector2f(fPositionX + fCurveWidth, fPositionY), kColorCurveBox));
        m_sfWindow->draw(kBorder);
    }

    // Draw Text Infos
    {
        GUGU_SCOPE_TRACE_MAIN("Text Infos");

        float fPosX = fPositionX + fCurveWidth + 10.f;
        float fPosY = fPositionY;
        float fGapY = 20.f;

        sf::Color kColorText = sf::Color(255, 255, 255, 255);
        int iFontSize = 15;

        // Frame Times
        m_statTextFameTime.setPosition(fPosX, fPosY);
        m_statTextFameTime.setFillColor(kColorText);
        m_statTextFameTime.setCharacterSize(iFontSize);
        m_statTextFameTime.setString(StringFormat("Frame: {0} ms", iFrameTime));
        m_statTextFameTime.setFont(*pFont);
        m_sfWindow->draw(m_statTextFameTime);
        fPosY += fGapY;

        m_statTextFameTimeAverage.setPosition(fPosX, fPosY);
        m_statTextFameTimeAverage.setFillColor(kColorText);
        m_statTextFameTimeAverage.setCharacterSize(iFontSize);
        m_statTextFameTimeAverage.setString(StringFormat("Avg : {0} ms", fAvgFrameTime));
        m_statTextFameTimeAverage.setFont(*pFont);
        m_sfWindow->draw(m_statTextFameTimeAverage);
        fPosY += fGapY;

        m_statTextFameTimeMin.setPosition(fPosX, fPosY);
        m_statTextFameTimeMin.setFillColor(kColorText);
        m_statTextFameTimeMin.setCharacterSize(iFontSize);
        m_statTextFameTimeMin.setString(StringFormat("Min: {0} ms", iMinFrameTime));
        m_statTextFameTimeMin.setFont(*pFont);
        m_sfWindow->draw(m_statTextFameTimeMin);
        fPosY += fGapY;

        m_statTextFameTimeMax.setPosition(fPosX, fPosY);
        m_statTextFameTimeMax.setFillColor(kColorText);
        m_statTextFameTimeMax.setCharacterSize(iFontSize);
        m_statTextFameTimeMax.setString(StringFormat("Max: {0} ms", iMaxFrameTime));
        m_statTextFameTimeMax.setFont(*pFont);
        m_sfWindow->draw(m_statTextFameTimeMax);
        fPosY += fGapY;

        m_statTextFPS.setPosition(fPosX, fPosY);
        m_statTextFPS.setFillColor(kColorText);
        m_statTextFPS.setCharacterSize(iFontSize);
        m_statTextFPS.setString(StringFormat("FPS: {0}", iFPS));
        m_statTextFPS.setFont(*pFont);
        m_sfWindow->draw(m_statTextFPS);
        fPosY += fGapY;

        // Step Times
        m_statTextStepTime.setPosition(fPositionX, fPositionY + fCurveHeight);
        m_statTextStepTime.setFillColor(colorCurveStepTimes);
        m_statTextStepTime.setCharacterSize(iFontSize);
        m_statTextStepTime.setString(StringFormat("Step: {0} ms (min: {1} ms, max: {2} ms)", lastStepTime, minStepTime, maxStepTime));
        m_statTextStepTime.setFont(*pFont);
        m_sfWindow->draw(m_statTextStepTime);

        // Update Times
        m_statTextUpdateTime.setPosition(fPositionX, fPositionY + 22.f + fCurveHeight);
        m_statTextUpdateTime.setFillColor(colorCurveUpdateTimes);
        m_statTextUpdateTime.setCharacterSize(iFontSize);
        m_statTextUpdateTime.setString(StringFormat("Update: {0} ms (min: {1} ms, max: {2} ms)", lastUpdateTime, minUpdateTime, maxUpdateTime));
        m_statTextUpdateTime.setFont(*pFont);
        m_sfWindow->draw(m_statTextUpdateTime);

        // Render Times
        m_statTextRenderTime.setPosition(fPositionX, fPositionY + 44.f + fCurveHeight);
        m_statTextRenderTime.setFillColor(colorCurveRenderTimes);
        m_statTextRenderTime.setCharacterSize(iFontSize);
        m_statTextRenderTime.setString(StringFormat("Render: {0} ms (min: {1} ms, max: {2} ms)", lastRenderTime, minRenderTime, maxRenderTime));
        m_statTextRenderTime.setFont(*pFont);
        m_sfWindow->draw(m_statTextRenderTime);

        // Draw Calls
        m_statTextDrawCalls.setPosition(fPositionX + 320.f, fPositionY + fCurveHeight);
        m_statTextDrawCalls.setFillColor(kColorCurveDrawCalls);
        m_statTextDrawCalls.setCharacterSize(iFontSize);
        m_statTextDrawCalls.setString(StringFormat("Draw calls: {0} (tris: {1})", kFrameInfos.statDrawCalls, kFrameInfos.statTriangles));
        m_statTextDrawCalls.setFont(*pFont);
        m_sfWindow->draw(m_statTextDrawCalls);

        // Is Tracing
        if (engineStats.isTracing)
        {
            m_statTextIsTracing.setPosition(fPosX, fPositionY + fCurveHeight);
            m_statTextIsTracing.setFillColor(kColorText);
            m_statTextIsTracing.setCharacterSize(iFontSize);
            m_statTextIsTracing.setString("(Tracing)");
            m_statTextIsTracing.setFont(*pFont);
            m_sfWindow->draw(m_statTextIsTracing);
        }
    }
}

void Window::DrawFPS(const DeltaTime& _kTimeSinceLastFrame)
{
    if (!GetResources()->GetDebugFont())
        return;

    sf::Font* pFont = GetResources()->GetDebugFont()->GetSFFont();

    int iFPS = 1000 / ((_kTimeSinceLastFrame.ms() > 0) ? _kTimeSinceLastFrame.ms() : 1);

    sf::Color oColorText = sf::Color(255,255,255);

    m_statTextFPS.setPosition(2.f, 2.f);
    m_statTextFPS.setFillColor(oColorText);
    m_statTextFPS.setCharacterSize(20);
    m_statTextFPS.setString("FPS: " + ToString(iFPS));
    m_statTextFPS.setFont(*pFont);
    m_sfWindow->draw(m_statTextFPS);
}

void Window::OnConsoleCommandValidated()
{
    std::string strCommandLine = m_consoleTextEntry->GetText();
    GetEngine()->ComputeCommandLine(strCommandLine);

    m_consoleTextEntry->SetText("");
    m_consoleNode->SetVisible(false);
}

Vector2u Window::GetSize() const
{
    return m_sfWindow->getSize();
}

void Window::ComputeSize(int _iWidth, int _iHeight)
{
    sf::View kView;
    kView.setSize((float)_iWidth, (float)_iHeight);
    kView.setCenter((float)_iWidth / 2.f, (float)_iHeight / 2.f);

    m_sfWindow->setView(kView);

    m_rootNode->SetSize((float)_iWidth, (float)_iHeight);
    //m_pMouseNode->SetSize((float)_iWidth, (float)_iHeight);
    //m_pConsoleNode->SetSize((float)_iWidth, (float)_iHeight);

    m_mainCamera->ComputeViewSize();
    for (size_t i = 0; i < m_cameras.size(); ++i)
    {
        m_cameras[i]->ComputeViewSize();
    }
}

bool Window::ProcessEvents()
{
    sf::Event event;
    while (m_sfWindow->pollEvent(event))
    {
        GUGU_SCOPE_TRACE_MAIN("Window Process Event");

        bool propagateEvent = true;

        if (event.type == sf::Event::Closed)
        {
            return true;
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            // TODO: the interaction code uses a method to get the mouse pixel coords not depending on this event, I should do the same for the cursor render.
            m_mouseNode->SetPosition(GetMousePosition());
        }
        else if (event.type == sf::Event::MouseLeft)
        {
            m_windowFocused = false;
        }
        else if (event.type == sf::Event::MouseEntered)
        {
            m_windowFocused = true;
        }
        else if (event.type == sf::Event::Resized)
        {
            ComputeSize(event.size.width, event.size.height);
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Quote)
        {
            if (!m_consoleNode->IsVisible())
            {
                m_consoleNode->SetVisible(true);
                m_consoleTextEntry->StartEdition();
            }
            else
            {
                m_consoleTextEntry->StopEdition();
                m_consoleNode->SetVisible(false);
            }

            propagateEvent = false;
        }
        else if (m_consoleNode->IsVisible())
        {
            m_consoleTextEntry->OnSFEvent(event);
            propagateEvent = false;
        }

        if (m_hostImGui && propagateEvent)
        {
            // This will help disabling imgui text entries, but not the mouse events, they are handled in ImGui::SFML::Update.
            if (!m_consoleNode->IsVisible())
            {
                ImGui::SFML::ProcessEvent(event);
            }
        }

        if (m_hostImGui && ImGui::GetIO().WantCaptureMouse
            && (event.type == sf::Event::MouseButtonPressed
                || event.type == sf::Event::MouseButtonReleased
                || event.type == sf::Event::MouseWheelMoved
                || event.type == sf::Event::MouseWheelScrolled
                || event.type == sf::Event::MouseMoved))
        {
            propagateEvent = false;
        }

        if (propagateEvent)
        {
            std::vector<HandlerEvents::InteractiveElementEntry> vecRootElements;

            if (m_rootNode && m_mainCamera)
            {
                m_rootNode->SortOnZIndex();

                HandlerEvents::InteractiveElementEntry kEntry;
                kEntry.element = m_rootNode;
                kEntry.camera = m_mainCamera;
                vecRootElements.push_back(kEntry);
            }

            for (size_t i = 0; i < m_levelBindings.size(); ++i)
            {
                if (m_levelBindings[i].level && m_levelBindings[i].level->GetRootNode() && m_levelBindings[i].camera)
                {
                    HandlerEvents::InteractiveElementEntry kEntry;
                    kEntry.element = m_levelBindings[i].level->GetRootNode();
                    kEntry.camera = m_levelBindings[i].camera;
                    vecRootElements.push_back(kEntry);
                }
            }

            m_handlerEvents->ProcessEventOnElements(event, vecRootElements);
        }
    }

    return false;
}

bool Window::IsConsoleVisible() const
{
    return m_consoleNode->IsVisible();
}

Element* Window::GetRootNode() const
{
    return m_rootNode;
}

Element* Window::GetUINode() const
{
    return m_rootUINode;
}

ElementSprite* Window::GetMouseNode() const
{
    return m_mouseNode;
}

void Window::SetSystemMouseVisible(bool _bIsVisible)
{
    m_systemMouseVisible = _bIsVisible;
}

void Window::SetMouseVisible(bool _bIsVisible)
{
    m_mouseVisible = _bIsVisible;
}

void Window::SetMouseTexture(const std::string& _strFile)
{
    m_mouseNode->SetTexture(_strFile);
}

Vector2i Window::GetMousePixelCoords() const
{
    return sf::Mouse::getPosition(*m_sfWindow);
}

Vector2f Window::GetMousePosition() const
{
    return Vector2f(sf::Mouse::getPosition(*m_sfWindow));
}

sf::RenderWindow* Window::GetSFRenderWindow() const
{
    return m_sfWindow;
}

HandlerEvents* Window::GetEvents() const
{
    return m_handlerEvents;
}

bool Window::Screenshot() const
{
    if (EnsureDirectoryExists(GetResources()->GetPathScreenshots()))
    {
        sf::Texture kTexture;
        kTexture.create(m_sfWindow->getSize().x, m_sfWindow->getSize().y);
        kTexture.update(*m_sfWindow);

        sf::Image kImage = kTexture.copyToImage();

        std::thread kThreadSaveFile([kImage]() {
            kImage.saveToFile(GetResources()->GetPathScreenshots() + StringFormat("Screenshot_{0}.png", GetTimestamp()));
        });
        kThreadSaveFile.detach();

        //TODO: If the application exits before the thread has finished its execution, the screenshot will be lost and there will be a memory leak.

        return true;
    }

    return false;
}

void Window::SetShowStats(bool _bShowStats)
{
    m_showStats = _bShowStats;
}

void Window::SetShowFPS(bool _bShowFPS)
{
    m_showFPS = _bShowFPS;
}

void Window::ToggleShowStats()
{
    m_showStats = !m_showStats;
    m_showFPS = false;
}

void Window::ToggleShowFPS()
{
    m_showStats = false;
    m_showFPS = !m_showFPS;
}

void Window::SetShowBounds(bool showBounds)
{
    m_showBounds = showBounds;
}

void Window::ToggleShowBounds()
{
    m_showBounds = !m_showBounds;
}

Window* GetGameWindow()
{
    return GetEngine()->GetGameWindow();
}

}   // namespace gugu
