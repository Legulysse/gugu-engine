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
    int32 ms() const;
    int64 micro() const;

    float unscaled_s() const;
    int32 unscaled_ms() const;
    int64 unscaled_micro() const;

private:

    sf::Time m_time;
    sf::Time m_unscaledTime;
    float m_seconds;
    float m_unscaledSeconds;
    float m_scale;
};

}   // namespace gugu
