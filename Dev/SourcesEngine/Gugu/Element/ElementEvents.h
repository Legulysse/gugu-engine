#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"
#include "Gugu/Math/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
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
    
namespace EElementInteractionEvent
{
    enum Type
    {
        None,
        
        MouseEntered,
        MouseLeft,
        MousePressed,
        MouseReleased,
        MouseSelected,
        MouseDeselected,
        MouseScrolled,
        MouseDragBegan,
        MouseDragMoved,
        MouseDragEnded,
        RawSFEvent,
    };
}

namespace EElementEvent
{
    enum Type
    {
        None,

        Destroyed,
        InteractionsEnabled,
        InteractionsDisabled,
    };
}

namespace EElementInteraction
{
    enum Type
    {
        None        = 0x0000,

        Focus       = 0x0001,
        Click       = 0x0002,
        Selection   = 0x0004,
        Scroll      = 0x0008,
        Drag        = 0x0010,
        RawSFEvent  = 0x0020,
    };
}

struct ElementInteractionInfos
{
    // Common infos (for all events except RawSFEvent, MouseLeft and MouseDeselected).
    Vector2f localPickingPosition = Vector2::Zero_f;
    const Camera* camera = nullptr;

    // Scroll infos (for MouseScrolled).
    int scrollDelta = 0;

    // Raw Event infos (for RawSFEvent).
    const sf::Event* rawSFEvent = nullptr;

    // Optional return value (for MouseEntered, MouseSelected, MousePressed, MouseReleased, MouseScrolled, RawSFEvent).
    mutable bool absorbEvent = true;
};

class ElementEvents //TODO: rename as ElementEventHandler
{
    friend class HandlerEvents;

public:

    using DelegateInteractionEvent = std::function<void(const ElementInteractionInfos&)>;

public:

    ElementEvents(Element* element);
    virtual ~ElementEvents();

    Element* GetElement() const;

    void UnregisterHandlerEvents();

    void SetAllInteractionsEnabled(bool enabled);
    void SetInteractionEnabled(EElementInteraction::Type interactionType, bool enabled);
    bool IsInteractionEnabled(EElementInteraction::Type interactionType) const;

    void AddCallback(EElementInteractionEvent::Type event, const DelegateInteractionEvent& callback);
    void RemoveCallbacks(EElementInteractionEvent::Type event);
    void FireCallbacks(EElementInteractionEvent::Type event, const ElementInteractionInfos& interactionInfos);

    void AddCallback(EElementEvent::Type event, const Callback& callback);
    void RemoveCallbacks(EElementEvent::Type event);
    void FireCallbacks(EElementEvent::Type event);

    void RemoveAllCallbacks();

private:

    void CheckRegistration(EElementInteractionEvent::Type event);

private:

    Element* m_element;
    HandlerEvents* m_handler;
    bool m_interactionsEnabled;
    int m_interactionsFilter;

    struct InteractionCallbackInfos
    {
        EElementInteractionEvent::Type event;
        DelegateInteractionEvent callback;
    };
    std::vector<InteractionCallbackInfos> m_interactionCallbacks;

    struct ElementCallbackInfos
    {
        EElementEvent::Type event;
        Callback callback;
    };
    std::vector<ElementCallbackInfos> m_elementCallbacks;
};

}   // namespace gugu
