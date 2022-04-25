#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Callback.h"

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

#include <string>
#include <sstream>
#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
    
namespace ELog {
    enum Type
    {
        Debug,
        Info,
        Warning,
        Error,
    };
}

class Logger
{
public:

    using DelegateLog = std::function<void(const std::string& timestamp, ELog::Type level, const std::string& category, const std::string& text)>;

public:
    
    Logger();
    virtual ~Logger();
    
    void SetFile(const std::string& _strFilePath);
    void SetAutoflush(bool _bFlush);
    void SetUseTimestamp(bool _bUseTimestamp);
    void SetConsoleOutput(bool _bConsole, bool outputInIDE);
    void SetActive(bool _bActive);

    void RegisterDelegate(void* handle, const DelegateLog& delegateLog);
    void UnregisterDelegate(void* handle);

    void Print(ELog::Type _eLogLevel, const std::string& _strText);

    void Flush();

protected:
    
    void PrintImpl(ELog::Type _eLogLevel, const std::string& _strCategory, const std::string& _strText);
    void FlushImpl();
        
protected:

    std::string m_filePath;
    bool m_autoFlush;
    bool m_useTimestamp;
    bool m_consoleOutput;
    bool m_consoleOutputIDE;
    bool m_isActive;

    struct DelegateInfos
    {
        DelegateLog delegateLog;
        void* handle;
    };
    std::vector<DelegateInfos> m_delegates;

    std::ostringstream m_buffer;

    sf::Mutex m_mutex;
};


namespace ELogEngine {
    enum Type
    {
        Engine,
        Resources,
        Audio,
    };
}

class LoggerEngine : public Logger
{
public:

    LoggerEngine() {}
    virtual ~LoggerEngine() {}

    using Logger::Print;    // Avoid Name Hiding
    void Print(ELog::Type _eLogLevel, ELogEngine::Type _eLogEngineCategory, const std::string& _strText);
};

LoggerEngine* GetLogEngine();

}   // namespace gugu
