////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Core/DeltaTime.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
DeltaTime::DeltaTime()
{
    m_delta = 0;
}

DeltaTime::DeltaTime(int _iDelta)
{
    m_delta = _iDelta;
}

DeltaTime::DeltaTime(sf::Time _kTime)
{
    m_delta = _kTime.asMilliseconds();
}

float DeltaTime::GetSeconds() const
{
    return s();
}

int DeltaTime::GetMilliseconds() const
{
    return ms();
}

float DeltaTime::s() const
{
    return static_cast<float>(m_delta) * 0.001f;
}

int DeltaTime::ms() const
{
    return m_delta;
}

DeltaTime::operator sf::Time() const
{
    return sf::milliseconds(ms());
}

DeltaTime& DeltaTime::operator += (const DeltaTime& _kOther)
{
    m_delta += _kOther.m_delta;
    return *this;
}

DeltaTime& DeltaTime::operator -= (const DeltaTime& _kOther)
{
    m_delta -= _kOther.m_delta;
    return *this;
}

bool DeltaTime::operator <  (const DeltaTime& _kOther) const
{
    return m_delta < _kOther.m_delta;
}

bool DeltaTime::operator >  (const DeltaTime& _kOther) const
{
    return m_delta > _kOther.m_delta;
}

bool DeltaTime::operator <= (const DeltaTime& _kOther) const
{
    return m_delta <= _kOther.m_delta;
}

bool DeltaTime::operator >= (const DeltaTime& _kOther) const
{
    return m_delta >= _kOther.m_delta;
}

bool DeltaTime::operator == (const DeltaTime& _kOther) const
{
    return m_delta == _kOther.m_delta;
}

bool DeltaTime::operator != (const DeltaTime& _kOther) const
{
    return m_delta != _kOther.m_delta;
}

}   // namespace gugu
