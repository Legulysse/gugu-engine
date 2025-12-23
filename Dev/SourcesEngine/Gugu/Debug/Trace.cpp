////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/Trace.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Time.h"

#include <fstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

void TraceGroup::Start()
{
    m_isActive = true;

    m_buffer << "{\"traceEvents\":[";
    m_isFirstLine = true;

    m_clock.restart();
}

void TraceGroup::Stop()
{
    m_buffer << "\n],\"meta_user\":\"gugu\",\"meta_cpu_count\":\"1\"}";

    m_isActive = false;

    std::ofstream kFile;
    kFile.open(StringFormat("Trace_{0}.json", GetLocalTimestampAsString(timeformat::Filename)), std::ios::out | std::ios::trunc);

    if (kFile)
    {
        kFile << m_buffer.str();
        kFile.close();
    }

    m_buffer.str("");
    m_buffer.clear();
}

bool TraceGroup::IsActive() const
{
    return m_isActive;
}

const sf::Clock& TraceGroup::GetClock() const
{
    return m_clock;
}

void TraceGroup::Trace(const std::string& _strDescription, const sf::Time& _kTimestamp, const sf::Time& _kElapsed)
{
    if (!m_isFirstLine)
    {
        m_buffer << ",";
    }
    m_isFirstLine = false;

    m_buffer << "\n{\"pid\":1,\"tid\":1,\"ts\":" << _kTimestamp.asMicroseconds() << ",\"dur\":" << _kElapsed.asMicroseconds() << ",\"ph\":\"X\",\"name\":\"" << _strDescription << "\"}";
}

ScopeTrace::ScopeTrace(TraceGroup* _pTraceGroup, const std::string& _strDescription)
{
    if (!_pTraceGroup || !_pTraceGroup->IsActive())
        return;

    m_traceGroup = _pTraceGroup;
    m_description = _strDescription;

    m_startTime = m_traceGroup->GetClock().getElapsedTime();
}

ScopeTrace::~ScopeTrace()
{
    if (!m_traceGroup)
        return;

    sf::Time kElapsedTime = m_traceGroup->GetClock().getElapsedTime() - m_startTime;
    m_traceGroup->Trace(m_description, m_startTime, kElapsedTime);
}

TraceGroup* GetTraceGroupMain()
{
    return GetEngine()->GetTraceGroupMain();
}

}   // namespace gugu
