#pragma once

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class HandlerEvents;
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
    friend class HandlerEvents;

public:

    EventListener();
    virtual ~EventListener();
    
    void RegisterHandlerEvents(Window* _pWindow);
    void RegisterHandlerEvents(HandlerEvents* _pHandler);
    void UnregisterHandlerEvents();

    bool IsRegistered() const;
    bool IsRegistered(Window* _pWindow) const;
    bool IsRegistered(HandlerEvents* _pHandler) const;

    virtual bool OnSFEvent(const sf::Event& _oSFEvent);     //Return true : allow event to be propagated.

private:

    HandlerEvents* m_handler;
};

}   // namespace gugu
