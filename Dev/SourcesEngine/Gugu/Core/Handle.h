#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
        
class Handle
{
public:

    Handle() {}
    explicit Handle(const void* ptr) : m_handleType(EHandleType::Pointer), m_ptr(ptr), m_uint64(0) {}
    explicit Handle(uint64_t uint64) : m_handleType(EHandleType::Uint64), m_ptr(nullptr), m_uint64(uint64) {}
    Handle(const void* ptr, uint64_t uint64) : m_handleType(EHandleType::PointerAndUint64), m_ptr(ptr), m_uint64(uint64) {}
    Handle(const Handle& right) : m_handleType(right.m_handleType), m_ptr(right.m_ptr), m_uint64(right.m_uint64) {}

    bool IsValid() const
    {
        return m_handleType != EHandleType::Invalid;
    }

    bool operator == (const Handle& right) const
    {
        return m_handleType == right.m_handleType && m_ptr == right.m_ptr && m_uint64 == right.m_uint64;
    }

    bool operator != (const Handle& right) const
    {
        return !(*this == right);
    }

    bool operator < (const Handle& right) const // Used by std sorts
    {
        switch (m_handleType)
        {
        default:
        case EHandleType::Invalid: return false; break;
        case EHandleType::Pointer: return m_ptr < right.m_ptr; break;
        case EHandleType::Uint64: return m_uint64 < right.m_uint64; break;
        case EHandleType::PointerAndUint64: return m_ptr < right.m_ptr || (m_ptr == right.m_ptr && m_uint64 < right.m_uint64); break;
        }
    }

private:

    enum EHandleType : uint8_t
    {
        Invalid,
        Pointer,
        Uint64,
        PointerAndUint64,
    };

    EHandleType m_handleType = EHandleType::Invalid;
    const void* m_ptr = nullptr;
    uint64_t m_uint64 = 0;
};

}   // namespace gugu
