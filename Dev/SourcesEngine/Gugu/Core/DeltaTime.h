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
    
    DeltaTime(const sf::Time& time, const sf::Time& unscaledTime, float scale);

    sf::Time GetTime() const;
    sf::Time GetUnscaledTime() const;
    float GetScale() const;

    float s() const;
    float ms() const;
    int64 micro() const;

    float unscaled_s() const;
    float unscaled_ms() const;
    int64 unscaled_micro() const;

    bool IsZero() const;

    static const DeltaTime Zero;

private:

    sf::Time m_time;
    sf::Time m_unscaledTime;
    float m_scale;

    float m_seconds;
    float m_unscaledSeconds;
    float m_milliseconds;
    float m_unscaledMilliseconds;
};

}   // namespace gugu
