#pragma once

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class WindowEventHandler;
    class Window;
}

namespace sf
{
    class Event;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class EventListener
{
    friend class WindowEventHandler;

public:

    EventListener();
    virtual ~EventListener();
    
    void RegisterEventHandler(Window* _pWindow);
    void RegisterEventHandler(WindowEventHandler* _pHandler);
    void UnregisterEventHandler();

    bool IsRegistered() const;
    bool IsRegistered(Window* _pWindow) const;
    bool IsRegistered(WindowEventHandler* _pHandler) const;

    //TODO: rename as OnWindowEvent ?
    //TODO: add a OnInputEvent ?
    virtual bool OnSFEvent(const sf::Event& _oSFEvent);     //Return true : allow event to be propagated.

private:

    WindowEventHandler* m_handler;
};

}   // namespace gugu
