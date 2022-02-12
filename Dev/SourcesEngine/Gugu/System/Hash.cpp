////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/Hash.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Hash::Hash()
{
    m_value = 0;
}

Hash::Hash(const std::string& _strValue)
{
    m_value = HashString(_strValue);
}

Hash::Hash(const char* _strValue)
{
    m_value = HashString(_strValue);
}

Hash::Hash(const Hash& _oRight)
{
    m_value = _oRight.m_value;
}

Hash::~Hash()
{
}

uint64 Hash::HashString(const std::string& _strValue)
{
    std::hash<std::string> hashFunction;
    uint64 result = hashFunction(_strValue);
    return result;
}

uint64 Hash::ToInt() const
{
    return m_value;
}

bool Hash::operator < (const Hash& _oRight) const
{
    return m_value < _oRight.m_value;
}

Hash& Hash::operator = (const Hash& _oRight)
{
    m_value = _oRight.m_value;
    return *this;
}

std::ostream& operator << (std::ostream& stream, const Hash& hash)
{
    return stream << hash.ToInt();
}

}   // namespace gugu
