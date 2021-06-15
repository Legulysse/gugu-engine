#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace EInteraction
{
    enum Type
    {
        None        = 0x0000,
        
        Disabled    = 0x0001,   // Disable Events on self
        Absorb      = 0x0002,   // Disable Events on children
        
        RawSFEvent  = 0x0004,

        Selection   = 0x0010,
        Focus       = 0x0020,
        Click       = 0x0040,
        Scroll      = 0x0080,
        Drag        = 0x0100,
    };
}

class ElementInteractions
{
public:

    ElementInteractions();
    virtual ~ElementInteractions();

    void SetInteractionFlags(int _iFlags);
    void AddInteractionFlag(EInteraction::Type _eFlag);
    void RemoveInteractionFlag(EInteraction::Type _eFlag);

    int GetInteractionFlags() const;
    bool HasInteractionFlags() const;   //Return true if Interaction flags are set besides Disabled and Absorb
    bool HasInteractionFlag(EInteraction::Type _eFlag) const;

    void AddCallback(EInteraction::Type _eFlag, Callback callback);
    void RemoveCallbacks(EInteraction::Type _eFlag);
    void RemoveCallbacks();

    void FireCallbacks(EInteraction::Type _eFlag);

private:

    struct CallbackInfos
    {
        EInteraction::Type interactionFlag;
        Callback callback;
    };

    int m_interactionFlags;
    std::vector<CallbackInfos> m_callbacks;
};

}   // namespace gugu
