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
    RemoveCallbacks();
    UnregisterHandlerEvents();
}

Element* ElementEvents::GetElement() const
{
    return m_element;
}

void ElementEvents::SetDependsOnPropagationList()
{
    if (m_handler)
    {
        GetLogEngine()->Print(ELog::Error, ELogEngine::Element, "An element should not be registered to a window event handler if it is used through an element propagation list");
    }

    m_dependsOnPropagationList = true;
}

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

void ElementEvents::AddCallback(EElementInteractionEvent::Type event, const CallbackInteractionEvent& callback)
{
    if (!callback || event == EElementInteractionEvent::None)
        return;

   // GetGameWindow()->GetHandlerEvents()->AddElementEventHandler(this);

    CallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    m_callbacks.push_back(kInfos);

    CheckRegistration(event);
    RefreshInteractionFlags();
}

void ElementEvents::RemoveCallbacks(EElementInteractionEvent::Type event)
{
    for (auto iteCallback = m_callbacks.begin(); iteCallback != m_callbacks.end();)
    {
        if (iteCallback->event == event)
        {
            iteCallback = m_callbacks.erase(iteCallback);
        }
        else
        {
             ++iteCallback;
        }
    }

    RefreshInteractionFlags();
}

void ElementEvents::RemoveCallbacks()
{
    m_callbacks.clear();
    //m_mouseScrollCallbacks.clear();
    //m_elementEventCallbacks.clear();

    RefreshInteractionFlags();
}

void ElementEvents::FireCallbacks(EElementInteractionEvent::Type event, const ElementInteractionInfos& interactionInfos)
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        if (m_callbacks[i].event == event)
        {
            m_callbacks[i].callback(interactionInfos);
        }
    }
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
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        if (event == EElementInteractionEvent::MouseSelected || event == EElementInteractionEvent::MouseDeselected)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Selection);
        }
        else if (event == EElementInteractionEvent::MouseScrolled)
        {
            GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::Scroll);
        }

        //TODO: other interactions
    }
}

void ElementEvents::RefreshInteractionFlags()
{
    m_interactionFlags = EElementInteraction::None;

    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        if (m_callbacks[i].event == EElementInteractionEvent::MouseSelected || m_callbacks[i].event == EElementInteractionEvent::MouseDeselected)
        {
            m_interactionFlags |= EElementInteraction::Selection;
        }

        //TODO: other interactions
    }
}

}   // namespace gugu
