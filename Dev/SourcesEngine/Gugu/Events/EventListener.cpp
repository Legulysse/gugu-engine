////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Events/EventListener.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/HandlerEvents.h"
#include "Gugu/Window/Window.h"

#include <SFML/Window/Event.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
EventListener::EventListener()
{
    m_handler = nullptr;
}

EventListener::~EventListener()
{
    UnregisterHandlerEvents();
}

void EventListener::RegisterHandlerEvents(Window* _pWindow)
{
    if (_pWindow && _pWindow->GetEvents())
    {
        RegisterHandlerEvents(_pWindow->GetEvents());
    }
}

void EventListener::RegisterHandlerEvents(HandlerEvents* _pHandler)
{
    if (_pHandler)
    {
        _pHandler->AddEventListener(this);
    }
}

void EventListener::UnregisterHandlerEvents()
{
    if (m_handler)
    {
        m_handler->RemoveEventListener(this);
    }
}

bool EventListener::IsRegistered() const
{
    return (m_handler != nullptr);
}

bool EventListener::IsRegistered(Window* _pWindow) const
{
    return (m_handler != nullptr && _pWindow != nullptr && m_handler == _pWindow->GetEvents());
}

bool EventListener::IsRegistered(HandlerEvents* _pHandler) const
{
    return (m_handler != nullptr && m_handler == _pHandler);
}

bool EventListener::OnSFEvent(const sf::Event& _oSFEvent)
{
    return true;
}

}   // namespace gugu
