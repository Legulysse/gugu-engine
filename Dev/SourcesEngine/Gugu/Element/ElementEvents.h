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
    
namespace EElementInteractionEvent
{
    enum Type
    {
        None        = 0x0000,
        
        //Disabled    = 0x0001,   // Disable Events on self
        //Absorb      = 0x0002,   // Disable Events on children
        
        //Destroyed   = 0x0003,   // Temporary hack to access callbacks

        RawSFEvent  = 0x0004,
        //Selection   = 0x0010,
        //Focus       = 0x0020,
        //Click       = 0x0040,
        //Scroll      = 0x0080,
        //Drag        = 0x0100,

        // Interactions (The element will register to window events automatically)
        MouseEnter,
        MouseLeave,
        MousePressed,
        MouseReleased,
        MouseSelected,
        MouseDeselected,
        MouseScrolled,
        MouseDragStart,
        MouseDragStop,
        MouseDragMove,

        //TODO
        InteractionEnabled,
        InteractionDisabled,
    };
}

namespace EElementEvent
{
    enum Type
    {
        None,
        Destroyed,
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
    // Common infos (except MouseLeave and MouseDeselected).
    Vector2f localPickingPosition = Vector2::Zero_f;
    Camera* camera = nullptr;

    // MouseScrolled infos.
    int scrollDelta = 0;
};

// TODO: disabled/absorb should be individual flags to not mess with interaction flags.
class ElementEvents //TODO: rename as ElementEventHandler
{
    friend class HandlerEvents;

public:

    using DelegateInteractionEvent = std::function<void(const ElementInteractionInfos&)>;

public:

    ElementEvents(Element* element);
    virtual ~ElementEvents();

    Element* GetElement() const;

//    void SetDependsOnPropagationList();
    //void RegisterHandlerEvents(HandlerEvents* _pHandler);
    void UnregisterHandlerEvents();

    void SetInteractionEnabled(bool enabled);
    bool IsInteractionEnabled() const;
    //bool HasInteraction(EElementInteraction::Type flag) const;

    // TODO: DEPRECATE ?
    void SetInteractionFlags(int _iFlags);
    void AddInteractionFlag(EElementInteractionEvent::Type _eFlag);
    void RemoveInteractionFlag(EElementInteractionEvent::Type _eFlag);

    // TODO: DEPRECATE ?
  //  int GetInteractionFlags() const;
  //  bool HasInteractionFlags() const;   //Return true if Interaction flags are set besides Disabled and Absorb
  //  bool HasInteractionFlag(EElementInteractionEvent::Type _eFlag) const;

    void AddCallback(EElementInteractionEvent::Type event, const DelegateInteractionEvent& callback);
    void RemoveCallbacks(EElementInteractionEvent::Type event);
    void FireCallbacks(EElementInteractionEvent::Type event, const ElementInteractionInfos& interactionInfos);

    void AddCallback(EElementEvent::Type event, const Callback& callback);
    void RemoveCallbacks(EElementEvent::Type event);
    void FireCallbacks(EElementEvent::Type event);

    void RemoveAllCallbacks();

    //void AddMouseSelectionCallback(const CallbackInteractionEvent& callback);
    //void OnMouseSelectedEvent(const ElementInteractionInfos& interactionInfos);
    //void OnMouseDeselectedEvent(const ElementInteractionInfos& interactionInfos);

    //void AddMouseScrollCallback(const CallbackInteractionEvent& callback);
    //void OnMouseScrolledEvent(const ElementInteractionInfos& interactionInfos);

private:

    void CheckRegistration(EElementInteractionEvent::Type event);
    void RefreshInteractionFlags();

private:

    Element* m_element;
    HandlerEvents* m_handler;
    bool m_dependsOnPropagationList;
    bool m_interactionEnabled;
    int m_interactionFlags;

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

    //std::vector<CallbackInteractionEvent> m_mouseSelectionCallbacks;
    //std::vector<CallbackInteractionEvent> m_mouseScrollCallbacks;
    //std::vector<CallbackElementEvent> m_elementEventCallbacks;
};

}   // namespace gugu
