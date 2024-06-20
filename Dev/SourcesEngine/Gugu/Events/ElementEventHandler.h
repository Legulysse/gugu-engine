#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Callback.h"
#include "Gugu/System/Handle.h"
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
    
namespace EInteractionEvent
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
        SizeChanged,
    };
}

namespace EInteractionType
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

struct InteractionInfos
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

class ElementEventHandler
{
    friend class WindowEventHandler;

public:

    using DelegateInteractionEvent = std::function<void(const InteractionInfos&)>;

public:

    ElementEventHandler(Element* element);
    virtual ~ElementEventHandler();

    Element* GetElement() const;

    void UnregisterWindowEventHandler();

    void SetAllInteractionsDisabled(bool disabled);
    void SetInteractionDisabled(EInteractionType::Type interactionType, bool disabled);
    bool IsInteractionDisabled(EInteractionType::Type interactionType) const;

    bool IsInteractionRegisteredAndEnabled(EInteractionType::Type interactionType) const;

    void AddCallback(EInteractionEvent::Type event, const DelegateInteractionEvent& callback);
    void RemoveCallbacks(EInteractionEvent::Type event);
    void FireCallbacks(EInteractionEvent::Type event, const InteractionInfos& interactionInfos);

    void AddCallback(EElementEvent::Type event, const Handle& handle, const Callback& callback);
    void RemoveCallbacks(EElementEvent::Type event, const Handle& handle);
    void FireCallbacks(EElementEvent::Type event);

    void RemoveAllCallbacks();

private:

    void CheckRegistration(EInteractionEvent::Type event);

private:

    Element* m_element;
    WindowEventHandler* m_handler;
    int m_registeredInteractions;
    int m_disabledInteractions;
    bool m_allInteractionsDisabled;
    bool m_processingInteractionCallbacks;
    bool m_processingElementCallbacks;

    struct InteractionCallbackInfos
    {
        EInteractionEvent::Type event;
        DelegateInteractionEvent callback;
    };
    std::vector<InteractionCallbackInfos> m_interactionCallbacks;

    struct ElementCallbackInfos
    {
        EElementEvent::Type event;
        Callback callback;
        Handle handle;
    };
    std::vector<ElementCallbackInfos> m_elementCallbacks;
};

}   // namespace gugu
