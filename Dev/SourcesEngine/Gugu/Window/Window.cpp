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

#include "Gugu/Scene/Scene.h"

#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"

#include "Gugu/Debug/Trace.h"
#include "Gugu/Debug/StatsDrawer.h"

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
    SafeDelete(m_statsDrawer);
    SafeDelete(m_handlerEvents);

    m_sfWindow->close();

    m_sceneBindings.clear();

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

        m_sfWindow->setIcon(oImgIcon.getSize().x, oImgIcon.getSize().y, oImgIcon.getPixelsPtr());
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
    m_statsDrawer = new StatsDrawer;
    m_showStats = config.showStats;
    m_showFPS = config.showFPS;

    // Finalize
    ComputeSize(m_sfWindow->getSize().x, m_sfWindow->getSize().y);
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

void Window::BindScene(Scene* scene)
{
    BindScene(scene, m_mainCamera, m_renderer);
}

void Window::BindScene(Scene* scene, Camera* _pCamera)
{
    BindScene(scene, _pCamera, m_renderer);
}

void Window::BindScene(Scene* scene, Renderer* _pRenderer)
{
    BindScene(scene, m_mainCamera, _pRenderer);
}

void Window::BindScene(Scene* scene, Camera* _pCamera, Renderer* _pRenderer)
{
    if (!scene || !_pCamera || !_pRenderer)
        return;

    _pCamera->SetScene(scene);

    SceneBinding kBinding;
    kBinding.scene = scene;
    kBinding.camera = _pCamera;
    kBinding.renderer = _pRenderer;
    m_sceneBindings.push_back(kBinding);
}

void Window::OnSceneReleased(Scene* scene)
{
    auto iteBinding = m_sceneBindings.begin();
    while (iteBinding != m_sceneBindings.end())
    {
        if (iteBinding->scene == scene)
        {
            //TODO: flag auto-delete on Camera ?
            // I cant delete cameras like that, I have to check if they are in used and if its not the MainCamera
            //Camera* pCamera = iteBinding->m_pCamera;
            //StdVectorRemove(m_vecCameras, pCamera);
            //SafeDelete(pCamera);

            iteBinding = m_sceneBindings.erase(iteBinding);
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
        GUGU_SCOPE_TRACE_MAIN("Scenes");

        //Render Scenes
        for (size_t i = 0; i < m_sceneBindings.size(); ++i)
        {
            m_sceneBindings[i].renderer->RenderScene(&kFrameInfos, this, m_sceneBindings[i].scene, m_sceneBindings[i].camera);
        }
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Prepare UI");

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

        m_mouseNode->SetPosition(GetGameWindow()->GetMousePosition());

        //Render Window UI
        m_rootNode->SortOnZIndex();
    }

    {
        GUGU_SCOPE_TRACE_MAIN("UI");

        if (m_renderer)
            m_renderer->RenderWindow(&kFrameInfos, this, m_mainCamera);
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
            m_statsDrawer->DrawStats(kFrameInfos, DeltaTime(kRenderClock.getElapsedTime()), dt, engineStats, this);
        else if (m_showFPS)
            m_statsDrawer->DrawFPS(dt, this);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Console");

        //Console
        RenderPass kRenderPassConsole;
        kRenderPassConsole.pass = GUGU_RENDERPASS_DEFAULT;
        kRenderPassConsole.target = m_sfWindow;
        kRenderPassConsole.rectViewport = Renderer::ComputeViewport(m_sfWindow->getView());

        m_consoleNode->Render(kRenderPassConsole, sf::Transform());
    }
}

void Window::Display()
{
    GUGU_SCOPE_TRACE_MAIN("Display");

    m_sfWindow->setActive();
    m_sfWindow->display();
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

        if (m_hostImGui && propagateEvent
            && !((event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && event.key.code == sf::Keyboard::Unknown))
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

            for (size_t i = 0; i < m_sceneBindings.size(); ++i)
            {
                if (m_sceneBindings[i].scene && m_sceneBindings[i].scene->GetRootNode() && m_sceneBindings[i].camera)
                {
                    HandlerEvents::InteractiveElementEntry kEntry;
                    kEntry.element = m_sceneBindings[i].scene->GetRootNode();
                    kEntry.camera = m_sceneBindings[i].camera;
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

Element* Window::GetMouseNode() const
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
    //TODO: Should I cache this every frame ?
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
