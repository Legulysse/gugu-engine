#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"

#include <vector>

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
        Selection   = 0x0010,
        Focus       = 0x0020,
        Click       = 0x0040,
        Scroll      = 0x0080,
        Drag        = 0x0100,
    };
}

// TODO: disabled/absorb should be individual flags to not mess with interaction flags.
// TODO: split logic between interaction flags (selection, focus etc) and callback events (selected, focused, destroyed etc).
class ElementEvents
{
public:

    ElementEvents();
    virtual ~ElementEvents();

    void SetInteractionFlags(int _iFlags);
    void AddInteractionFlag(EElementEvent::Type _eFlag);
    void RemoveInteractionFlag(EElementEvent::Type _eFlag);

    int GetInteractionFlags() const;
    bool HasInteractionFlags() const;   //Return true if Interaction flags are set besides Disabled and Absorb
    bool HasInteractionFlag(EElementEvent::Type _eFlag) const;

    void AddCallback(EElementEvent::Type _eFlag, const Callback& callback);
    void RemoveCallbacks(EElementEvent::Type _eFlag);
    void RemoveCallbacks();

    void FireCallbacks(EElementEvent::Type _eFlag);

private:

    struct CallbackInfos
    {
        EElementEvent::Type interactionFlag;
        Callback callback;
    };

    int m_interactionFlags;
    std::vector<CallbackInfos> m_callbacks;
};

}   // namespace gugu
