#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Handle.h"

#include <functional>
#include <assert.h>

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

    ~Signal()
    {
        assert(!m_processingCallbacks);  // Destructor is called while callbacks are still being processed.
    }

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
        m_processingCallbacks = true;

        // Check the number of subscribers before the loop, to ignore new subscribers.
        size_t count = m_subscribers.size();

        for (size_t i = 0; i < count; ++i)
        {
            if (!m_subscribers[i].pendingRemove)
            {
                m_subscribers[i].callback(args...);
            }
        }

        FlushRemovedSubscribers();

        m_processingCallbacks = false;
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
    bool m_processingCallbacks = false;
};

}   // namespace gugu
