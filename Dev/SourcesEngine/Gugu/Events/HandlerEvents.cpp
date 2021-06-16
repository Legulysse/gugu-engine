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

    m_interactiveElements.clear();
}

void HandlerEvents::AddEventListener(EventListener* _pEventListener)
{
    if (!_pEventListener)
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

void HandlerEvents::ProcessEventOnElements(const sf::Event& _oSFEvent, const std::vector<InteractiveElementEntry>& _vecRootElements)
{
    BeginEvent(_vecRootElements);
    ProcessEvent(_oSFEvent);
    FinishEvent();
}

void HandlerEvents::BeginEvent(const std::vector<InteractiveElementEntry>& _vecRootElements)
{
    GUGU_SCOPE_TRACE_MAIN("Begin Event");

    bool bFoundFocused = false;
    bool bFoundSelected = false;
    bool bFoundDragged = false;

    for (InteractiveElementEntry kEntry : _vecRootElements)
    {
        ParseElements(kEntry.element, kEntry.camera);
    }

    for (size_t i = 0; i < m_interactiveElements.size(); ++i)
    {
        if (m_interactiveElements[i].element == m_elementMouseFocused)
            bFoundFocused = true;
        if (m_interactiveElements[i].element == m_elementMouseSelected)
            bFoundSelected = true;
        if (m_interactiveElements[i].element == m_elementMouseDragged)
            bFoundDragged = true;
    }

    if (!bFoundFocused)
        m_elementMouseFocused = nullptr;
    if (!bFoundSelected)
        m_elementMouseSelected = nullptr;
    if (!bFoundDragged)
        m_elementMouseDragged = nullptr;
}

void HandlerEvents::FinishEvent()
{
    m_interactiveElements.clear();
}

void HandlerEvents::ParseElements(Element* _pElement, Camera* _pCamera)
{
    if (!_pElement->IsVisible())
        return;

    if (!_pElement->HasInteractionFlag(EInteraction::Absorb))
    {
        const std::vector<Element*>& listChildren = _pElement->GetChildren();

        auto iteChild = listChildren.rbegin();
        auto iteEnd = listChildren.rend();
        while (iteChild != iteEnd)
        {
            Element* pChild = *iteChild;
            ParseElements(pChild, _pCamera);

            ++iteChild;
        }
    }

    if (!_pElement->HasInteractionFlag(EInteraction::Disabled))
    {
        std::vector<Element*> vecPropagationList;
        _pElement->GetPropagationList(vecPropagationList);
        for (size_t i = 0; i < vecPropagationList.size(); ++i)
        {
            if (vecPropagationList[i]->IsVisible() && !vecPropagationList[i]->HasInteractionFlag(EInteraction::Disabled) && vecPropagationList[i]->HasInteractionFlags())
            {
                InteractiveElementEntry kEntry;
                kEntry.element = vecPropagationList[i];
                kEntry.camera = _pCamera;
                m_interactiveElements.push_back(kEntry);
            }
        }

        if (_pElement->HasInteractionFlags())
        {
            InteractiveElementEntry kEntry;
            kEntry.element = _pElement;
            kEntry.camera = _pCamera;
            m_interactiveElements.push_back(kEntry);
        }
    }
}

void HandlerEvents::ProcessEvent(const sf::Event& _oSFEvent)
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
            Vector2f fLocalCoord = Vector2f(oMouseCoords);
            if (m_elementMouseDragged->GetParent())
                fLocalCoord = m_elementMouseDragged->GetParent()->TransformToLocalFull(fLocalCoord);

            m_elementMouseDragged->SetPosition(fLocalCoord - m_lastMouseCoords);
            m_elementMouseDragged->OnMouseDragMove();

            if (m_elementMouseDragged->GetInteractions())
                m_elementMouseDragged->GetInteractions()->FireCallbacks(EInteraction::Drag);

            //m_oLastMouseCoords = oMouseCoords;

            bContinue = false;
        }
        else
        {
            for (size_t i = 0; i < m_interactiveElements.size(); ++i)
            {
                Element* pElement = m_interactiveElements[i].element;
                if (pElement->HasInteractionFlag(EInteraction::Focus) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
                {
                    if (m_elementMouseFocused != pElement)
                    {
                        if (m_elementMouseFocused)
                        {
                            m_elementMouseFocused->OnMouseLeave();
                            m_elementMouseFocused = nullptr;
                        }

                        m_elementMouseFocused = pElement;
                        m_elementMouseFocused->OnMouseEnter();

                        if (m_elementMouseFocused->GetInteractions())
                            m_elementMouseFocused->GetInteractions()->FireCallbacks(EInteraction::Focus);
                    }

                    bContinue = false;
                    break;
                }
            }

            //Nothing got the Focus, release the current one if needed
            if (bContinue && m_elementMouseFocused)
            {
                m_elementMouseFocused->OnMouseLeave();
                m_elementMouseFocused = nullptr;
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseButtonPressed)
    {
        if (_oSFEvent.mouseButton.button == sf::Mouse::Left)
        {
            for (size_t i = 0; i < m_interactiveElements.size(); ++i)
            {
                Element* pElement = m_interactiveElements[i].element;
                if (pElement->HasInteractionFlag(EInteraction::Selection) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
                {
                    if (m_elementMouseSelected != pElement)
                    {
                        if (m_elementMouseSelected)
                        {
                            m_elementMouseSelected->OnMouseDeselected();
                            m_elementMouseSelected = nullptr;
                        }

                        m_elementMouseSelected = pElement;

                        if (m_elementMouseSelected->OnMouseSelected())
                        {
                            if (m_elementMouseSelected->GetInteractions())
                                m_elementMouseSelected->GetInteractions()->FireCallbacks(EInteraction::Selection);
                        }
                    }

                    bContinue = false;
                    break;
                }
            }

            //Nothing got the selection, release the current one if needed
            if (bContinue && m_elementMouseSelected)
            {
                m_elementMouseSelected->OnMouseDeselected();
                m_elementMouseSelected = nullptr;
            }

            //Mouse Press
            if (bContinue)
            {
                for (size_t i = 0; i < m_interactiveElements.size(); ++i)
                {
                    Element* pElement = m_interactiveElements[i].element;
                    if (pElement->HasInteractionFlag(EInteraction::Click) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
                    {
                        pElement->OnMousePressed();
                        bContinue = false;
                        break;
                    }
                }
            }

            //Drag Start
            if (bContinue)
            {
                for (size_t i = 0; i < m_interactiveElements.size(); ++i)
                {
                    Element* pElement = m_interactiveElements[i].element;
                    if (pElement->HasInteractionFlag(EInteraction::Drag) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
                    {
                        m_elementMouseDragged = pElement;
                        m_elementMouseDragged->OnMouseDragStart();

                        Vector2f fLocalCoord = Vector2f(oMouseCoords);
                        if (m_elementMouseDragged->GetParent())
                            fLocalCoord = m_elementMouseDragged->GetParent()->TransformToLocalFull(fLocalCoord);
                        m_lastMouseCoords = fLocalCoord - m_elementMouseDragged->GetPosition();
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
                m_elementMouseDragged->OnMouseDragStop();
                m_elementMouseDragged = nullptr;
                bContinue = false;
            }

            //Mouse Release
            if (bContinue)
            {
                for (size_t i = 0; i < m_interactiveElements.size(); ++i)
                {
                    Element* pElement = m_interactiveElements[i].element;
                    if (pElement->HasInteractionFlag(EInteraction::Click) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
                    {
                        if (pElement->OnMouseReleased())
                        {
                            if (pElement->GetInteractions())
                                pElement->GetInteractions()->FireCallbacks(EInteraction::Click);
                        }

                        bContinue = false;
                        break;
                    }
                }
            }
        }
    }
    else if (_oSFEvent.type == sf::Event::MouseWheelMoved)
    {
        for (size_t i = 0; i < m_interactiveElements.size(); ++i)
        {
            Element* pElement = m_interactiveElements[i].element;
            if (pElement->HasInteractionFlag(EInteraction::Scroll) && m_interactiveElements[i].camera->IsMouseOverElement(oMouseCoords, pElement))
            {
                pElement->OnMouseScrolled(_oSFEvent.mouseWheel.delta);

                if (pElement->GetInteractions())
                    pElement->GetInteractions()->FireCallbacks(EInteraction::Scroll);

                bContinue = false;
                break;
            }
        }
    }

    if (bContinue)
    {
        for (size_t i = 0; i < m_interactiveElements.size(); ++i)
        {
            Element* pElement = m_interactiveElements[i].element;
            if (pElement->HasInteractionFlag(EInteraction::RawSFEvent))
            {
                if (!pElement->OnSFEvent(_oSFEvent))
                {
                    bContinue = false;
                    break;
                }
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
