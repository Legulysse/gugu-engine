#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DeltaTime
{
public:
    
    DeltaTime();
    DeltaTime(sf::Time time);

    sf::Time GetSFTime() const;

    float s() const;
    int32 ms() const;
    int64 micro() const;

private:

    sf::Time m_time;
    float m_seconds;
};

}   // namespace gugu
