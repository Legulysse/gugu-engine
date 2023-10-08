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

    // Cache the conversions (it will be used a lot).
    m_seconds = static_cast<float>(static_cast<double>(m_time.asMicroseconds()) / 1000000.0);
    m_unscaledSeconds = static_cast<float>(static_cast<double>(m_unscaledTime.asMicroseconds()) / 1000000.0);
    m_milliseconds = static_cast<float>(static_cast<double>(m_time.asMicroseconds()) / 1000.0);
    m_unscaledMilliseconds = static_cast<float>(static_cast<double>(m_unscaledTime.asMicroseconds()) / 1000.0);
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

float DeltaTime::ms() const
{
    return m_milliseconds;
}

int64 DeltaTime::micro() const
{
    return m_time.asMicroseconds();
}

float DeltaTime::unscaled_s() const
{
    return m_unscaledSeconds;
}

float DeltaTime::unscaled_ms() const
{
    return m_unscaledMilliseconds;
}

int64 DeltaTime::unscaled_micro() const
{
    return m_unscaledTime.asMicroseconds();
}

}   // namespace gugu
