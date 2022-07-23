////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementEvents.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Events/HandlerEvents.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
ElementEvents::ElementEvents(Element* element)
    : m_element(element)
    , m_handler(nullptr)
    , m_dependsOnPropagationList(false)
    , m_disabled(false)
    , m_interactionFlags(EElementInteractionEvent::None)
{
}

ElementEvents::~ElementEvents()
{
    RemoveAllCallbacks();
    UnregisterHandlerEvents();
}

Element* ElementEvents::GetElement() const
{
    return m_element;
}

//void ElementEvents::SetDependsOnPropagationList()
//{
//    if (m_handler)
//    {
//        GetLogEngine()->Print(ELog::Error, ELogEngine::Element, "An element should not be registered to a window event handler if it is used through an element propagation list");
//    }
//
//    m_dependsOnPropagationList = true;
//}

//void ElementEvents::RegisterHandlerEvents(HandlerEvents* _pHandler)
//{
//    if (m_dependsOnPropagationList)
//    {
//        GetLogEngine()->Print(ELog::Error, ELogEngine::Element, "An element should not be registered to a window event handler if it is used through an element propagation list");
//    }
//
//    if (_pHandler)
//    {
//        _pHandler->AddElementEventHandler(this);
//    }
//}

void ElementEvents::UnregisterHandlerEvents()
{
    if (m_handler)
    {
        m_handler->UnregisterElementEventHandler(this);
    }
}

bool ElementEvents::IsInteractionEnabled() const
{
    return m_element->IsVisible() && !m_disabled;
}

//bool ElementEvents::HasInteraction(EElementInteraction::Type flag) const
//{
//    return ((m_interactionFlags & flag) != EElementInteraction::None);
//}

void ElementEvents::SetInteractionFlags(int _iFlags)
{
    m_interactionFlags = _iFlags;
}

void ElementEvents::AddInteractionFlag(EElementInteractionEvent::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags | _eFlag;
}

void ElementEvents::RemoveInteractionFlag(EElementInteractionEvent::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags & ~_eFlag;
}

//int ElementEvents::GetInteractionFlags() const
//{
//    return m_interactionFlags;
//}

//bool ElementEvents::HasInteractionFlags() const
//{
//    return ((m_interactionFlags & ~EElementInteractionEvent::Absorb) & ~EElementInteractionEvent::Disabled) != EElementInteractionEvent::None;
//}
//
//bool ElementEvents::HasInteractionFlag(EElementInteractionEvent::Type _eFlag) const
//{
//    return ((m_interactionFlags & _eFlag) != EElementInteractionEvent::None);
//}

void ElementEvents::AddCallback(EElementInteractionEvent::Type event, const DelegateInteractionEvent& callback)
{
    if (!callback || event == EElementInteractionEvent::None)
        return;

   // GetGameWindow()->GetHandlerEvents()->AddElementEventHandler(this);

    InteractionCallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    m_interactionCallbacks.push_back(kInfos);

    CheckRegistration(event);
    RefreshInteractionFlags();
}

void ElementEvents::RemoveCallbacks(EElementInteractionEvent::Type event)
{
    for (auto iteCallback = m_interactionCallbacks.begin(); iteCallback != m_interactionCallbacks.end();)
    {
        if (iteCallback->event == event)
        {
            iteCallback = m_interactionCallbacks.erase(iteCallback);
        }
        else
        {
             ++iteCallback;
        }
    }

    RefreshInteractionFlags();
}

void ElementEvents::FireCallbacks(EElementInteractionEvent::Type event, const ElementInteractionInfos& interactionInfos)
{
    for (size_t i = 0; i < m_interactionCallbacks.size(); ++i)
    {
        if (m_interactionCallbacks[i].event == event)
        {
            m_interactionCallbacks[i].callback(interactionInfos);
        }
    }
}

void ElementEvents::AddCallback(EElementEvent::Type event, const Callback& callback)
{
    if (!callback || event == EElementEvent::None)
        return;

    ElementCallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    m_elementCallbacks.push_back(kInfos);
}

void ElementEvents::RemoveCallbacks(EElementEvent::Type event)
{
    for (auto iteCallback = m_elementCallbacks.begin(); iteCallback != m_elementCallbacks.end();)
    {
        if (iteCallback->event == event)
        {
            iteCallback = m_elementCallbacks.erase(iteCallback);
        }
        else
        {
            ++iteCallback;
        }
    }
}

void ElementEvents::FireCallbacks(EElementEvent::Type event)
{
    for (size_t i = 0; i < m_elementCallbacks.size(); ++i)
    {
        if (m_elementCallbacks[i].event == event)
        {
            m_elementCallbacks[i].callback();
        }
    }
}

void ElementEvents::RemoveAllCallbacks()
{
    m_elementCallbacks.clear();
    m_interactionCallbacks.clear();

    RefreshInteractionFlags();
}

//void ElementEvents::AddMouseSelectionCallback(const CallbackInteractionEvent& callback)
//{
//    if (!callback)
//        return;
//
//    GetGameWindow()->GetHandlerEvents()->AddMouseSelectionElementEventHandler(this);
//    m_mouseSelectionCallbacks.push_back(callback);
//}
//
//void ElementEvents::OnMouseSelectionEvent(const ElementInteractionInfos& interactionInfos)
//{
//    for (size_t i = 0; i < m_mouseSelectionCallbacks.size(); ++i)
//    {
//        m_mouseSelectionCallbacks[i](interactionInfos);
//    }
//}
//
//void ElementEvents::AddMouseScrollCallback(const CallbackInteractionEvent& callback)
//{
//    if (!callback)
//        return;
//
//    GetGameWindow()->GetHandlerEvents()->AddMouseScrollElementEventHandler(this);
//    m_mouseScrollCallbacks.push_back(callback);
//}
//
//void ElementEvents::OnMouseScrollEvent(const ElementInteractionInfos& interactionInfos)
//{
//    for (size_t i = 0; i < m_mouseScrollCallbacks.size(); ++i)
//    {
//        m_mouseScrollCallbacks[i](interactionInfos);
//    }
//}

void ElementEvents::CheckRegistration(EElementInteractionEvent::Type event)
{
    for (size_t i = 0; i < m_interactionCallbacks.size(); ++i)
    {
        if (event == EElementInteractionEvent::MouseEnter
            || event == EElementInteractionEvent::MouseLeave)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Focus);
        }
        else if (event == EElementInteractionEvent::MousePressed
            || event == EElementInteractionEvent::MouseReleased)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Click);
        }
        else if (event == EElementInteractionEvent::MouseSelected
            || event == EElementInteractionEvent::MouseDeselected)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Selection);
        }
        else if (event == EElementInteractionEvent::MouseScrolled)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Scroll);
        }
        else if (event == EElementInteractionEvent::MouseDragStart
            || event == EElementInteractionEvent::MouseDragStop
            || event == EElementInteractionEvent::MouseDragMove)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Drag);
        }
    }
}

void ElementEvents::RefreshInteractionFlags()
{
    m_interactionFlags = EElementInteraction::None;

    for (size_t i = 0; i < m_interactionCallbacks.size(); ++i)
    {
        if (m_interactionCallbacks[i].event == EElementInteractionEvent::MouseSelected || m_interactionCallbacks[i].event == EElementInteractionEvent::MouseDeselected)
        {
            m_interactionFlags |= EElementInteraction::Selection;
        }

        //TODO: other interactions
    }
}

}   // namespace gugu
