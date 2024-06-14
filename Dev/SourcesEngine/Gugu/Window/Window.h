#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    struct EngineStats;
    struct FrameInfos;
    class WindowEventHandler;
    class Element;
    class ElementSprite;
    class ElementEditableText;
    class Camera;
    class Renderer;
    class Scene;
    class StatsDrawer;
}

namespace sf
{
    class RenderWindow;
    class Event;
    class Time;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class Window
{
public:

    Window();
    virtual ~Window();

    sf::RenderWindow*   Create  (const EngineConfig& config, bool hostImGui);
    virtual void        Init    (sf::RenderWindow* _pSFWindow, const EngineConfig& config);

    void        SetRenderer    (Renderer* _pRenderer);     //Assign a Renderer (no ownership transmission)
    Renderer*   GetRenderer    () const;

    Camera*     CreateCamera    ();
    Camera*     AddCamera       (Camera* _pCamera);
    void        RemoveCamera    (Camera* _pCamera);
    void        DeleteCamera    (Camera* _pCamera);
    void        DeleteAllCameras();

    Camera*     GetCamera       (size_t _iIndex) const;
    Camera*     GetMainCamera   () const;
    
    void        BindScene       (Scene* scene, Camera* _pCamera);
    void        BindScene       (Scene* scene, Camera* _pCamera, Renderer* _pRenderer);
    void        UnbindScene     (Scene* scene);

    void        OnSceneReleased (Scene* scene);

    void        Render          (const sf::Time& loopTime, const EngineStats& engineStats);
    void        Display         ();

    Vector2u    GetSize     () const;

    void            SetSystemMouseVisible   (bool _bIsVisible);
    void            SetMouseVisible         (bool _bIsVisible);
    void            SetMouseTexture         (const std::string& _strTexture);

    Vector2i    GetMousePixelCoords     () const;   //Exact mouse position
    Vector2f    GetMousePosition        () const;   //GetMouseCoords() converted to float
    
    Element*            GetRootNode     () const;   //Should not be used directly, prefer using UINode
    Element*            GetUINode       () const;
    Element*            GetMouseNode    () const;

    sf::RenderWindow*   GetSFRenderWindow() const;

    WindowEventHandler*      GetEventHandler() const;

    bool ProcessEvents();
    bool IsInputAllowed() const;

    bool IsHostingImGui() const;

    bool IsFocused() const;
    bool IsHovered() const;
    
    bool IsConsoleVisible() const;

    bool Screenshot() const;

    void SetShowStats(bool showStats);
    void SetShowFPS(bool showFPS);
    void SetShowBounds(bool showBounds);
    void SetShowRuler(bool showRuler);
    void ToggleShowStats();
    void ToggleShowFPS();
    void ToggleShowBounds();
    void ToggleShowRuler();

    virtual void ComputeSize    (int _iWidth, int _iHeight);

private:

    void OnConsoleCommandValidated();

protected:

    //TODO: It should be better to directly use Cameras to store and handle their attachment to Windows and Scenes
    struct SceneBinding
    {
        Scene* scene;
        Camera* camera;
        Renderer* renderer;
    };

    sf::RenderWindow* m_sfWindow;
    Renderer* m_renderer;

    bool m_hostImGui;

    Camera* m_mainCamera;
    std::vector<Camera*> m_cameras;
    std::vector<SceneBinding> m_sceneBindings;

    WindowEventHandler* m_eventHandler;

    Element* m_rootNode;                        //Root for all Window Elements
    Element* m_rootUINode;                      //Root for the interface content
    Element* m_consoleNode;                     //Root for the console
    ElementEditableText* m_consoleTextEntry;
    ElementSprite* m_mouseNode;                 //Root for the mouse

    bool m_systemMouseVisible;
    bool m_mouseVisible;
    bool m_wasSystemMouseVisible;

    bool m_windowFocused;
    bool m_windowHovered;

    sf::Color m_backgroundColor;

    // Debug
    StatsDrawer* m_statsDrawer;
    sf::VertexArray m_ruler;
    bool m_showStats;
    bool m_showFPS;
    bool m_showBounds;
    bool m_showRuler;
};

Window* GetGameWindow();

}   // namespace gugu
