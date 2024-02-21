////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Events/ElementEventHandler.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Element/Element.h"
#include "Gugu/Window/Window.h"
#include "Gugu/Events/WindowEventHandler.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

ElementEventHandler::ElementEventHandler(Element* element)
    : m_element(element)
    , m_handler(nullptr)
    , m_registeredInteractions(EInteractionType::None)
    , m_disabledInteractions(EInteractionType::None)
    , m_interactionsEnabled(true)
{
}

ElementEventHandler::~ElementEventHandler()
{
    RemoveAllCallbacks();
    UnregisterWindowEventHandler();
}

Element* ElementEventHandler::GetElement() const
{
    return m_element;
}

void ElementEventHandler::UnregisterWindowEventHandler()
{
    if (m_handler)
    {
        m_handler->UnregisterElementEventHandler(this);
    }
}

void ElementEventHandler::SetAllInteractionsEnabled(bool enabled)
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

void ElementEventHandler::SetInteractionEnabled(EInteractionType::Type interactionType, bool enabled)
{
    if (enabled)
    {
        m_disabledInteractions = m_disabledInteractions & ~interactionType;
    }
    else
    {
        m_disabledInteractions = m_disabledInteractions | interactionType;
    }
}

bool ElementEventHandler::IsInteractionDisabled(EInteractionType::Type interactionType) const
{
    return m_interactionsEnabled
        && (m_disabledInteractions & interactionType) == EInteractionType::None
        && m_element->IsVisible(true);
}

bool ElementEventHandler::IsInteractionRegisteredAndEnabled(EInteractionType::Type interactionType) const
{
    return m_interactionsEnabled
        && (m_registeredInteractions & interactionType) == interactionType
        && (m_disabledInteractions & interactionType) == EInteractionType::None
        && m_element->IsVisible(true);
}

void ElementEventHandler::AddCallback(EInteractionEvent::Type event, const DelegateInteractionEvent& callback)
{
    if (!callback || event == EInteractionEvent::None)
        return;

    InteractionCallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    m_interactionCallbacks.push_back(kInfos);

    CheckRegistration(event);
}

void ElementEventHandler::RemoveCallbacks(EInteractionEvent::Type event)
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

void ElementEventHandler::FireCallbacks(EInteractionEvent::Type event, const InteractionInfos& interactionInfos)
{
    for (size_t i = 0; i < m_interactionCallbacks.size(); ++i)
    {
        if (m_interactionCallbacks[i].event == event)
        {
            m_interactionCallbacks[i].callback(interactionInfos);
        }
    }
}

void ElementEventHandler::AddCallback(EElementEvent::Type event, const Handle& handle, const Callback& callback)
{
    if (!callback || event == EElementEvent::None)
        return;

    ElementCallbackInfos kInfos;
    kInfos.event = event;
    kInfos.callback = callback;
    kInfos.handle = handle;
    m_elementCallbacks.push_back(kInfos);
}

void ElementEventHandler::RemoveCallbacks(EElementEvent::Type event, const Handle& handle)
{
    for (auto iteCallback = m_elementCallbacks.begin(); iteCallback != m_elementCallbacks.end();)
    {
        if (iteCallback->event == event && iteCallback->handle == handle)
        {
            iteCallback = m_elementCallbacks.erase(iteCallback);
        }
        else
        {
            ++iteCallback;
        }
    }
}

void ElementEventHandler::FireCallbacks(EElementEvent::Type event)
{
    for (size_t i = 0; i < m_elementCallbacks.size(); ++i)
    {
        if (m_elementCallbacks[i].event == event)
        {
            m_elementCallbacks[i].callback();
        }
    }
}

void ElementEventHandler::RemoveAllCallbacks()
{
    m_elementCallbacks.clear();
    m_interactionCallbacks.clear();
}

void ElementEventHandler::CheckRegistration(EInteractionEvent::Type event)
{
    EInteractionType::Type interactionType = EInteractionType::None;

    switch (event)
    {
    case EInteractionEvent::MouseEntered:
    case EInteractionEvent::MouseLeft:
        interactionType = EInteractionType::Focus;
        break;
    case EInteractionEvent::MousePressed:
    case EInteractionEvent::MouseReleased:
        interactionType = EInteractionType::Click;
        break;
    case EInteractionEvent::MouseSelected:
    case EInteractionEvent::MouseDeselected:
        interactionType = EInteractionType::Selection;
        break;
    case EInteractionEvent::MouseScrolled:
        interactionType = EInteractionType::Scroll;
        break;
    case EInteractionEvent::MouseDragBegan:
    case EInteractionEvent::MouseDragMoved:
    case EInteractionEvent::MouseDragEnded:
        interactionType = EInteractionType::Drag;
        break;
    case EInteractionEvent::RawSFEvent:
        interactionType = EInteractionType::RawSFEvent;
        break;
    default:
        break;
    }

    if (interactionType != EInteractionType::None)
    {
        m_registeredInteractions = m_registeredInteractions | interactionType;
        GetGameWindow()->GetEventHandler()->RegisterElementEventHandler(this, interactionType);
    }
}

}   // namespace gugu
