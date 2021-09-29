#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <SFML/System/Clock.hpp>

#include <sstream>

////////////////////////////////////////////////////////////////
// Macros

#define GUGU_SCOPE_TRACE(GROUP_PTR, ID) ScopeTrace LocalScopeTrace(GROUP_PTR, ID)
#define GUGU_SCOPE_TRACE_MAIN(ID) ScopeTrace LocalScopeTrace(GetTraceGroupMain(), ID)
#define GUGU_SCOPE_TRACE_(GROUP_PTR, ID, VARIABLE_NAME) ScopeTrace LocalScopeTrace_##VARIABLE_NAME(GROUP_PTR, ID)
#define GUGU_SCOPE_TRACE_MAIN_(ID, VARIABLE_NAME) ScopeTrace LocalScopeTrace_##VARIABLE_NAME(GetTraceGroupMain(), ID)

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class TraceGroup
{
public:

    void Start();
    void Stop();

    bool IsActive() const;
    const sf::Clock& GetClock() const;

    void Trace(const std::string& _strDescription, const sf::Time& _kTimestamp, const sf::Time& _kElapsed);

private:

    bool m_isActive = false;
    sf::Clock m_clock;

    std::ostringstream  m_buffer;
    bool m_isFirstLine = false;
};

class ScopeTrace
{
public:

    ScopeTrace(TraceGroup* _pTraceGroup, const std::string& _strDescription);
    ~ScopeTrace();

private:

    TraceGroup* m_traceGroup = nullptr;
    std::string m_description;
    sf::Time m_startTime;
};

TraceGroup* GetTraceGroupMain();

}   // namespace gugu
