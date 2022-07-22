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
        MouseSelected,
        MouseDeselected,
        MouseScrolled,

        //TODO
        InteractionEnabled,
        InteractionDisabled,
        MouseEnter,
        MouseLeave,
        MousePressed,
        MouseReleased,
        MouseDragStart,
        MouseDragStop,
        MouseDragMove,
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

struct ElementInteractionInfos
{
    Vector2f localPickingPosition;
    Camera* camera;

    int scrollDelta;
};

// TODO: disabled/absorb should be individual flags to not mess with interaction flags.
// TODO: split logic between interaction flags (selection, focus etc) and callback events (selected, focused, destroyed etc).
class ElementEvents //TODO: rename as ElementEventHandler
{
    using CallbackInteractionEvent = std::function<void(const ElementInteractionInfos&)>;
    //using CallbackMouseScrollEvent = std::function<void(int)>;
    //using CallbackElementEvent = std::function<void()>;

    friend class HandlerEvents;

public:

    ElementEvents(Element* element);
    virtual ~ElementEvents();

    Element* GetElement() const;

    void SetDependsOnPropagationList();
    //void RegisterHandlerEvents(HandlerEvents* _pHandler);
    void UnregisterHandlerEvents();

    bool IsInteractionEnabled() const;
    //bool HasInteraction(EElementInteraction::Type flag) const;

    // TODO: DEPRECATE ?
    void SetInteractionFlags(int _iFlags);
    void AddInteractionFlag(EElementEvent::Type _eFlag);
    void RemoveInteractionFlag(EElementEvent::Type _eFlag);

    // TODO: DEPRECATE ?
  //  int GetInteractionFlags() const;
  //  bool HasInteractionFlags() const;   //Return true if Interaction flags are set besides Disabled and Absorb
  //  bool HasInteractionFlag(EElementEvent::Type _eFlag) const;

    void AddCallback(EElementEvent::Type event, const CallbackInteractionEvent& callback);
    void RemoveCallbacks(EElementEvent::Type event);
    void RemoveCallbacks();

    void FireCallbacks(EElementEvent::Type event, const ElementInteractionInfos& interactionInfos);

    //void AddMouseSelectionCallback(const CallbackInteractionEvent& callback);
    //void OnMouseSelectedEvent(const ElementInteractionInfos& interactionInfos);
    //void OnMouseDeselectedEvent(const ElementInteractionInfos& interactionInfos);

    //void AddMouseScrollCallback(const CallbackInteractionEvent& callback);
    //void OnMouseScrolledEvent(const ElementInteractionInfos& interactionInfos);

private:

    void CheckRegistration(EElementEvent::Type event);
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
        CallbackInteractionEvent callback;
    };
    std::vector<CallbackInfos> m_callbacks;

    //std::vector<CallbackInteractionEvent> m_mouseSelectionCallbacks;
    //std::vector<CallbackInteractionEvent> m_mouseScrollCallbacks;
    //std::vector<CallbackElementEvent> m_elementEventCallbacks;
};

}   // namespace gugu
