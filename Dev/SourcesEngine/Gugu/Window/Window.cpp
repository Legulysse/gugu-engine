////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Window.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Manager/ManagerResources.h"

#include "Gugu/Render/Renderer.h"
#include "Gugu/Window/HandlerEvents.h"
#include "Gugu/Window/Camera.h"

#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementText.h"

#include "Gugu/World/Level.h"

#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Math.h"
#include "Gugu/Utility/Action.h"

#include "Gugu/Misc/Trace.h"

#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <thread>   // Used for async screenshot save

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Window::Window()
{
    m_sfWindow     = nullptr;
    m_renderer     = nullptr;
    m_mainCamera   = nullptr;

    m_handlerEvents = nullptr;

    m_rootNode     = nullptr;
    m_rootUINode        = nullptr;
    m_consoleNode  = nullptr;
    m_consoleTextEntry = nullptr;
    m_mouseNode    = nullptr;

    m_showStats        = false;
    m_showFPS           = false;
    m_isMouseVisible  = true;

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

sf::RenderWindow* Window::Create(const EngineConfig& config)
{
    m_sfWindow = new sf::RenderWindow;

    //Windows Settings
    sf::ContextSettings Settings;
    Settings.depthBits         = 24; // Request a 24 bits depth buffer
    Settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
    Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing

    //Create main window
    m_sfWindow->create(sf::VideoMode(config.windowWidth, config.windowHeight, 32), config.applicationName,/* sf::Style::Default */ sf::Style::Resize | sf::Style::Close, Settings);
    m_sfWindow->setFramerateLimit(config.framerateLimit);
    m_sfWindow->setVerticalSyncEnabled(config.enableVerticalSync);

    if (GetResources()->HasResource(config.applicationIcon))
    {
        sf::Image oImgIcon;
        oImgIcon.loadFromFile(GetResources()->GetResourcePathName(config.applicationIcon));

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
    SetMouseVisible(false);

    //Console Node
    m_consoleNode = m_rootNode->AddChild<Element>();
    //m_pConsoleNode->SetExtendViewOnResize(true);
    //m_pConsoleNode->SetSize((float)config.windowWidth, (float)config.windowHeight);
    m_consoleNode->SetUnifiedSize(UDim2::SIZE_FULL);
    m_consoleNode->SetVisible(false);

    sf::Image oImgConsole;
    oImgConsole.create(8, 8, sf::Color(0, 0, 0, 128));

    Texture* pTextureConsole = GetResources()->GetCustomTexture("GuguConsoleBackground");
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
    m_consoleTextEntry->SetOnValidate(new ActionClass<Window>(this, &Window::OnConsoleCommandValidated));

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

void Window::Refresh(const DeltaTime& dt, const EngineStats& engineStats)
{
    m_sfWindow->setActive();

    Render(dt, engineStats);
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

        //Render Window UI
        m_rootNode->SortOnZIndex();

        if (m_renderer)
            m_renderer->RenderWindow(kFrameInfos, this, m_mainCamera);
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

    {
        GUGU_SCOPE_TRACE_MAIN("Stats");

        //Stats
        if (m_showStats)
            DrawStats(kFrameInfos, DeltaTime(kRenderClock.getElapsedTime()), dt, engineStats);
        else if (m_showFPS)
            DrawFPS(dt);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Display");

        m_sfWindow->display();
    }
}

void Window::DrawStats(const FrameInfos& kFrameInfos, const DeltaTime& _kFrameTime, const DeltaTime& _kTimeSinceLastFrame, const EngineStats& engineStats)
{
    if (!GetResources()->GetDebugFont())
        return;

    sf::Font* pFont = GetResources()->GetDebugFont()->GetSFFont();

    // Compute Values
    uint32 iNbCurvePointsMax = 150;

    // Frame Times
    int iFrameTime = _kFrameTime.ms();
    int iMinFrameTime = 9999999;
    int iMaxFrameTime = 1;
    float fAvgFrameTime = 0.f;
    int iFPS = 1000 / ((_kTimeSinceLastFrame.ms() > 0) ? _kTimeSinceLastFrame.ms() : 1);

    // Draw Calls
    int iMaxDrawCalls = 1;

    // Step Times
    int maxStepTime = 1;
    int lastStepTime = (engineStats.stepTimes.empty()) ? 0 : engineStats.stepTimes.front();

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
            maxStepTime = Max(maxStepTime, value);
        }
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

    {
        GUGU_SCOPE_TRACE_MAIN("Curves");

        // Background
        Texture* textureBackground = GetResources()->GetCustomTexture("GuguConsoleBackground");
        m_statsBackground.setTexture(*textureBackground->GetSFTexture());
        m_statsBackground.setScale((fCurveWidth + fPositionX + 200.f) / textureBackground->GetSize().x, (fCurveHeight + fPositionY + 22.f) / textureBackground->GetSize().y);
        m_sfWindow->draw(m_statsBackground);

        // Frame Times
        {
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / iMaxFrameTime;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statFrameTimes.size());
            for (int iValue : m_statFrameTimes)
            {
                kCurve[iIndex] = (sf::Vertex(sf::Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveFrameTimes));
                ++iIndex;
            }
            m_sfWindow->draw(kCurve);
        }

        // Draw Calls
        {
            float fCurvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float fCurvePointScaleY = (fCurveHeight - fMarginTop) / iMaxDrawCalls;

            int iIndex = 0;
            sf::VertexArray kCurve(sf::PrimitiveType::LineStrip);
            kCurve.resize(m_statDrawCalls.size());
            for (int iValue : m_statDrawCalls)
            {
                kCurve[iIndex] = (sf::Vertex(sf::Vector2f(fPositionX + iIndex * fCurvePointScaleX, fPositionY + fCurveHeight - iValue * fCurvePointScaleY), kColorCurveDrawCalls));
                ++iIndex;
            }
            m_sfWindow->draw(kCurve);
        }

        // Step Times
        {
            float curvePointScaleX = fCurveWidth / (iNbCurvePointsMax - 1);
            float curvePointScaleY = (fCurveHeight - fMarginTop) / maxStepTime;

            int index = 0;
            sf::VertexArray curve(sf::PrimitiveType::LineStrip);
            curve.resize(engineStats.stepTimes.size());
            for (int value : engineStats.stepTimes)
            {
                curve[index] = (sf::Vertex(sf::Vector2f(fPositionX + index * curvePointScaleX, fPositionY + fCurveHeight - value * curvePointScaleY), colorCurveStepTimes));
                ++index;
            }
            m_sfWindow->draw(curve);
        }

        // Borders
        sf::VertexArray kBorder(sf::PrimitiveType::LineStrip);
        kBorder.resize(4);
        kBorder[0] = (sf::Vertex(sf::Vector2f(fPositionX, fPositionY), kColorCurveBox));
        kBorder[1] = (sf::Vertex(sf::Vector2f(fPositionX, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[2] = (sf::Vertex(sf::Vector2f(fPositionX + fCurveWidth, fPositionY + fCurveHeight), kColorCurveBox));
        kBorder[3] = (sf::Vertex(sf::Vector2f(fPositionX + fCurveWidth, fPositionY), kColorCurveBox));
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

        // Draw Calls
        m_statTextDrawCalls.setPosition(fPositionX, fPositionY + fCurveHeight);
        m_statTextDrawCalls.setFillColor(kColorCurveDrawCalls);
        m_statTextDrawCalls.setCharacterSize(iFontSize);
        m_statTextDrawCalls.setString(StringFormat("Draw calls: {0}", kFrameInfos.statDrawCalls));
        m_statTextDrawCalls.setFont(*pFont);
        m_sfWindow->draw(m_statTextDrawCalls);

        // Step Times
        m_statTextStepTime.setPosition(fPositionX + 180.f, fPositionY + fCurveHeight);
        m_statTextStepTime.setFillColor(colorCurveStepTimes);
        m_statTextStepTime.setCharacterSize(iFontSize);
        m_statTextStepTime.setString(StringFormat("Step time: {0} ms", lastStepTime));
        m_statTextStepTime.setFont(*pFont);
        m_sfWindow->draw(m_statTextStepTime);

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

sf::Vector2u Window::GetSize() const
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
    sf::Event oEvent;
    while (m_sfWindow->pollEvent(oEvent))
    {
        GUGU_SCOPE_TRACE_MAIN("Window Process Event");

        if (oEvent.type == sf::Event::Closed)
        {
            return true;
        }
        else if (m_consoleNode->IsVisible() && m_consoleTextEntry)
        {
            if (ProcessEvent(oEvent))
            {
                m_consoleTextEntry->OnSFEvent(oEvent);
            }
        }
        else
        {
            if (ProcessEvent(oEvent))
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

                m_handlerEvents->ProcessEventOnElements(oEvent, vecRootElements);
            }
        }
    }

    return false;
}

bool Window::ProcessEvent(const sf::Event& _oEvent)
{
    if (_oEvent.type == sf::Event::MouseMoved)
    {
        m_mouseNode->SetPosition(GetMousePosition());
    }
    else if (_oEvent.type == sf::Event::MouseLeft)
    {
        if (m_isMouseVisible)
            m_mouseNode->SetVisible(false);
    }
    else if (_oEvent.type == sf::Event::MouseEntered)
    {
        if (m_isMouseVisible)
            m_mouseNode->SetVisible(true);
    }
    else if (_oEvent.type == sf::Event::Resized)
    {
        ComputeSize(_oEvent.size.width, _oEvent.size.height);
    }
    else if (_oEvent.type == sf::Event::KeyReleased)
    {
        if (_oEvent.key.code == sf::Keyboard::Quote)
        {
            if (!m_consoleNode->IsVisible())
            {
                //Todo: Make sure the Console gets events before anything else, eventually blocking everything
                m_consoleNode->SetVisible(true);
                m_consoleTextEntry->StartEdition();
            }
            else
            {
                m_consoleTextEntry->StopEdition();
                m_consoleNode->SetVisible(false);
            }

            return false;
        }
    }

    return true;
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
    m_sfWindow->setMouseCursorVisible(_bIsVisible);
}

void Window::SetMouseVisible(bool _bIsVisible)
{
    m_isMouseVisible = _bIsVisible;
    m_mouseNode->SetVisible(m_isMouseVisible);
}

void Window::SetMouseTexture(const std::string& _strFile)
{
    m_mouseNode->SetTexture(_strFile);
}

sf::Vector2i Window::GetMousePixelCoords() const
{
    return sf::Mouse::getPosition(*m_sfWindow);
}

sf::Vector2f Window::GetMousePosition() const
{
    return sf::Vector2f(sf::Mouse::getPosition(*m_sfWindow));
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
