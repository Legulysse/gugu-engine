#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/Math/Vector2.h"

#include <vector>
#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class EventListener;
    class Element;
    class Camera;
}

namespace sf
{
    class Event;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class WindowEventHandler
{
public:

    struct InteractiveElementEntry
    {
        Element* element;
        Camera* camera;
    };

    WindowEventHandler();
    ~WindowEventHandler();

    void AddEventListener(EventListener* _pEventListener);
    void RemoveEventListener(EventListener* _pEventListener);
    bool IsEventListenerRegistered(EventListener* _pEventListener) const;

    void RegisterElementEventHandler(ElementEventHandler* eventHandler, EInteractionType::Type interactionType);
    void UnregisterElementEventHandler(ElementEventHandler* eventHandler);

    void ProcessWindowEvent(const sf::Event& event, const std::vector<const Camera*>& windowCameras);

private:

    bool CheckElementEventHandlerRegistration(ElementEventHandler* eventHandler);

    void BeginInteractions();
    bool ProcessEventListeners(const sf::Event& event);
    bool ProcessCameraElementInteractions(const sf::Event& event, const Camera* camera);
    bool ProcessElementInteractions(const sf::Event& event);

private:

    std::vector<EventListener*> m_eventListeners;

    std::vector<ElementEventHandler*> m_elementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseFocusElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseClickElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseScrollElementEventHandlers;
    std::vector<ElementEventHandler*> m_rawSFEventElementEventHandlers;

    Element* m_elementMouseFocused;
    Element* m_elementMouseSelected;
    Element* m_elementMouseDragged;
    Vector2f m_lastMouseCoords;
};

}   // namespace gugu
