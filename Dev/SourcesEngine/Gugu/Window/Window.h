#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Math/Vector2.h"

#include <SFML/Graphics/Sprite.hpp>   //TODO: use pointers + forward declaration
#include <SFML/Graphics/Text.hpp>   //TODO: use pointers + forward declaration

#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    struct EngineConfig;
    struct EngineStats;
    struct FrameInfos;
    class HandlerEvents;
    class Element;
    class ElementSprite;
    class ElementText;
    class Camera;
    class Renderer;
    class Scene;
    class StatsDrawer;
}

namespace sf
{
    class RenderWindow;
    class Event;
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
    
    void        BindScene       (Scene* scene);
    void        BindScene       (Scene* scene, Camera* _pCamera);
    void        BindScene       (Scene* scene, Renderer* _pRenderer);
    void        BindScene       (Scene* scene, Camera* _pCamera, Renderer* _pRenderer);
    void        OnSceneReleased (Scene* scene);

    void        Render          (const DeltaTime& dt, const EngineStats& engineStats);
    void        Display         ();

    Vector2u    GetSize     () const;

    void            SetSystemMouseVisible   (bool _bIsVisible);
    void            SetMouseVisible         (bool _bIsVisible);
    void            SetMouseTexture         (const std::string& _strTexture);

    Vector2i    GetMousePixelCoords     () const;   //Exact mouse position
    Vector2f    GetMousePosition        () const;   //GetMouseCoords() converted to float
    
    Element*            GetRootNode     () const;   //Should not be used directly, prefer using UINode
    Element*            GetUINode       () const;
    ElementSprite*      GetMouseNode    () const;

    sf::RenderWindow*   GetSFRenderWindow() const;

    HandlerEvents*      GetEvents() const;

    bool ProcessEvents();

    bool IsConsoleVisible() const;

    bool Screenshot() const;

    void SetShowStats   (bool _bShowStats);
    void SetShowFPS     (bool _bShowFPS);
    void ToggleShowStats    ();
    void ToggleShowFPS      ();

    void SetShowBounds(bool showBounds);
    void ToggleShowBounds();

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

    sf::RenderWindow*   m_sfWindow;
    Renderer*           m_renderer;

    bool                m_hostImGui;

    Camera*                     m_mainCamera;
    std::vector<Camera*>        m_cameras;
    std::vector<SceneBinding>   m_sceneBindings;

    HandlerEvents*      m_handlerEvents;

    Element*            m_rootNode;             //Root for all Window Elements
    Element*            m_rootUINode;           //Root for the interface content
    Element*            m_consoleNode;          //Root for the console
    ElementText*        m_consoleTextEntry;
    ElementSprite*      m_mouseNode;            //Root for the mouse

    bool                m_systemMouseVisible;
    bool                m_mouseVisible;
    bool                m_windowFocused;

    sf::Color           m_backgroundColor;

    // Stats
    bool                m_showStats;
    bool                m_showFPS;

    // Debug
    StatsDrawer* m_statsDrawer;
    bool m_showBounds;
};

Window* GetGameWindow();

}   // namespace gugu
