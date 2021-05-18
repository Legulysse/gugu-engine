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
    Hash(const std::string& _strValue);
    Hash(const char* _strValue);
    Hash(const Hash& _oRight);
    ~Hash();

    static uint64 HashString(const std::string& _strValue);

    uint64 ToInt() const;

    bool operator < (const Hash& _oRight) const; // Used by std sorts
    Hash& operator = (const Hash& _oRight);

private:

    uint64 m_value;
};

std::ostream& operator << (std::ostream& stream, const Hash& hash); // Used by toString

}   // namespace gugu
