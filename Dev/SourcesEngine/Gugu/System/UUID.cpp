////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/UUID.h"

////////////////////////////////////////////////////////////////
// Includes

#include <sstream>
#include <charconv>

#if defined(GUGU_OS_WINDOWS)
    #include <combaseapi.h>
#endif

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

UUID UUID::Generate()
{
    UUID uuid;

#if defined(GUGU_OS_WINDOWS)

    GUID guid;
    HRESULT hr = CoCreateGuid(&guid);
    if (SUCCEEDED(hr))
    {
        uuid.m_data1 = ((uint64_t)guid.Data1) << 32 | ((uint64_t)guid.Data2) << 16 | (uint64_t)guid.Data3;
        uuid.m_data2 = ((uint64_t)guid.Data4[0]) << 56
            | ((uint64_t)guid.Data4[1]) << 48
            | ((uint64_t)guid.Data4[2]) << 40
            | ((uint64_t)guid.Data4[3]) << 32
            | ((uint64_t)guid.Data4[4]) << 24
            | ((uint64_t)guid.Data4[5]) << 16
            | ((uint64_t)guid.Data4[6]) << 8
            | (uint64_t)guid.Data4[7];
    }

#endif

    return uuid;
}

UUID UUID::FromString(const std::string& value)
{
    UUID uuid;
    std::from_chars(value.data(), value.data() + 16, uuid.m_data1, 16);
    std::from_chars(value.data() + 16, value.data() + 16 + 16, uuid.m_data2, 16);
    return uuid;
}

std::string UUID::ToString() const
{
    std::string uuid;
    uuid.resize(32, '0');
    std::to_chars(uuid.data(), uuid.data() + 16, m_data1, 16);
    std::to_chars(uuid.data() + 16, uuid.data() + 16 + 16, m_data2, 16);
    return uuid;
}

bool UUID::IsZero() const
{
    return m_data1 == 0 && m_data2 == 0;
}

bool UUID::operator == (const UUID& right) const
{
    return m_data1 == right.m_data1 && m_data2 == right.m_data2;
}

bool UUID::operator != (const UUID& right) const
{
    return m_data1 != right.m_data1 || m_data2 != right.m_data2;
}

bool UUID::operator < (const UUID& right) const
{
    return m_data1 < right.m_data1 || (m_data1 == right.m_data1 && m_data2 < right.m_data2);
}

UUID GenerateUUID()
{
    return UUID::Generate();
}

std::string GenerateUUIDAsString()
{
    return UUID::Generate().ToString();
}

std::ostream& operator << (std::ostream& stream, const UUID& uuid)
{
    return stream << uuid.ToString();
}

}   // namespace gugu
