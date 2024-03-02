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
        // This variation loses the original value (probably due to endianness ?) but is still a valid uuid in itself.
        static_assert(sizeof(GUID) == sizeof(UUID));
        uuid = reinterpret_cast<UUID&>(guid);

        // This variation allows an exact string version of the original GUID (but is slower).
        //std::string uuid;
        //uuid.resize(32, '0');
        //snprintf(uuid.data(), 9, "%08X", guid.Data1);
        //snprintf(uuid.data() + 8, 5, "%04hX", guid.Data2);
        //snprintf(uuid.data() + 12, 5, "%04hX", guid.Data3);

        //for (size_t i = 0; i < 8; ++i)
        //{
        //    snprintf(uuid.data() + 16 + i * 2, 3, "%02X", guid.Data4[i]);
        //}
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
    snprintf(uuid.data(), 17, "%016llX", m_data1);
    snprintf(uuid.data() + 16, 17, "%016llX", m_data2);
    return uuid;
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
