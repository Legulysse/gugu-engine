#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/ElementEvents.h"
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

    void RegisterElementEventHandler(ElementEventHandler* eventHandler, EElementInteraction::Type interactionType);
    void UnregisterElementEventHandler(ElementEventHandler* eventHandler);

    void ProcessWindowEvent(const sf::Event& _oSFEvent, const std::vector<const Camera*>& windowCameras);

private:

    bool CheckElementEventHandlerRegistration(ElementEventHandler* eventHandler);

    void BeginInteractions();
    bool ProcessEventListeners(const sf::Event& _oSFEvent);
    bool ProcessCameraElementInteractions(const sf::Event& _oSFEvent, const Camera* camera);
    bool ProcessElementInteractions(const sf::Event& _oSFEvent);

private:

    std::vector<EventListener*> m_eventListeners;

    std::vector<ElementEventHandler*> m_elementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseFocusElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseClickElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseSelectionElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseScrollElementEventHandlers;
    std::vector<ElementEventHandler*> m_mouseDragElementEventHandlers;
    std::vector<ElementEventHandler*> m_rawSFEventElementEventHandlers;

    Element* m_elementMouseFocused;
    Element* m_elementMouseSelected;
    Element* m_elementMouseDragged;
    Vector2f m_lastMouseCoords;
};

}   // namespace gugu
