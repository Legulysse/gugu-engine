#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class UUID
{
public:

    static UUID Generate();

    std::string ToString() const;

    bool operator == (const UUID& right) const;
    bool operator != (const UUID& right) const;
    bool operator < (const UUID& right) const; // Used by std sorts

private:

    UUID() = default;

private:

    uint64_t m_data1 = 0;
    uint64_t m_data2 = 0;
};

UUID GenerateUUID();
std::string GenerateUUIDAsString();

std::ostream& operator << (std::ostream& stream, const UUID& uuid); // Used by toString

}   // namespace gugu
