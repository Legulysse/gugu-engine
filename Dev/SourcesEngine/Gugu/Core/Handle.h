#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
        
class Handle
{
public:

    Handle() {}
    explicit Handle(const void* ptr) : m_ptr(ptr) {}
    explicit Handle(size_t id) : m_id(id) {}
    Handle(const void* ptr, size_t id) : m_ptr(ptr), m_id(id) {}
    Handle(const Handle& right) : m_ptr(right.m_ptr), m_id(right.m_id) {}

    bool operator==(const Handle& right) const
    {
        return m_ptr == right.m_ptr && m_id == right.m_id;
    }

    bool operator!=(const Handle& right) const
    {
        return m_ptr != right.m_ptr || m_id != right.m_id;
    }

private:

    const void* m_ptr = nullptr;
    size_t m_id = 0;
};

}   // namespace gugu
