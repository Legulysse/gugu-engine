////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

DeltaTime::DeltaTime(const sf::Time& time, const sf::Time& unscaledTime, float scale)
{
    m_time = time;
    m_unscaledTime = unscaledTime;
    m_scale = scale;

    // Cache the conversion (it will be used a lot).
    m_seconds = m_time.asSeconds();
    m_unscaledSeconds = m_unscaledTime.asSeconds();
}

sf::Time DeltaTime::GetTime() const
{
    return m_time;
}

sf::Time DeltaTime::GetUnscaledTime() const
{
    return m_unscaledTime;
}

float DeltaTime::GetScale() const
{
    return m_scale;
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

float DeltaTime::unscaled_s() const
{
    return m_unscaledSeconds;
}

int DeltaTime::unscaled_ms() const
{
    return m_unscaledTime.asMilliseconds();
}

int64 DeltaTime::unscaled_micro() const
{
    return m_unscaledTime.asMicroseconds();
}

}   // namespace gugu
