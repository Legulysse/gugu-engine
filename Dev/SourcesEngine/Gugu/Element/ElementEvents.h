#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace EElementEvent
{
    enum Type
    {
        None        = 0x0000,
        
        Disabled    = 0x0001,   // Disable Events on self
        Absorb      = 0x0002,   // Disable Events on children
        
        Destroyed   = 0x0003,   // Temporary hack to access callbacks

        RawSFEvent  = 0x0004,
        //Selection   = 0x0010,
        Focus       = 0x0020,
        Click       = 0x0040,
        Scroll      = 0x0080,
        Drag        = 0x0100,

        // Interactions (The element will register to window events automatically)
        OnMouseSelected,
        OnMouseDeselected,

        //TODO
        OnInteractionEnabled,
        OnInteractionDisabled,
        OnMouseEnter,
        OnMouseLeave,
        OnMousePressed,
        OnMouseReleased,
        OnMouseScrolled,
        OnMouseDragStart,
        OnMouseDragStop,
        OnMouseDragMove,
    };
}

namespace EElementInteraction
{
    enum Type
    {
        None        = 0x0000,

        Selection   = 0x0001,
        Focus       = 0x0002,
        Click       = 0x0004,
        Scroll      = 0x0008,
        Drag        = 0x0010,
    };
}

// TODO: disabled/absorb should be individual flags to not mess with interaction flags.
// TODO: split logic between interaction flags (selection, focus etc) and callback events (selected, focused, destroyed etc).
class ElementEvents
{
    friend class HandlerEvents;

public:

    ElementEvents(Element* element);
    virtual ~ElementEvents();

    Element* GetElement() const;

    void SetDependsOnPropagationList();
    void RegisterHandlerEvents(HandlerEvents* _pHandler);
    void UnregisterHandlerEvents();

    bool IsInteractionEnabled() const;
    bool HasInteraction(EElementInteraction::Type flag) const;

    // TODO: DEPRECATE ?
    void SetInteractionFlags(int _iFlags);
    void AddInteractionFlag(EElementEvent::Type _eFlag);
    void RemoveInteractionFlag(EElementEvent::Type _eFlag);

    // TODO: DEPRECATE ?
  //  int GetInteractionFlags() const;
  //  bool HasInteractionFlags() const;   //Return true if Interaction flags are set besides Disabled and Absorb
  //  bool HasInteractionFlag(EElementEvent::Type _eFlag) const;

    void AddCallback(EElementEvent::Type event, const Callback& callback);
    void RemoveCallbacks(EElementEvent::Type event);
    void RemoveCallbacks();

    void FireCallbacks(EElementEvent::Type event);

private:

    void RefreshInteractionFlags();

private:

    Element* m_element;
    HandlerEvents* m_handler;
    bool m_dependsOnPropagationList;
    bool m_disabled;
    int m_interactionFlags;

    struct CallbackInfos
    {
        EElementEvent::Type event;
        Callback callback;
    };
    std::vector<CallbackInfos> m_callbacks;
};

}   // namespace gugu
