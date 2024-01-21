////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/Hash.h"

////////////////////////////////////////////////////////////////
// Includes

#include <sstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Hash::Hash()
{
    m_value = 0;
}

Hash::Hash(const std::string& value)
{
    m_value = HashString(value);
}

Hash::Hash(const char* value)
{
    m_value = HashString(value);
}

Hash::Hash(const Hash& right)
{
    m_value = right.m_value;
}

Hash::~Hash()
{
}

uint64 Hash::HashString(const std::string& value)
{
    std::hash<std::string> hashFunction;
    uint64 result = hashFunction(value);
    return result;
}

uint64 Hash::ToInt() const
{
    return m_value;
}

bool Hash::operator < (const Hash& right) const
{
    return m_value < right.m_value;
}

Hash& Hash::operator = (const Hash& right)
{
    m_value = right.m_value;
    return *this;
}

std::ostream& operator << (std::ostream& stream, const Hash& hash)
{
    return stream << hash.ToInt();
}

}   // namespace gugu
