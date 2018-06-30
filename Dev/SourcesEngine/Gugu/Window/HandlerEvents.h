#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <list>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class EventListener;
    class Element;
    class Camera;
    class DeltaTime;
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

	void ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<InteractiveElementEntry>& _vecRootElements);
    
private:

    void BeginEvent		(const std::vector<InteractiveElementEntry>& _vecRootElements);
    void ProcessEvent   (const sf::Event& _oSFEvent);
    void FinishEvent	();

    void ParseElements          (Element* _pRoot, Camera* _pCamera);
    bool PropagateToListeners   (const sf::Event& _oSFEvent);

private:

    std::vector<EventListener*> m_eventListeners;
    std::vector<InteractiveElementEntry> m_interactiveElements;

    Element* m_elementMouseFocused;	    //Mouse is over this element
    Element* m_elementMouseSelected;	//This element has been selected by the mouse
    Element* m_elementMouseDragged;
    sf::Vector2f m_lastMouseCoords;
};

}   // namespace gugu
