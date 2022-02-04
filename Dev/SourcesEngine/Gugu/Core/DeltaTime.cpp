////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
DeltaTime::DeltaTime()
    : m_time(sf::Time::Zero)
    , m_seconds(0.f)
{
}

DeltaTime::DeltaTime(sf::Time time)
{
    m_time = time;

    // Cache the conversion (it will be used a lot).
    m_seconds = m_time.asSeconds();
}

sf::Time DeltaTime::GetSFTime() const
{
    return m_time;
}

float DeltaTime::s() const
{
    return m_seconds;
}

int DeltaTime::ms() const
{
    return m_time.asMilliseconds();
}

int64 DeltaTime::micro() const
{
    return m_time.asMicroseconds();
}

}   // namespace gugu
