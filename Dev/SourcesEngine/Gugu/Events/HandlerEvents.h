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

    //void AddElementEventHandler(ElementEvents* elementEventHandler);
    //void RemoveElementEventHandler(ElementEvents* elementEventHandler);

    void RegisterElementEventHandler(ElementEvents* elementEventHandler, EElementInteraction::Type interactionType);
    void UnregisterElementEventHandler(ElementEvents* elementEventHandler);

    //void AddMouseSelectionElementEventHandler(ElementEvents* elementEventHandler);
    //void RemoveMouseSelectionElementEventHandler(ElementEvents* elementEventHandler);

    //void AddMouseScrollElementEventHandler(ElementEvents* elementEventHandler);
    //void RemoveMouseScrollElementEventHandler(ElementEvents* elementEventHandler);

    void ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<const Camera*>& windowCameras);

private:

    bool CheckElementEventHandlerRegistration(ElementEvents* elementEventHandler);

    void BeginInteractions(/*const std::vector<InteractiveElementEntry>& _vecRootElements*/);
    bool ProcessEventListeners(const sf::Event& _oSFEvent);
    bool ProcessElementInteractions(const sf::Event& _oSFEvent, const Camera* camera);
    bool ProcessElements(const sf::Event& _oSFEvent);
    //void FinishEvent();

    //void ParseElements          (Element* _pRoot, Camera* _pCamera);
    //bool PropagateToListeners   (const sf::Event& _oSFEvent);

private:

    std::vector<EventListener*> m_eventListeners;

    std::vector<ElementEvents*> m_elementEventHandlers;
    std::vector<ElementEvents*> m_mouseFocusElementEventHandlers;
    std::vector<ElementEvents*> m_mouseClickElementEventHandlers;
    std::vector<ElementEvents*> m_mouseSelectionElementEventHandlers;
    std::vector<ElementEvents*> m_mouseScrollElementEventHandlers;
    std::vector<ElementEvents*> m_mouseDragElementEventHandlers;
    std::vector<ElementEvents*> m_rawSFEventElementEventHandlers;

    //std::vector<InteractiveElementEntry> m_interactiveElements;

    Element* m_elementMouseFocused;     //Mouse is over this element
    Element* m_elementMouseSelected;    //This element has been selected by the mouse
    Element* m_elementMouseDragged;
    Vector2f m_lastMouseCoords;
};

}   // namespace gugu
