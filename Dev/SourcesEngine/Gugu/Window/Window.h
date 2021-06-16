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
    class HandlerEvents;
    class Element;
    class ElementSprite;
    class ElementText;
    class Camera;
    class Renderer;
    struct FrameInfos;
    class Level;
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

    void        Step            (const DeltaTime& dt);
    void        Update          (const DeltaTime& dt);

    void        SetRenderer    (Renderer* _pRenderer);     //Assign a Renderer (no ownership transmission)
    Renderer*   GetRenderer    () const;

    Camera*     CreateCamera    ();
    Camera*     AddCamera       (Camera* _pCamera);
    void        RemoveCamera    (Camera* _pCamera);
    void        DeleteCamera    (Camera* _pCamera);
    void        DeleteAllCameras();

    Camera*     GetCamera       (size_t _iIndex) const;
    Camera*     GetMainCamera   () const;
    
    void        BindLevel       (Level* _pLevel);
    void        BindLevel       (Level* _pLevel, Camera* _pCamera);
    void        BindLevel       (Level* _pLevel, Renderer* _pRenderer);
    void        BindLevel       (Level* _pLevel, Camera* _pCamera, Renderer* _pRenderer);
    void        OnLevelReleased (Level* _pLevel);

    void        Refresh         (const DeltaTime& dt, const EngineStats& engineStats);
    
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

protected:

    virtual void Render(const DeltaTime& dt, const EngineStats& engineStats);

private:

    bool ProcessEvent(const sf::Event& event);

    void OnConsoleCommandValidated();

    void DrawStats(const FrameInfos& kFrameInfos, const DeltaTime& _kFrameTime, const DeltaTime& _kTimeSinceLastFrame, const EngineStats& engineStats);
    void DrawFPS(const DeltaTime& _kTimeSinceLastFrame);

protected:

    //TODO: It should be better to directly use Cameras to store and handle their attachment to Windows and Levels
    struct LevelBinding
    {
        Level* level;
        Camera* camera;
        Renderer* renderer;
    };

    sf::RenderWindow*   m_sfWindow;
    Renderer*           m_renderer;

    bool                m_hostImGui;

    Camera*                     m_mainCamera;
    std::vector<Camera*>        m_cameras;
    std::vector<LevelBinding>   m_levelBindings;

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
    std::list<int>      m_statFrameTimes;
    std::list<int>      m_statDrawCalls;

    sf::Sprite m_statsBackground;
    sf::Text m_statTextFameTime;
    sf::Text m_statTextFameTimeAverage;
    sf::Text m_statTextFameTimeMin;
    sf::Text m_statTextFameTimeMax;
    sf::Text m_statTextFPS;
    sf::Text m_statTextDrawCalls;
    sf::Text m_statTextStepTime;
    sf::Text m_statTextIsTracing;

    // Debug
    bool m_showBounds;
};

Window* GetGameWindow();

}   // namespace gugu
