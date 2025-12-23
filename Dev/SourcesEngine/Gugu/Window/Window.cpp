////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Window/Window.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/Core/EngineConfig.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Resources/Font.h"
#include "Gugu/Window/Renderer.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Window/Vertex2.h"
#include "Gugu/Events/WindowEventHandler.h"
#include "Gugu/Element/Element.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/UI/ElementEditableText.h"
#include "Gugu/Scene/Scene.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Time.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/Debug/StatsDrawer.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <thread>           // Used for async screenshot save.
#if defined(GUGU_OS_WINDOWS)
    #include <windows.h>    // Used for maximizing the window.
#endif

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

Window::Window()
    : m_windowFocused(false)
    , m_windowHovered(false)
    , m_usedFocusSafety(false)
{
    m_sfWindow = nullptr;
    m_renderer = nullptr;

    m_hostImGui = false;

    m_mainCamera = nullptr;
    m_eventHandler = nullptr;

    m_rootNode = nullptr;
    m_rootUINode = nullptr;
    m_consoleNode = nullptr;
    m_consoleTextEntry = nullptr;
    m_mouseNode = nullptr;

    m_systemMouseVisible = true;
    m_mouseVisible = false;
    m_wasSystemMouseVisible = true;

    m_backgroundColor = sf::Color(128,128,128,255);

    m_statsDrawer = nullptr;
    m_showStats = false;
    m_showFPS = false;
    m_showBounds = false;
    m_showRuler = false;
    m_rulerSize = 100;
}

Window::~Window()
{
    SafeDelete(m_statsDrawer);
    SafeDelete(m_eventHandler);

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
    sf::ContextSettings contextSettings;
    //contextSettings.antiAliasingLevel = 2;  // Request 2 levels of antialiasing

    //Create main window
    uint32 windowStyle = sf::Style::Default;
    sf::State windowState = sf::State::Windowed;
    int windowWidth = config.windowWidth;
    int windowHeight = config.windowHeight;

    if (config.fullscreen)
    {
        // sf::VideoMode::getDesktopMode() gives the current desktop resolution (ignoring potential scaling so its safe to use as-is).
        // sf::VideoMode::getFullscreenModes() could be used to provide a settings menu with a resolution selection.
        windowState = sf::State::Fullscreen;
        windowWidth = sf::VideoMode::getDesktopMode().size.x;
        windowHeight = sf::VideoMode::getDesktopMode().size.y;
    }

    m_sfWindow->create(sf::VideoMode(Vector2u(windowWidth, windowHeight)), config.applicationName, windowStyle, windowState, contextSettings);
    m_sfWindow->setFramerateLimit(config.framerateLimit);
    m_sfWindow->setVerticalSyncEnabled(config.enableVerticalSync);

    if (config.maximizeWindow && !config.fullscreen)
    {
#if defined(GUGU_OS_WINDOWS)
        ::ShowWindow(m_sfWindow->getNativeHandle(), SW_MAXIMIZE);
#endif
    }

    if (GetResources()->HasResource(config.applicationIcon))
    {
        sf::Image oImgIcon;
        if (oImgIcon.loadFromFile(GetResources()->GetResourceFileInfo(config.applicationIcon).GetFileSystemPath()))
        {
            m_sfWindow->setIcon(Vector2u(oImgIcon.getSize().x, oImgIcon.getSize().y), oImgIcon.getPixelsPtr());
        }
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
    m_eventHandler = new WindowEventHandler;

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

    sf::Image consoleImage(Vector2u(8, 8), sf::Color(0, 0, 0, 150));

    Texture* pTextureConsole = GetResources()->GetCustomTexture("GuguConsoleBackground");   //TODO: Prepare custom textures in the manager beforehand
    sf::Texture* pSFTextureConsole = pTextureConsole->GetSFTexture();
    bool loadResult = pSFTextureConsole->loadFromImage(consoleImage);

    Font* pFont = GetResources()->GetDebugFont();

    ElementSprite* pConsoleBackground = m_consoleNode->AddChild<ElementSprite>();
    pConsoleBackground->SetTexture(pTextureConsole);
    pConsoleBackground->SetUnifiedSize(UDim2(Vector2f(1.f, 0.f), Vector2f(0.f, 200.f)));

    m_consoleTextEntry = pConsoleBackground->AddChild<ElementEditableText>();
    m_consoleTextEntry->SetResizeRule(ETextResizeRule::FixedSize);
    m_consoleTextEntry->SetFont(pFont);
    m_consoleTextEntry->SetText("");
    m_consoleTextEntry->SetMultiline(false);
    //m_pConsoleTextEntry->SetEditable(true);
    m_consoleTextEntry->SetColor(sf::Color(220, 220, 220));
    m_consoleTextEntry->SetUnifiedPosition(UDim2::POSITION_BOTTOM_LEFT);
    m_consoleTextEntry->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    m_consoleTextEntry->SetUnifiedSize(UDim2(Vector2f(1.f, 0.f), Vector2f(0.f, 30.f)));
    m_consoleTextEntry->SetOnValidate(std::bind(&Window::OnConsoleCommandValidated, this));

    // Stats
    m_statsDrawer = new StatsDrawer;
    m_showStats = config.showStats;
    m_showFPS = config.showFPS;

    // Finalize
    ComputeSize(m_sfWindow->getSize());

    m_windowFocused = m_sfWindow->hasFocus();
}

void Window::SetBackgroundColor(const sf::Color& backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

const sf::Color& Window::GetBackgroundColor() const
{
    return m_backgroundColor;
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
    _pCamera->RecomputeSizeFromWindow();
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

void Window::BindScene(Scene* scene, Camera* camera)
{
    BindScene(scene, camera, m_renderer);
}

void Window::BindScene(Scene* scene, Camera* camera, Renderer* renderer)
{
    if (!scene || !camera || !renderer)
        return;

    if (camera == m_mainCamera)
        return;

    camera->SetScene(scene);

    SceneBinding kBinding;
    kBinding.scene = scene;
    kBinding.camera = camera;
    kBinding.renderer = renderer;
    m_sceneBindings.push_back(kBinding);
}

void Window::UnbindScene(Scene* scene)
{
    auto iteBinding = m_sceneBindings.begin();
    while (iteBinding != m_sceneBindings.end())
    {
        if (iteBinding->scene == scene)
        {
            //TODO: flag auto-delete on Camera ?
            // - I cant delete cameras like that, I have to check if they are in use and if its not the MainCamera.
            // - There are also cases where it is legit to regularly bind/unbind scenes/cameras on a window.
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

void Window::OnSceneReleased(Scene* scene)
{
    UnbindScene(scene);
}

void Window::Update(const DeltaTime& dt)
{
    // Note : There seems to be a bug with focus events on app start.
    // - I can get in a situation where the window has focus without the app getting events for this.
    // - Getting hasFocus on window setup then listening to events does not seem to be enough.
    // - This safety will check focus state until it is obtained at least once
    if (!m_windowFocused && !m_usedFocusSafety)
    {
        if (m_sfWindow->hasFocus())
        {
            m_windowFocused = true;
            m_usedFocusSafety = true;
        }
    }
}

void Window::Render(const sf::Time& loopTime, const EngineStats& engineStats)
{
    FrameInfos kFrameInfos;
    kFrameInfos.showBounds = m_showBounds;
    kFrameInfos.defaultBoundsShape.setOutlineThickness(-1.f);
    kFrameInfos.defaultBoundsShape.setOutlineColor(sf::Color(255, 0, 255, 200));
    kFrameInfos.defaultBoundsShape.setFillColor(sf::Color::Transparent);

    {
        GUGU_SCOPE_TRACE_MAIN("Clear");

        bool activationResult = m_sfWindow->setActive(true);
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
        GUGU_SCOPE_TRACE_MAIN("UI");

        //Handle Mouse visibility
        bool isMouseWantedVisible = m_mouseVisible && m_windowHovered && m_windowFocused;

        if (m_hostImGui)
        {
            if (ImGui::GetIO().WantCaptureMouse)
            {
                isMouseWantedVisible = false;
            }
            else
            {
                // The Imgui-Sfml backend will handle the cursor visibility when providing ImGuiMouseCursor_None.
                ImGui::SetMouseCursor(m_systemMouseVisible ? ImGuiMouseCursor_Arrow : ImGuiMouseCursor_None);
            }
        }
        else
        {
            bool isSystemMouseWantedVisible = m_systemMouseVisible || !m_windowHovered || !m_windowFocused;
            if (isSystemMouseWantedVisible != m_wasSystemMouseVisible)
            {
                m_sfWindow->setMouseCursorVisible(isSystemMouseWantedVisible);
                m_wasSystemMouseVisible = isSystemMouseWantedVisible;
            }
        }

        // Update mouse node.
        m_mouseNode->SetVisible(isMouseWantedVisible);
        m_mouseNode->SetPosition(GetGameWindow()->GetMousePosition());

        // Render UI
        m_rootNode->SortOnZIndex();

        if (m_renderer)
            m_renderer->RenderWindow(&kFrameInfos, this, m_mainCamera);
    }

    if (m_hostImGui)
    {
        GUGU_SCOPE_TRACE_MAIN("ImGui");

        ImGui::SFML::Render(*m_sfWindow);
    }

    {
        GUGU_SCOPE_TRACE_MAIN("Debug");

        //Ruler
        if (m_showRuler && m_windowFocused)
        {
            //TODO: merge into StatsDrawer, rename as DebugDrawer.
            if (m_ruler.getVertexCount() == 0)
            {
                float graduationSize = static_cast<float>(m_rulerSize);
                int graduations = RoundNearestInt(Max(GetSize().x, GetSize().y)) / m_rulerSize;

                Vector2f position = Vector2f(0.5f, 0.5f);
                Vector2f size = Vector2f(GetSize());
                sf::Color rulerColor = sf::Color(255, 0, 255, 200);
                m_ruler = sf::VertexArray(sf::PrimitiveType::Lines, 4 + graduations * 8);

                size_t v = 0;
                m_ruler[v++] = BuildVertex(Vector2f(position.x, -size.y), rulerColor);
                m_ruler[v++] = BuildVertex(Vector2f(position.x, size.y + 1.f), rulerColor);
                m_ruler[v++] = BuildVertex(Vector2f(-size.x, position.y), rulerColor);
                m_ruler[v++] = BuildVertex(Vector2f(size.x + 1.f, position.y), rulerColor);

                for (int i = 0; i < graduations; ++i)
                {
                    float offset = graduationSize * (i + 1);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x + offset, position.y - 10.f), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x + offset, position.y + 11.f), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x - offset, position.y - 10.f), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x - offset, position.y + 11.f), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x - 10.f, position.y + offset), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x + 11.f, position.y + offset), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x - 10.f, position.y - offset), rulerColor);
                    m_ruler[v++] = BuildVertex(Vector2f(position.x + 11.f, position.y - offset), rulerColor);
                }
            }

            Vector2f position = Vector2f(GetMousePixelCoords());
            sf::Transform rulerTransform;
            rulerTransform.translate(position);
            m_sfWindow->draw(m_ruler, sf::RenderStates(rulerTransform));
        }

        //Stats
        if (m_showStats)
            m_statsDrawer->DrawStats(kFrameInfos, kRenderClock.getElapsedTime(), loopTime, engineStats, this);
        else if (m_showFPS)
            m_statsDrawer->DrawFPS(loopTime, this);
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

    m_sfWindow->display();
    //m_sfWindow->setActive(false);
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

void Window::ComputeSize(const Vector2u& size)
{
    Vector2f floatSize = Vector2f(size);

    sf::View mainView;
    mainView.setSize(floatSize);
    mainView.setCenter(floatSize / 2.f);
    m_sfWindow->setView(mainView);

    m_rootNode->SetSize(floatSize);

    m_mainCamera->RecomputeSizeFromWindow();
    for (size_t i = 0; i < m_cameras.size(); ++i)
    {
        m_cameras[i]->RecomputeSizeFromWindow();
    }
}

bool Window::ProcessEvents()
{
    while (const std::optional<sf::Event> event = m_sfWindow->pollEvent())
    {
        GUGU_SCOPE_TRACE_MAIN("Window Process Event");

        bool propagateEvent = true;

        if (event->is<sf::Event::Closed>())
        {
            return true;
        }
        else if (event->is<sf::Event::MouseLeft>())
        {
            m_windowHovered = false;
        }
        else if (event->is<sf::Event::MouseEntered>())
        {
            m_windowHovered = true;
        }
        else if (event->is<sf::Event::FocusLost>())
        {
            m_windowFocused = false;
        }
        else if (event->is<sf::Event::FocusGained>())
        {
            m_windowFocused = true;
        }
        else if (const auto resizedEvent = event->getIf<sf::Event::Resized>())
        {
            ComputeSize(resizedEvent->size);
        }
        else if (const auto keyPressedEvent = event->getIf<sf::Event::KeyPressed>(); keyPressedEvent && keyPressedEvent->scancode == sf::Keyboard::Scan::Grave)
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
            m_consoleTextEntry->ProcessSFEvent(*event);
            propagateEvent = false;
        }

        if (m_hostImGui && propagateEvent)
        {
            const auto keyPressedEvent = event->getIf<sf::Event::KeyPressed>();
            const auto keyReleasedEvent = event->getIf<sf::Event::KeyReleased>();
            if (!(keyPressedEvent && keyPressedEvent->scancode == sf::Keyboard::Scan::Unknown)
                && !(keyReleasedEvent && keyReleasedEvent->scancode == sf::Keyboard::Scan::Unknown))
            {
                // This will help disabling imgui text entries, but not the mouse events, they are handled in ImGui::SFML::Update.
                if (!m_consoleNode->IsVisible())
                {
                    ImGui::SFML::ProcessEvent(*m_sfWindow, *event);
                }
            }
        }

        if (m_hostImGui)
        {
            if (ImGui::GetIO().WantCaptureMouse
                && (event->is<sf::Event::MouseButtonPressed>()
                    || event->is<sf::Event::MouseButtonReleased>()
                    || event->is<sf::Event::MouseWheelScrolled>()
                    || event->is<sf::Event::MouseMoved>()
                    || event->is<sf::Event::MouseMovedRaw>()))
            {
                propagateEvent = false;
            }
            else if (ImGui::GetIO().WantCaptureKeyboard
                && (event->is<sf::Event::TextEntered>()
                    || event->is<sf::Event::KeyPressed>()
                    || event->is<sf::Event::KeyReleased>()))
            {
                propagateEvent = false;
            }
        }

        if (propagateEvent)
        {
            std::vector<const Camera*> cameras;

            if (m_rootNode && m_mainCamera)
            {
                cameras.push_back(m_mainCamera);
            }

            for (size_t i = 0; i < m_sceneBindings.size(); ++i)
            {
                if (m_sceneBindings[i].scene && m_sceneBindings[i].scene->GetRootNode() && m_sceneBindings[i].camera)
                {
                    cameras.push_back(m_sceneBindings[i].camera);
                }
            }

            m_eventHandler->ProcessWindowEvent(*event, cameras);
        }
    }

    return false;
}

bool Window::IsFocused() const
{
    return m_windowFocused;
}

bool Window::IsHovered() const
{
    return m_windowHovered;
}

bool Window::UsedFocusSafety() const
{
    return m_usedFocusSafety;
}

bool Window::IsInputAllowed() const
{
    return m_windowFocused && !IsConsoleVisible();
}

bool Window::IsHostingImGui() const
{
    return m_hostImGui;
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

void Window::SetSystemMouseVisible(bool visible)
{
    m_systemMouseVisible = visible;
}

bool Window::IsSystemMouseVisible() const
{
    return m_systemMouseVisible;
}

void Window::SetMouseVisible(bool visible)
{
    m_mouseVisible = visible;
}

bool Window::IsMouseVisible() const
{
    return m_mouseVisible;
}

void Window::SetMouseTexture(const std::string& textureId)
{
    m_mouseNode->SetTexture(textureId);
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

WindowEventHandler* Window::GetEventHandler() const
{
    return m_eventHandler;
}

bool Window::Screenshot() const
{
    if (EnsureDirectoryExists(GetResources()->GetPathScreenshots()))
    {
        sf::Texture captureTexture(m_sfWindow->getSize());
        captureTexture.update(*m_sfWindow);

        sf::Image captureImage = captureTexture.copyToImage();

        std::thread saveThread([captureImage]() {
            bool saveResult = captureImage.saveToFile(CombinePaths(GetResources()->GetPathScreenshots(), StringFormat("Screenshot_{0}.png", GetLocalTimestampAsString(timeformat::Filename))));
        });
        saveThread.detach();

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

void Window::SetShowBounds(bool showBounds)
{
    m_showBounds = showBounds;
}

void Window::SetShowRuler(bool showRuler)
{
    m_showRuler = showRuler;
}

void Window::SetRulerSize(int rulerSize)
{
    rulerSize = Max(2, Absolute(rulerSize));

    if (m_rulerSize != rulerSize)
    {
        m_rulerSize = rulerSize;
        m_ruler.clear();
    }
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

void Window::ToggleShowBounds()
{
    m_showBounds = !m_showBounds;
}

void Window::ToggleShowRuler()
{
    m_showRuler = !m_showRuler;
}

bool Window::GetShowStats() const
{
    return m_showStats;
}

bool Window::GetShowFPS() const
{
    return m_showFPS;
}

bool Window::GetShowBounds() const
{
    return m_showBounds;
}

bool Window::GetShowRuler() const
{
    return m_showRuler;
}

Window* GetGameWindow()
{
    return GetEngine()->GetGameWindow();
}

}   // namespace gugu
