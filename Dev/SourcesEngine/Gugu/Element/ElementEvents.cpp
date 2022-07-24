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
    , m_interactionsEnabled(true)
    , m_interactionsFilter(EElementInteractionEvent::None)
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

void ElementEvents::UnregisterHandlerEvents()
{
    if (m_handler)
    {
        m_handler->UnregisterElementEventHandler(this);
    }
}

void ElementEvents::SetAllInteractionsEnabled(bool enabled)
{
    if (m_interactionsEnabled == enabled)
        return;

    m_interactionsEnabled = enabled;

    if (m_interactionsEnabled)
    {
        FireCallbacks(EElementEvent::InteractionsEnabled);
    }
    else
    {
        FireCallbacks(EElementEvent::InteractionsDisabled);
    }
}

void ElementEvents::SetInteractionEnabled(EElementInteraction::Type interactionType, bool enabled)
{
    if (enabled)
    {
        m_interactionsFilter = m_interactionsFilter & ~interactionType;
    }
    else
    {
        m_interactionsFilter = m_interactionsFilter | interactionType;
    }
}

bool ElementEvents::IsInteractionEnabled(EElementInteraction::Type interactionType) const
{
    return m_interactionsEnabled
        && (m_interactionsFilter & interactionType) == EElementInteraction::None
        && m_element->IsVisible(true);
}

void ElementEvents::AddCallback(EElementInteractionEvent::Type event, const DelegateInteractionEvent& callback)
{
    if (!callback || event == EElementInteractionEvent::None)
        return;

    InteractionCallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    m_interactionCallbacks.push_back(kInfos);

    CheckRegistration(event);
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
}

void ElementEvents::CheckRegistration(EElementInteractionEvent::Type event)
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
    else if (event == EElementInteractionEvent::RawSFEvent)
    {
        GetGameWindow()->GetHandlerEvents()->RegisterElementEventHandler(this, EElementInteraction::RawSFEvent);
    }
}

}   // namespace gugu
