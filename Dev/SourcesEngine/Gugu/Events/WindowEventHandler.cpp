////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Events/WindowEventHandler.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/System/Container.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Element/Element.h"

#include <SFML/Window/Event.hpp>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

WindowEventHandler::WindowEventHandler()
{
    m_elementMouseFocused = nullptr;
    m_elementMouseSelected = nullptr;
    m_elementMouseDragged = nullptr;

    m_lastMouseCoords = Vector2f(0.f, 0.f);
}

WindowEventHandler::~WindowEventHandler()
{
    for (EventListener* listener : m_eventListeners)
    {
        if (listener)
            listener->m_handler = nullptr;
    }
    m_eventListeners.clear();

    for (ElementEventHandler* elementEventHandler : m_elementEventHandlers)
    {
        if (elementEventHandler)
            elementEventHandler->m_handler = nullptr;
    }
    m_elementEventHandlers.clear();

    m_mouseFocusElementEventHandlers.clear();
    m_mouseClickElementEventHandlers.clear();
    m_mouseScrollElementEventHandlers.clear();
    m_rawSFEventElementEventHandlers.clear();
}

void WindowEventHandler::AddEventListener(EventListener* _pEventListener)
{
    if (!_pEventListener || _pEventListener->m_handler == this)
        return;

    // Dont allow a listener to be registered on two handlers (this will also ensure it is not registered here already)
    _pEventListener->UnregisterEventHandler();

    _pEventListener->m_handler = this;
    m_eventListeners.push_back(_pEventListener);
}

void WindowEventHandler::RemoveEventListener(EventListener* _pEventListener)
{
    if (!_pEventListener || _pEventListener->m_handler != this)
        return;

    _pEventListener->m_handler = nullptr;
    StdVectorRemove(m_eventListeners, _pEventListener);
}

bool WindowEventHandler::IsEventListenerRegistered(EventListener* _pEventListener) const
{
    if (!_pEventListener)
        return false;

    return StdVectorContains(m_eventListeners, _pEventListener);
}

bool WindowEventHandler::CheckElementEventHandlerRegistration(ElementEventHandler* eventHandler)
{
    if (!eventHandler)
        return false;

    if (eventHandler->m_handler != nullptr)
    {
        if (eventHandler->m_handler != this)
        {
            // Dont allow a listener to be registered on two handlers.
            return false;
        }

        // Element is already registered.
        return true;
    }

    eventHandler->m_handler = this;
    m_elementEventHandlers.push_back(eventHandler);
    return true;
}

void WindowEventHandler::RegisterElementEventHandler(ElementEventHandler* eventHandler, EInteractionType::Type interactionType)
{
    if (!CheckElementEventHandlerRegistration(eventHandler))
        return;

    if (interactionType == EInteractionType::Focus)
    {
        if (!StdVectorContains(m_mouseFocusElementEventHandlers, eventHandler))
        {
            m_mouseFocusElementEventHandlers.push_back(eventHandler);
        }
    }
    else if (interactionType == EInteractionType::Click
        || interactionType == EInteractionType::Selection
        || interactionType == EInteractionType::Drag)
    {
        if (!StdVectorContains(m_mouseClickElementEventHandlers, eventHandler))
        {
            m_mouseClickElementEventHandlers.push_back(eventHandler);
        }
    }
    else if (interactionType == EInteractionType::Scroll)
    {
        if (!StdVectorContains(m_mouseScrollElementEventHandlers, eventHandler))
        {
            m_mouseScrollElementEventHandlers.push_back(eventHandler);
        }
    }
    else if (interactionType == EInteractionType::RawSFEvent)
    {
        if (!StdVectorContains(m_rawSFEventElementEventHandlers, eventHandler))
        {
            m_rawSFEventElementEventHandlers.push_back(eventHandler);
        }
    }
}

void WindowEventHandler::UnregisterElementEventHandler(ElementEventHandler* eventHandler)
{
    if (!eventHandler || eventHandler->m_handler != this)
        return;

    eventHandler->m_handler = nullptr;
    StdVectorRemove(m_elementEventHandlers, eventHandler);

    StdVectorRemove(m_mouseFocusElementEventHandlers, eventHandler);
    StdVectorRemove(m_mouseClickElementEventHandlers, eventHandler);
    StdVectorRemove(m_mouseScrollElementEventHandlers, eventHandler);
    StdVectorRemove(m_rawSFEventElementEventHandlers, eventHandler);
}

void WindowEventHandler::ProcessWindowEvent(const sf::Event& event, const std::vector<const Camera*>& windowCameras)
{
    bool propagateEvent = ProcessEventListeners(event);

    for (const Camera* camera : windowCameras)
    {
        if (propagateEvent)
        {
            BeginInteractions();
            propagateEvent = ProcessCameraElementInteractions(event, camera);
        }
    }

    if (propagateEvent)
    {
        propagateEvent = ProcessElementInteractions(event);
    }
}

void WindowEventHandler::BeginInteractions()
{
    GUGU_SCOPE_TRACE_MAIN("Begin Interactions");

    if (m_elementMouseFocused)
    {
        bool bFoundFocused = false;
        for (ElementEventHandler* elementEventHandler : m_mouseFocusElementEventHandlers)
        {
            if (elementEventHandler->GetElement() == m_elementMouseFocused)
            {
                bFoundFocused = true;
                break;
            }
        }

        if (!bFoundFocused)
        {
            m_elementMouseFocused = nullptr;
        }
    }

    if (m_elementMouseSelected)
    {
        bool bFoundSelected = false;
        for (ElementEventHandler* elementEventHandler : m_mouseClickElementEventHandlers)
        {
            if (elementEventHandler->GetElement() == m_elementMouseSelected)
            {
                bFoundSelected = true;
                break;
            }
        }

        if (!bFoundSelected)
        {
            m_elementMouseSelected = nullptr;
        }
    }

    if (m_elementMouseDragged)
    {
        bool bFoundDragged = false;
        for (ElementEventHandler* elementEventHandler : m_mouseClickElementEventHandlers)
        {
            if (elementEventHandler->GetElement() == m_elementMouseDragged)
            {
                bFoundDragged = true;
                break;
            }
        }

        if (!bFoundDragged)
        {
            m_elementMouseDragged = nullptr;
        }
    }
}

bool WindowEventHandler::ProcessEventListeners(const sf::Event& event)
{
    GUGU_SCOPE_TRACE_MAIN("Process EventListeners");

    //TODO: add a way to make easy on/off listening, particularly for piled States
    //TODO: invert true/false meaning

    for (EventListener* listener : m_eventListeners)
    {
        if (listener && !listener->OnSFEvent(event))
        {
            return false;
        }
    }

    return true;
}

bool WindowEventHandler::ProcessCameraElementInteractions(const sf::Event& event, const Camera* camera)
{
    GUGU_SCOPE_TRACE_MAIN("Process CameraElementInteractions");

    bool propagateEvent = true;

    if (const auto mouseMovedEvent = event.getIf<sf::Event::MouseMoved>())
    {
        Vector2i mouseCoords = mouseMovedEvent->position;

        if (m_elementMouseDragged)
        {
            Vector2f parentCoords = Vector2f(mouseCoords);
            if (m_elementMouseDragged->GetParent())
                parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

            m_elementMouseDragged->SetPosition(parentCoords - m_lastMouseCoords);

            Vector2f localPickedCoords = m_elementMouseDragged->TransformToLocal(Vector2f(mouseCoords));

            InteractionInfos interactionInfos;
            interactionInfos.localPickingPosition = localPickedCoords;
            interactionInfos.camera = camera;
            m_elementMouseDragged->GetEvents()->FireCallbacks(EInteractionEvent::MouseDragMoved, interactionInfos);

            propagateEvent = false;
        }
        else
        {
            for (size_t i = m_mouseFocusElementEventHandlers.size(); i-- > 0;)
            {
                ElementEventHandler* elementEventHandler = m_mouseFocusElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Focus)
                    && camera->IsMouseOverElement(mouseCoords, pElement, localPickedCoords))
                {
                    if (m_elementMouseFocused != pElement)
                    {
                        if (m_elementMouseFocused)
                        {
                            InteractionInfos interactionInfos;
                            m_elementMouseFocused->GetEvents()->FireCallbacks(EInteractionEvent::MouseLeft, interactionInfos);
                        }

                        m_elementMouseFocused = pElement;

                        InteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EInteractionEvent::MouseEntered, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            propagateEvent = false;
                            break;
                        }
                    }

                    // TODO: Should I handle a MouseMove event ?
                    propagateEvent = false;
                    break;
                }
            }

            //Nothing got the Focus, release the current one if needed
            if (propagateEvent && m_elementMouseFocused)
            {
                InteractionInfos interactionInfos;
                m_elementMouseFocused->GetEvents()->FireCallbacks(EInteractionEvent::MouseLeft, interactionInfos);

                m_elementMouseFocused = nullptr;
            }
        }
    }
    else if (const auto mouseButtonPressedEvent = event.getIf<sf::Event::MouseButtonPressed>())
    {
        Vector2i mouseCoords = mouseButtonPressedEvent->position;

        if (mouseButtonPressedEvent->button == sf::Mouse::Button::Left)
        {
            bool clickedOnSelectedElement = false;

            for (size_t i = m_mouseClickElementEventHandlers.size(); i-- > 0;)
            {
                ElementEventHandler* elementEventHandler = m_mouseClickElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                bool selectionEnabled = elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Selection);
                bool clickEnabled = elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Click);
                bool dragEnabled = elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Drag);

                Vector2f localPickedCoords;
                if ((selectionEnabled || clickEnabled || dragEnabled)
                    && camera->IsMouseOverElement(mouseCoords, pElement, localPickedCoords))
                {
                    // Selection
                    if (selectionEnabled)
                    {
                        clickedOnSelectedElement = true;

                        if (m_elementMouseSelected != pElement)
                        {
                            if (m_elementMouseSelected)
                            {
                                InteractionInfos interactionInfos;
                                m_elementMouseSelected->GetEvents()->FireCallbacks(EInteractionEvent::MouseDeselected, interactionInfos);
                            }

                            m_elementMouseSelected = pElement;

                            InteractionInfos interactionInfos;
                            interactionInfos.localPickingPosition = localPickedCoords;
                            interactionInfos.camera = camera;
                            elementEventHandler->FireCallbacks(EInteractionEvent::MouseSelected, interactionInfos);

                            if (interactionInfos.absorbEvent)
                            {
                                propagateEvent = false;
                                break;
                            }
                        }

                        // TODO: Should I handle a MouseReselected event ?
                        propagateEvent = false;
                        break;
                    }

                    // Click
                    if (clickEnabled)
                    {
                        InteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EInteractionEvent::MousePressed, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            propagateEvent = false;
                            break;
                        }
                    }

                    // Drag Start
                    if (dragEnabled)
                    {
                        m_elementMouseDragged = pElement;

                        InteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EInteractionEvent::MouseDragBegan, interactionInfos);

                        Vector2f parentCoords = Vector2f(mouseCoords);
                        if (m_elementMouseDragged->GetParent())
                            parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

                        m_lastMouseCoords = parentCoords - m_elementMouseDragged->GetPosition();

                        propagateEvent = false;
                        break;
                    }
                }
            }

            //Nothing got the selection, release the current one if needed
            if (!clickedOnSelectedElement && m_elementMouseSelected)
            {
                InteractionInfos interactionInfos;
                m_elementMouseSelected->GetEvents()->FireCallbacks(EInteractionEvent::MouseDeselected, interactionInfos);

                m_elementMouseSelected = nullptr;
            }
        }
    }
    else if (const auto mouseButtonReleasedEvent = event.getIf<sf::Event::MouseButtonReleased>())
    {
        Vector2i mouseCoords = mouseButtonReleasedEvent->position;

        if (mouseButtonReleasedEvent->button == sf::Mouse::Button::Left)
        {
            //Drag Stop
            if (propagateEvent && m_elementMouseDragged)
            {
                // Final DragMove
                Vector2f parentCoords = Vector2f(mouseCoords);
                if (m_elementMouseDragged->GetParent())
                    parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

                m_elementMouseDragged->SetPosition(parentCoords - m_lastMouseCoords);

                Vector2f localPickedCoords = m_elementMouseDragged->TransformToLocal(Vector2f(mouseCoords));

                InteractionInfos dragMoveInteractionInfos;
                dragMoveInteractionInfos.localPickingPosition = localPickedCoords;
                dragMoveInteractionInfos.camera = camera;
                m_elementMouseDragged->GetEvents()->FireCallbacks(EInteractionEvent::MouseDragMoved, dragMoveInteractionInfos);

                // Actual DragStop
                InteractionInfos interactionInfos;
                interactionInfos.localPickingPosition = localPickedCoords;
                interactionInfos.camera = camera;
                m_elementMouseDragged->GetEvents()->FireCallbacks(EInteractionEvent::MouseDragEnded, interactionInfos);

                m_elementMouseDragged = nullptr;
                propagateEvent = false;
            }

            //Mouse Release
            if (propagateEvent)
            {
                for (size_t i = m_mouseClickElementEventHandlers.size(); i-- > 0;)
                {
                    ElementEventHandler* elementEventHandler = m_mouseClickElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    if (elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Click)
                        && camera->IsMouseOverElement(mouseCoords, pElement, localPickedCoords))
                    {
                        InteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EInteractionEvent::MouseReleased, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            propagateEvent = false;
                            break;
                        }
                    }
                }
            }
        }
    }
    else if (const auto mouseWheelScrolledEvent = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        Vector2i mouseCoords = mouseWheelScrolledEvent->position;

        if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical)
        {
            for (size_t i = m_mouseScrollElementEventHandlers.size(); i-- > 0;)
            {
                ElementEventHandler* elementEventHandler = m_mouseScrollElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::Scroll)
                    && camera->IsMouseOverElement(mouseCoords, pElement, localPickedCoords))
                {
                    // TODO: handle float scroll for high-precision mice ? (I need to test what happens with this kind of hardware).
                    InteractionInfos interactionInfos;
                    interactionInfos.scrollDelta = (int)std::round(mouseWheelScrolledEvent->delta);
                    interactionInfos.localPickingPosition = localPickedCoords;
                    interactionInfos.camera = camera;
                    elementEventHandler->FireCallbacks(EInteractionEvent::MouseScrolled, interactionInfos);

                    if (interactionInfos.absorbEvent)
                    {
                        propagateEvent = false;
                        break;
                    }
                }
            }
        }
    }

    return propagateEvent;
}

bool WindowEventHandler::ProcessElementInteractions(const sf::Event& event)
{
    GUGU_SCOPE_TRACE_MAIN("Process ElementInteractions");

    bool propagateEvent = true;

    for (size_t i = m_rawSFEventElementEventHandlers.size(); i-- > 0;)
    {
        ElementEventHandler* elementEventHandler = m_rawSFEventElementEventHandlers[i];

        if (elementEventHandler->IsInteractionRegisteredAndEnabled(EInteractionType::RawSFEvent))
        {
            InteractionInfos interactionInfos;
            interactionInfos.rawSFEvent = &event;
            elementEventHandler->FireCallbacks(EInteractionEvent::RawSFEvent, interactionInfos);

            if (interactionInfos.absorbEvent)
            {
                propagateEvent = false;
                break;
            }
        }
    }

    return propagateEvent;
}

}   // namespace gugu
