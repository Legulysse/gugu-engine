#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Time.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class DeltaTime
{
public:
    
    DeltaTime();
    DeltaTime(int _iDelta);
    DeltaTime(sf::Time _kTime);

    float   GetSeconds() const;
    int     GetMilliseconds() const;
    
    float   s() const;
    int     ms() const;

    operator sf::Time() const;

    DeltaTime& operator += (const DeltaTime& _kOther);
    DeltaTime& operator -= (const DeltaTime& _kOther);

    bool operator <  (const DeltaTime& _kOther) const;
    bool operator >  (const DeltaTime& _kOther) const;
    bool operator <= (const DeltaTime& _kOther) const;
    bool operator >= (const DeltaTime& _kOther) const;
    bool operator == (const DeltaTime& _kOther) const;
    bool operator != (const DeltaTime& _kOther) const;

private:

    int m_delta;
};

}	// namespace gugu
