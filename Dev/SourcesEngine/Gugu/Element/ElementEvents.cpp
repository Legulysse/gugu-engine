////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementEvents.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
ElementEvents::ElementEvents()
{
    m_interactionFlags = EElementEvent::None;
}

ElementEvents::~ElementEvents()
{
    RemoveCallbacks();
}

void ElementEvents::SetInteractionFlags(int _iFlags)
{
    m_interactionFlags = _iFlags;
}

void ElementEvents::AddInteractionFlag(EElementEvent::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags | _eFlag;
}

void ElementEvents::RemoveInteractionFlag(EElementEvent::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags & ~_eFlag;
}

int ElementEvents::GetInteractionFlags() const
{
    return m_interactionFlags;
}

bool ElementEvents::HasInteractionFlags() const
{
    return ((m_interactionFlags & ~EElementEvent::Absorb) & ~EElementEvent::Disabled) != EElementEvent::None;
}

bool ElementEvents::HasInteractionFlag(EElementEvent::Type _eFlag) const
{
    return ((m_interactionFlags & _eFlag) != EElementEvent::None);
}

void ElementEvents::AddCallback(EElementEvent::Type _eFlag, const Callback& callback)
{
    if (!callback || _eFlag == EElementEvent::None)
        return;

    CallbackInfos kInfos;
    kInfos.interactionFlag = _eFlag;
    kInfos.callback = callback;
    m_callbacks.push_back(kInfos);
}

void ElementEvents::RemoveCallbacks(EElementEvent::Type _eFlag)
{
    for (auto iteCallback = m_callbacks.begin(); iteCallback != m_callbacks.end();)
    {
        if (iteCallback->interactionFlag == _eFlag)
        {
            iteCallback = m_callbacks.erase(iteCallback);
        }
        else
        {
             ++iteCallback;
        }
    }
}

void ElementEvents::RemoveCallbacks()
{
    m_callbacks.clear();
}

void ElementEvents::FireCallbacks(EElementEvent::Type _eFlag)
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        if (m_callbacks[i].interactionFlag == _eFlag)
        {
            m_callbacks[i].callback();
        }
    }
}

}   // namespace gugu
