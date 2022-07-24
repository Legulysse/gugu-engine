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

class HandlerEvents
{
public:

    struct InteractiveElementEntry
    {
        Element* element;
        Camera* camera;
    };

    HandlerEvents();
    ~HandlerEvents();

    void AddEventListener(EventListener* _pEventListener);
    void RemoveEventListener(EventListener* _pEventListener);
    bool IsEventListenerRegistered(EventListener* _pEventListener) const;

    void RegisterElementEventHandler(ElementEvents* elementEventHandler, EElementInteraction::Type interactionType);
    void UnregisterElementEventHandler(ElementEvents* elementEventHandler);

    void ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<const Camera*>& windowCameras);

private:

    bool CheckElementEventHandlerRegistration(ElementEvents* elementEventHandler);

    void BeginInteractions();
    bool ProcessEventListeners(const sf::Event& _oSFEvent);
    bool ProcessElementInteractions(const sf::Event& _oSFEvent, const Camera* camera);
    bool ProcessElements(const sf::Event& _oSFEvent);

private:

    std::vector<EventListener*> m_eventListeners;

    std::vector<ElementEvents*> m_elementEventHandlers;
    std::vector<ElementEvents*> m_mouseFocusElementEventHandlers;
    std::vector<ElementEvents*> m_mouseClickElementEventHandlers;
    std::vector<ElementEvents*> m_mouseSelectionElementEventHandlers;
    std::vector<ElementEvents*> m_mouseScrollElementEventHandlers;
    std::vector<ElementEvents*> m_mouseDragElementEventHandlers;
    std::vector<ElementEvents*> m_rawSFEventElementEventHandlers;

    Element* m_elementMouseFocused;
    Element* m_elementMouseSelected;
    Element* m_elementMouseDragged;
    Vector2f m_lastMouseCoords;
};

}   // namespace gugu
