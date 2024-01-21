#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
class Hash
{
public:

    Hash();
    Hash(const std::string& value);
    Hash(const char* value);
    Hash(const Hash& right);
    ~Hash();

    static uint64 HashString(const std::string& value);

    uint64 ToInt() const;

    bool operator < (const Hash& right) const; // Used by std sorts
    Hash& operator = (const Hash& right);

private:

    uint64 m_value;
};

std::ostream& operator << (std::ostream& stream, const Hash& hash); // Used by toString

}   // namespace gugu
