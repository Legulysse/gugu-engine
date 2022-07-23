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

    //m_interactiveElements.clear();

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

//void HandlerEvents::AddElementEventHandler(ElementEvents* elementEventHandler)
//{
//    if (!elementEventHandler || elementEventHandler->m_handler == this)
//        return;
//
//    // Dont allow a listener to be registered on two handlers (this will also ensure it is not registered here already)
//    elementEventHandler->UnregisterHandlerEvents();
//
//    elementEventHandler->m_handler = this;
//    m_elementEventHandlers.push_back(elementEventHandler);
//}
//
//void HandlerEvents::RemoveElementEventHandler(ElementEvents* elementEventHandler)
//{
//    if (!elementEventHandler || elementEventHandler->m_handler != this)
//        return;
//
//    elementEventHandler->m_handler = nullptr;
//    StdVectorRemove(m_elementEventHandlers, elementEventHandler);
//
//}

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
        m_mouseFocusElementEventHandlers.push_back(elementEventHandler);
    }
    else if (interactionType == EElementInteraction::Click)
    {
        m_mouseClickElementEventHandlers.push_back(elementEventHandler);
    }
    else if (interactionType == EElementInteraction::Selection)
    {
        m_mouseSelectionElementEventHandlers.push_back(elementEventHandler);
    }
    else if (interactionType == EElementInteraction::Scroll)
    {
        m_mouseScrollElementEventHandlers.push_back(elementEventHandler);
    }
    else if (interactionType == EElementInteraction::Drag)
    {
        m_mouseDragElementEventHandlers.push_back(elementEventHandler);
    }
    else if (interactionType == EElementInteraction::RawSFEvent)
    {
        m_rawSFEventElementEventHandlers.push_back(elementEventHandler);
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
//
//void HandlerEvents::AddMouseSelectionElementEventHandler(ElementEvents* elementEventHandler)
//{
//    if (!CheckElementEventHandlerRegistration(elementEventHandler))
//        return;
//
//    m_mouseSelectionElementEventHandlers.push_back(elementEventHandler);
//}
//
//void HandlerEvents::RemoveMouseSelectionElementEventHandler(ElementEvents* elementEventHandler)
//{
//    StdVectorRemove(m_mouseSelectionElementEventHandlers, elementEventHandler);
//}
//
//void HandlerEvents::AddMouseScrollElementEventHandler(ElementEvents* elementEventHandler)
//{
//    if (!CheckElementEventHandlerRegistration(elementEventHandler))
//        return;
//
//    m_mouseScrollElementEventHandlers.push_back(elementEventHandler);
//}
//
//void HandlerEvents::RemoveMouseScrollElementEventHandler(ElementEvents* elementEventHandler)
//{
//    StdVectorRemove(m_mouseScrollElementEventHandlers, elementEventHandler);
//}

void HandlerEvents::ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<InteractiveElementEntry>& _vecRootElements)
{
    BeginEvent(_vecRootElements);
    //ProcessEvent(_oSFEvent);
    //FinishEvent();

    for (InteractiveElementEntry kEntry : _vecRootElements)
    {
        ProcessEvent(_oSFEvent, kEntry.camera);
    }
}

void HandlerEvents::BeginEvent(const std::vector<InteractiveElementEntry>& _vecRootElements)
{
    GUGU_SCOPE_TRACE_MAIN("Begin Event");

    bool bFoundFocused = false;
    bool bFoundSelected = false;
    bool bFoundDragged = false;

    //for (InteractiveElementEntry kEntry : _vecRootElements)
    //{
    //    ParseElements(kEntry.element, kEntry.camera);
    //}

    //TODO: check all arrays for deprecated pointers
    for (ElementEvents* elementEventHandler : m_mouseFocusElementEventHandlers)
    {
        if (elementEventHandler->GetElement() == m_elementMouseFocused)
        {
            bFoundFocused = true;
            break;
        }
    }

    for (ElementEvents* elementEventHandler : m_mouseSelectionElementEventHandlers)
    {
        if (elementEventHandler->GetElement() == m_elementMouseSelected)
        {
            bFoundSelected = true;
            break;
        }
    }

    for (ElementEvents* elementEventHandler : m_mouseDragElementEventHandlers)
    {
        if (elementEventHandler->GetElement() == m_elementMouseDragged)
        {
            bFoundDragged = true;
            break;
        }
    }

    if (!bFoundFocused)
        m_elementMouseFocused = nullptr;

    if (!bFoundSelected)
        m_elementMouseSelected = nullptr;

    if (!bFoundDragged)
        m_elementMouseDragged = nullptr;

    //for (size_t i = 0; i < m_interactiveElements.size(); ++i)
    //{
    //    if (m_interactiveElements[i].element == m_elementMouseFocused)
    //        bFoundFocused = true;
    //    if (m_interactiveElements[i].element == m_elementMouseSelected)
    //        bFoundSelected = true;
    //    if (m_interactiveElements[i].element == m_elementMouseDragged)
    //        bFoundDragged = true;
    //}

    //if (!bFoundFocused)
    //    m_elementMouseFocused = nullptr;
    //if (!bFoundSelected)
    //    m_elementMouseSelected = nullptr;
    //if (!bFoundDragged)
    //    m_elementMouseDragged = nullptr;
}

void HandlerEvents::FinishEvent()
{
    //m_interactiveElements.clear();
}
//
//void HandlerEvents::ParseElements(Element* root, Camera* camera)
//{
//    if (!root->IsVisible())
//        return;
//
//    for (ElementEvents* elementEventHandler : m_elementEventHandlers)
//    {
//        if (elementEventHandler->IsInteractionEnabled())
//        {
//            Element* element = elementEventHandler->GetElement();
//
//            std::vector<ElementEvents*> vecPropagationList;
//            element->GetPropagationList(vecPropagationList);
//
//            for (size_t i = 0; i < vecPropagationList.size(); ++i)
//            {
//                if (vecPropagationList[i]->IsInteractionEnabled())
//                {
//                    InteractiveElementEntry kEntry;
//                    kEntry.element = vecPropagationList[i]->GetElement();
//                    kEntry.camera = camera;
//                    m_interactiveElements.push_back(kEntry);
//                }
//            }
//
//            InteractiveElementEntry kEntry;
//            kEntry.element = element;
//            kEntry.camera = camera;
//            m_interactiveElements.push_back(kEntry);
//        }
//    }
//}

void HandlerEvents::ProcessEvent(const sf::Event& _oSFEvent, Camera* camera)
{
    GUGU_SCOPE_TRACE_MAIN("Handler Process Event");

    if (!PropagateToListeners(_oSFEvent))
    {
        return;     //the event has been closed by a listener
    }

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
            m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragMove, interactionInfos);

            /*Vector2f fLocalCoord = Vector2f(oMouseCoords);
            if (m_elementMouseDragged->GetParent())
                fLocalCoord = m_elementMouseDragged->GetParent()->TransformToLocal(fLocalCoord);

            m_elementMouseDragged->SetPosition(fLocalCoord - m_lastMouseCoords);
            m_elementMouseDragged->OnMouseDragMove();

            if (m_elementMouseDragged->GetInteractions())
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::Drag);
            */
            //m_oLastMouseCoords = oMouseCoords;

            bContinue = false;
        }
        else
        {
            for (size_t i = 0; i < m_mouseFocusElementEventHandlers.size(); ++i)
            {
                ElementEvents* elementEventHandler = m_mouseFocusElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                //Element* pElement = m_interactiveElements[i].element;
                if (//pElement->HasInteractionFlag(EElementInteractionEvent::Focus) && 
                    elementEventHandler->IsInteractionEnabled()
                    && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                {
                    if (m_elementMouseFocused != pElement)
                    {
                        if (m_elementMouseFocused)
                        {
                            ElementInteractionInfos interactionInfos;
                            m_elementMouseFocused->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseLeave, interactionInfos);
                        }

                        //if (m_elementMouseFocused)
                        //{
                        //    m_elementMouseFocused->OnMouseLeave();
                        //    m_elementMouseFocused = nullptr;
                        //}

                        m_elementMouseFocused = pElement;
                        //m_elementMouseFocused->OnMouseEnter();

                        //if (m_elementMouseFocused->GetInteractions())
                        //    m_elementMouseFocused->GetInteractions()->FireCallbacks(EElementInteractionEvent::Focus);

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseEnter, interactionInfos);
                    }

                    bContinue = false;
                    break;
                }
            }

            //Nothing got the Focus, release the current one if needed
            if (bContinue && m_elementMouseFocused)
            {
                ElementInteractionInfos interactionInfos;
                m_elementMouseFocused->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseLeave, interactionInfos);

                m_elementMouseFocused = nullptr;

                //m_elementMouseFocused->OnMouseLeave();
                //m_elementMouseFocused = nullptr;
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            for (size_t i = 0; i < m_mouseSelectionElementEventHandlers.size(); ++i)
            {
                ElementEvents* elementEventHandler = m_mouseSelectionElementEventHandlers[i];
                Element* pElement = elementEventHandler->GetElement();

                Vector2f localPickedCoords;
                if (/*pElement->GetInteractions()->HasInteraction(EElementInteraction::Selection)
                    &&*/ 
                    elementEventHandler->IsInteractionEnabled()
                    && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                {
                    if (m_elementMouseSelected != pElement)
                    {
                        if (m_elementMouseSelected)
                        {
                            ElementInteractionInfos interactionInfos;
                            m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDeselected, interactionInfos);

                            //m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDeselected);

                            //m_elementMouseSelected->OnMouseDeselected();
                            //m_elementMouseSelected = nullptr;
                        }

                        m_elementMouseSelected = pElement;

                        //if (m_elementMouseSelected->OnMouseSelected())
                        //{
                        //    if (m_elementMouseSelected->GetInteractions())
                        //        m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::OnMouseSelected);
                        //}

                        //m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseSelected);

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseSelected, interactionInfos);
                    }

                    bContinue = false;
                    break;
                }
            }

            //Nothing got the selection, release the current one if needed
            if (bContinue && m_elementMouseSelected)
            {
                //m_elementMouseSelected->OnMouseDeselected();
                //m_elementMouseSelected = nullptr;

                ElementInteractionInfos interactionInfos;
                m_elementMouseSelected->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDeselected, interactionInfos);

                m_elementMouseSelected = nullptr;
            }

            //Mouse Press
            if (bContinue)
            {
                for (size_t i = 0; i < m_mouseClickElementEventHandlers.size(); ++i)
                {
                    ElementEvents* elementEventHandler = m_mouseClickElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    //Element* pElement = m_interactiveElements[i].element;
                    if (//pElement->HasInteractionFlag(EElementInteractionEvent::Click) && 
                        elementEventHandler->IsInteractionEnabled()
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        //pElement->OnMousePressed();

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MousePressed, interactionInfos);

                        bContinue = false;
                        break;
                    }
                }
            }

            //Drag Start
            if (bContinue)
            {
                for (size_t i = 0; i < m_mouseDragElementEventHandlers.size(); ++i)
                {
                    ElementEvents* elementEventHandler = m_mouseDragElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    if (//pElement->HasInteractionFlag(EElementInteractionEvent::Drag)
                        elementEventHandler->IsInteractionEnabled()
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        m_elementMouseDragged = pElement;

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseDragStart, interactionInfos);

                        //m_elementMouseDragged->OnMouseDragStart();

                        Vector2f parentCoords = Vector2f(oMouseCoords);
                        if (m_elementMouseDragged->GetParent())
                            parentCoords = m_elementMouseDragged->GetParent()->TransformToLocal(parentCoords);
                        m_lastMouseCoords = parentCoords - m_elementMouseDragged->GetPosition();

                        //m_lastMouseCoords = m_elementMouseDragged->GetPosition() + localPickedCoords;

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
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragMove, dragMoveInteractionInfos);

                // Actual DragStop
                ElementInteractionInfos interactionInfos;
                interactionInfos.localPickingPosition = localPickedCoords;
                interactionInfos.camera = camera;
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EElementInteractionEvent::MouseDragStop, interactionInfos);

                m_elementMouseDragged = nullptr;
                bContinue = false;
            }

            //Mouse Release
            if (bContinue)
            {
                for (size_t i = 0; i < m_mouseClickElementEventHandlers.size(); ++i)
                {
                    ElementEvents* elementEventHandler = m_mouseClickElementEventHandlers[i];
                    Element* pElement = elementEventHandler->GetElement();

                    Vector2f localPickedCoords;
                    //Element* pElement = m_interactiveElements[i].element;
                    if (//pElement->HasInteractionFlag(EElementInteractionEvent::Click) 
                        elementEventHandler->IsInteractionEnabled()
                        && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
                    {
                        //if (pElement->OnMouseReleased())
                        //{
                        //    if (pElement->GetInteractions())
                        //        pElement->GetInteractions()->FireCallbacks(EElementInteractionEvent::Click);
                        //}

                        ElementInteractionInfos interactionInfos;
                        interactionInfos.localPickingPosition = localPickedCoords;
                        interactionInfos.camera = camera;
                        elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseReleased, interactionInfos);

                        bContinue = false;
                        break;
                    }
                }
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseWheelMoved)
    {
        for (size_t i = 0; i < m_mouseScrollElementEventHandlers.size(); ++i)
        {
            ElementEvents* elementEventHandler = m_mouseScrollElementEventHandlers[i];
            Element* pElement = elementEventHandler->GetElement();

            Vector2f localPickedCoords;
            if (//pElement->HasInteractionFlag(EElementInteractionEvent::Scroll) &&
                elementEventHandler->IsInteractionEnabled()
                && camera->IsMouseOverElement(oMouseCoords, pElement, localPickedCoords))
            {
                //pElement->OnMouseScrolled(_oSFEvent.mouseWheel.delta);

                //if (pElement->GetInteractions())
                //    pElement->GetInteractions()->FireCallbacks(EElementInteractionEvent::Scroll);

                ElementInteractionInfos interactionInfos;
                interactionInfos.scrollDelta = _oSFEvent.mouseWheel.delta;
                interactionInfos.localPickingPosition = localPickedCoords;
                interactionInfos.camera = camera;
                elementEventHandler->FireCallbacks(EElementInteractionEvent::MouseScrolled, interactionInfos);

                bContinue = false;
                break;
            }
        }
    }

    if (bContinue)
    {
        for (size_t i = 0; i < m_rawSFEventElementEventHandlers.size(); ++i)
        {
            ElementEvents* elementEventHandler = m_rawSFEventElementEventHandlers[i];

            if (elementEventHandler->IsInteractionEnabled())
            {
                ElementInteractionInfos interactionInfos;
                interactionInfos.rawSFEvent = &_oSFEvent;
                elementEventHandler->FireCallbacks(EElementInteractionEvent::RawSFEvent, interactionInfos);

                //if (!pElement->OnSFEvent(_oSFEvent))
                //{
                //    bContinue = false;
                //    break;
                //}
            }
        }
    }
}

bool HandlerEvents::PropagateToListeners(const sf::Event& _oSFEvent)
{
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

}   // namespace gugu
