#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Handle.h"

#include <functional>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename ...T>
class Signal
{
private:

    struct SubcriberInfos
    {
        Handle handle;
        std::function<void(T...)> callback;
        bool pendingRemove;
    };

public:

    void Subscribe(const Handle& handle, const std::function<void(T...)>& callback)
    {
        m_subscribers.push_back({ handle, callback, false });
    }

    void Unsubscribe(const Handle& handle)
    {
        for (size_t i = 0; i < m_subscribers.size(); ++i)
        {
            if (m_subscribers[i].handle == handle)
            {
                m_subscribers[i].pendingRemove = true;
            }
        }
    }

    void Notify(T... args)
    {
        size_t count = m_subscribers.size();
        for (size_t i = 0; i < count; ++i)
        {
            if (!m_subscribers[i].pendingRemove)
            {
                m_subscribers[i].callback(args...);
            }
        }

        FlushRemovedSubscribers();
    }

    void FlushRemovedSubscribers()
    {
        for (size_t i = m_subscribers.size(); i > 0; --i)
        {
            if (m_subscribers[i - 1].pendingRemove)
            {
                m_subscribers.erase(m_subscribers.begin() + i - 1);
            }
        }
    }

private:

    std::vector<SubcriberInfos> m_subscribers;
};

}   // namespace gugu
