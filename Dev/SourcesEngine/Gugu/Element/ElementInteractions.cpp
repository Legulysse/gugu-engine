////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Element/ElementInteractions.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
        
ElementInteractions::ElementInteractions()
{
    m_interactionFlags = EInteraction::None;
}

ElementInteractions::~ElementInteractions()
{
    RemoveCallbacks();
}

void ElementInteractions::SetInteractionFlags(int _iFlags)
{
    m_interactionFlags = _iFlags;
}

void ElementInteractions::AddInteractionFlag(EInteraction::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags | _eFlag;
}

void ElementInteractions::RemoveInteractionFlag(EInteraction::Type _eFlag)
{
    m_interactionFlags = m_interactionFlags & ~_eFlag;
}

int ElementInteractions::GetInteractionFlags() const
{
    return m_interactionFlags;
}

bool ElementInteractions::HasInteractionFlags() const
{
    return ((m_interactionFlags & ~EInteraction::Absorb) & ~EInteraction::Disabled) != EInteraction::None;
}

bool ElementInteractions::HasInteractionFlag(EInteraction::Type _eFlag) const
{
    return ((m_interactionFlags & _eFlag) != EInteraction::None);
}

void ElementInteractions::AddCallback(EInteraction::Type _eFlag, Callback callback)
{
    if (!callback || _eFlag == EInteraction::None)
        return;

    CallbackInfos kInfos;
    kInfos.interactionFlag = _eFlag;
    kInfos.callback = callback;
    m_callbacks.push_back(kInfos);
}

void ElementInteractions::RemoveCallbacks(EInteraction::Type _eFlag)
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

void ElementInteractions::RemoveCallbacks()
{
    m_callbacks.clear();
}

void ElementInteractions::FireCallbacks(EInteraction::Type _eFlag)
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
