////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Events/HandlerEvents.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Window/Camera.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Debug/Trace.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Core/DeltaTime.h"
#include "Gugu/Element/Element.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

HandlerEvents::HandlerEvents()
{
    m_elementMouseFocused = nullptr;
    m_elementMouseSelected = nullptr;
    m_elementMouseDragged = nullptr;

    m_lastMouseCoords = Vector2f(0.f, 0.f);
}

HandlerEvents::~HandlerEvents()
{
    for (EventListener* listener : m_eventListeners)
    {
        if (listener)
            listener->m_handler = nullptr;
    }
    m_eventListeners.clear();

    for (ElementEvents* elementEventHandler : m_elementEventHandlers)
    {
        if (elementEventHandler)
            elementEventHandler->m_handler = nullptr;
    }
    m_elementEventHandlers.clear();

    m_mouseFocusElementEventHandlers.clear();
    m_mouseClickElementEventHandlers.clear();
    m_mouseSelectionElementEventHandlers.clear();
    m_mouseScrollElementEventHandlers.clear();
    m_mouseDragElementEventHandlers.clear();
}

void HandlerEvents::AddEventListener(EventListener* _pEventListener)
{
    if (!_pEventListener || _pEventListener->m_handler == this)
        return;

    // Dont allow a listener to be registered on two handlers (this will also ensure it is not registered here already)
    _pEventListener->UnregisterHandlerEvents();

    _pEventListener->m_handler = this;
    m_eventListeners.push_back(_pEventListener);
}

void HandlerEvents::RemoveEventListener(EventListener* _pEventListener)
{
    if (!_pEventListener || _pEventListener->m_handler != this)
        return;

    _pEventListener->m_handler = nullptr;
    StdVectorRemove(m_eventListeners, _pEventListener);
}

bool HandlerEvents::IsEventListenerRegistered(EventListener* _pEventListener) const
{
    if (!_pEventListener)
        return false;

    return StdVectorContains(m_eventListeners, _pEventListener);
}

bool HandlerEvents::CheckElementEventHandlerRegistration(ElementEvents* elementEventHandler)
{
    if (!elementEventHandler)
        return false;

    if (elementEventHandler->m_handler != nullptr)
    {
        if (elementEventHandler->m_handler != this)
        {
            // Dont allow a listener to be registered on two handlers.
            return false;
        }

        // Element is already registered.
        return true;
    }

    elementEventHandler->m_handler = this;
    m_elementEventHandlers.push_back(elementEventHandler);
    return true;
}

void HandlerEvents::RegisterElementEventHandler(ElementEvents* elementEventHandler, EElementInteraction::Type interactionType)
{
    if (!CheckElementEventHandlerRegistration(elementEventHandler))
        return;

    if (interactionType == EElementInteraction::Focus)
    {
        if (!StdVectorContains(m_mouseFocusElementEventHandlers, elementEventHandler))
        {
            m_mouseFocusElementEventHandlers.push_back(elementEventHandler);
        }
    }
    else if (interactionType == EElementInteraction::Click)
    {
        if (!StdVectorContains(m_mouseClickElementEventHandlers, elementEventHandler))
        {
            m_mouseClickElementEventHandlers.push_back(elementEventHandler);
        }
    }
    else if (interactionType == EElementInteraction::Selection)
    {
        if (!StdVectorContains(m_mouseSelectionElementEventHandlers, elementEventHandler))
        {
            m_mouseSelectionElementEventHandlers.push_back(elementEventHandler);
        }
    }
    else if (interactionType == EElementInteraction::Scroll)
    {
        if (!StdVectorContains(m_mouseScrollElementEventHandlers, elementEventHandler))
        {
            m_mouseScrollElementEventHandlers.push_back(elementEventHandler);
        }
    }
    else if (interactionType == EElementInteraction::Drag)
    {
        if (!StdVectorContains(m_mouseDragElementEventHandlers, elementEventHandler))
        {
            m_mouseDragElementEventHandlers.push_back(elementEventHandler);
        }
    }
    else if (interactionType == EElementInteraction::RawSFEvent)
    {
        if (!StdVectorContains(m_rawSFEventElementEventHandlers, elementEventHandler))
        {
            m_rawSFEventElementEventHandlers.push_back(elementEventHandler);
        }
    }
}

void HandlerEvents::UnregisterElementEventHandler(ElementEvents* elementEventHandler)
{
    if (!elementEventHandler || elementEventHandler->m_handler != this)
        return;

    elementEventHandler->m_handler = nullptr;
    StdVectorRemove(m_elementEventHandlers, elementEventHandler);

    StdVectorRemove(m_mouseFocusElementEventHandlers, elementEventHandler);
    StdVectorRemove(m_mouseClickElementEventHandlers, elementEventHandler);
    StdVectorRemove(m_mouseSelectionElementEventHandlers, elementEventHandler);
    StdVectorRemove(m_mouseScrollElementEventHandlers, elementEventHandler);
    StdVectorRemove(m_mouseDragElementEventHandlers, elementEventHandler);
    StdVectorRemove(m_rawSFEventElementEventHandlers, elementEventHandler);
}

void HandlerEvents::ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<const Camera*>& windowCameras)
{
    bool propagateEvent = ProcessEventListeners(_oSFEvent);

    for (const Camera* camera : windowCameras)
    {
        if (propagateEvent)
        {
            BeginInteractions();
            propagateEvent = ProcessElementInteractions(_oSFEvent, camera);
        }
    }

    if (propagateEvent)
    {
        propagateEvent = ProcessElements(_oSFEvent);
    }
}

void HandlerEvents::BeginInteractions()
{
    GUGU_SCOPE_TRACE_MAIN("Begin Interactions");

    if (m_elementMouseFocused)
    {
        bool bFoundFocused = false;
        for (ElementEvents* elementEventHandler : m_mouseFocusElementEventHandlers)
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
        for (ElementEvents* elementEventHandler : m_mouseSelectionElementEventHandlers)
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
        for (ElementEvents* elementEventHandler : m_mouseDragElementEventHandlers)
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

bool HandlerEvents::ProcessEventListeners(const sf::Event& _oSFEvent)
{
    GUGU_SCOPE_TRACE_MAIN("Process EventListeners");

    //TODO: add a way to make easy on/off listening, particularly for piled States
    //TODO: invert true/false meaning

    for (EventListener* listener : m_eventListeners)
    {
        if (listener && !listener->OnSFEvent(_oSFEvent))
        {
            return false;
        }
    }

    return true;
}

bool HandlerEvents::ProcessElementInteractions(const sf::Event& _oSFEvent, const Camera* camera)
{
    GUGU_SCOPE_TRACE_MAIN("Process ElementInteractions");

    Vector2i oMouseCoords = GetGameWindow()->GetMousePixelCoords();
    bool bContinue = true;

    if (_oSFEvent.type == sf::Event::MouseMoved)
    {
        if (m_elementMouseDragged)
        {
            Vector2f parentCoords = Vector2f(oMouseCoords);
            if (m_elementMouseDragged->GetParent())
                parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

            m_elementMouseDragged->SetPosition(parentCoords - m_lastMouseCoords);

            Vector2f localPickedCoords = m_elementMouseDragged->TransformToLocal(Vector2f(oMouseCoords));

            ElementInteractionInfos interactionInfos;
            interactionInfos.localPickingPosition = localPickedCoords;
            interactionInfos.camera = camera;
            m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragMoved, interactionInfos);

            bContinue = false;
        }
        else
        {
            for (size_t i = m_mouseFocusElementEventHandlers.size(); i-- > 0;)
            {
                ElementEvents* elementEventHandler = m_mouseFocusElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Focus)
                    && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                {
                    if (m_elementMouseFocused != pElement)
                    {
                        if (m_elementMouseFocused)
                        {
                            ElementInteractionInfos interactionInfos;
                            m_elementMouseFocused->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseLeft, interactionInfos);
                        }

                        m_elementMouseFocused = pElement;

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseEntered, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            bContinue = false;
                            break;
                        }
                    }

                    // TODO: Should I handle a MouseMove event ?
                    bContinue = false;
                    break;
                }
            }

            //Nothing got the Focus, release the current one if needed
            if (bContinue && m_elementMouseFocused)
            {
                ElementInteractionInfos interactionInfos;
                m_elementMouseFocused->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseLeft, interactionInfos);

                m_elementMouseFocused = nullptr;
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            for (size_t i = m_mouseSelectionElementEventHandlers.size(); i-- > 0;)
            {
                ElementEvents* elementEventHandler = m_mouseSelectionElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Selection)
                    && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                {
                    if (m_elementMouseSelected != pElement)
                    {
                        if (m_elementMouseSelected)
                        {
                            ElementInteractionInfos interactionInfos;
                            m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDeselected, interactionInfos);
                        }

                        m_elementMouseSelected = pElement;

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseSelected, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            bContinue = false;
                            break;
                        }
                    }

                    // TODO: Should I handle a MouseReselected event ?
                    bContinue = false;
                    break;
                }
            }

            //Nothing got the selection, release the current one if needed
            if (bContinue && m_elementMouseSelected)
            {
                ElementInteractionInfos interactionInfos;
                m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDeselected, interactionInfos);

                m_elementMouseSelected = nullptr;
            }

            //Mouse Press
            if (bContinue)
            {
                for (size_t i = m_mouseClickElementEventHandlers.size(); i-- > 0;)
                {
                    ElementEvents* elementEventHandler = m_mouseClickElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Click)
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MousePressed, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            bContinue = false;
                            break;
                        }
                    }
                }
            }

            //Drag Start
            if (bContinue)
            {
                for (size_t i = m_mouseDragElementEventHandlers.size(); i-- > 0;)
                {
                    ElementEvents* elementEventHandler = m_mouseDragElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Drag)
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        m_elementMouseDragged = pElement;

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseDragBegan, interactionInfos);

                        Vector2f parentCoords = Vector2f(oMouseCoords);
                        if (m_elementMouseDragged->GetParent())
                            parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

                        m_lastMouseCoords = parentCoords - m_elementMouseDragged->GetPosition();

                        bContinue = false;
                        break;
                    }
                }
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseButtonReleased)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            //Drag Stop
            if (bContinue && m_elementMouseDragged)
            {
                // Final DragMove
                Vector2f parentCoords = Vector2f(oMouseCoords);
                if (m_elementMouseDragged->GetParent())
                    parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);

                m_elementMouseDragged->SetPosition(parentCoords - m_lastMouseCoords);

                Vector2f localPickedCoords = m_elementMouseDragged->TransformToLocal(Vector2f(oMouseCoords));

                ElementInteractionInfos dragMoveInteractionInfos;
                dragMoveInteractionInfos.localPickingPosition = localPickedCoords;
                dragMoveInteractionInfos.camera = camera;
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragMoved, dragMoveInteractionInfos);

                // Actual DragStop
                ElementInteractionInfos interactionInfos;
                interactionInfos.localPickingPosition = localPickedCoords;
                interactionInfos.camera = camera;
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragEnded, interactionInfos);

                m_elementMouseDragged = nullptr;
                bContinue = false;
            }

            //Mouse Release
            if (bContinue)
            {
                for (size_t i = m_mouseClickElementEventHandlers.size(); i-- > 0;)
                {
                    ElementEvents* elementEventHandler = m_mouseClickElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Click)
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseReleased, interactionInfos);

                        if (interactionInfos.absorbEvent)
                        {
                            bContinue = false;
                            break;
                        }
                    }
                }
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseWheelScrolled)
    {
        if (_oSFEvent.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
        {
            for (size_t i = m_mouseScrollElementEventHandlers.size(); i-- > 0;)
            {
                ElementEvents* elementEventHandler = m_mouseScrollElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (elementEventHandler->IsInteractionEnabled(EElementInteraction::Scroll)
                    && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                {
                    ElementInteractionInfos interactionInfos;
                    interactionInfos.scrollDelta = (int)std::round(_oSFEvent.mouseWheelScroll.delta);
                    interactionInfos.localPickingPosition = localPickedCoords;
                    interactionInfos.camera = camera;
                    elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseScrolled, interactionInfos);

                    if (interactionInfos.absorbEvent)
                    {
                        bContinue = false;
                        break;
                    }
                }
            }
        }
    }

    return bContinue;
}

bool HandlerEvents::ProcessElements(const sf::Event& _oSFEvent)
{
    GUGU_SCOPE_TRACE_MAIN("Process Elements");

    bool propagateEvent = true;

    for (size_t i = m_rawSFEventElementEventHandlers.size(); i-- > 0;)
    {
        ElementEvents* elementEventHandler = m_rawSFEventElementEventHandlers[i];

        if (elementEventHandler->IsInteractionEnabled(EElementInteraction::RawSFEvent))
        {
            ElementInteractionInfos interactionInfos;
            interactionInfos.rawSFEvent = &_oSFEvent;
            elementEventHandler->FireCallbacks(EElementInteractionEvent::RawSFEvent, interactionInfos);

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
