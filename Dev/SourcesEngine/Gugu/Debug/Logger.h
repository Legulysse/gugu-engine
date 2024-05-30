#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Callback.h"

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

#include <string>
#include <sstream>
#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

//----------------------------------------------
// Logger

class Logger
{
public:
    
    Logger();
    virtual ~Logger();
    
    void SetFilePath(const std::string& filePath);
    void SetAutoflush(bool autoFlush);
    void SetConsoleOutput(bool output, bool outputInIDE);
    void SetActive(bool active);

    void Print(const std::string& text);

    void Flush();

protected:

    void PrintImpl_End();

    void FlushImpl();
        
protected:

    std::string m_filePath;
    bool m_autoFlush;
    bool m_consoleOutput;
    bool m_consoleOutputIDE;
    bool m_isActive;

    std::ostringstream m_buffer;

    sf::Mutex m_mutex;
};

//----------------------------------------------
// LoggerEngine

namespace ELog {
    enum Type
    {
        Debug,
        Info,
        Warning,
        Error,
    };
}

namespace ELogEngine {
    enum Type
    {
        Engine,
        Editor,
        Resources,
        Audio,
        Network,
        Element,
        Databinding,
    };
}

class LoggerEngine : public Logger
{
public:

    using DelegateLog = std::function<void(const std::string& timestamp, ELog::Type level, ELogEngine::Type category, const std::string& text)>;

public:

    LoggerEngine();
    virtual ~LoggerEngine();

    void Print(ELog::Type level, ELogEngine::Type category, const std::string& text);

    void SetUseTimestamp(bool useTimestamp);
    void IncrementFrameNumber();

    void RegisterDelegate(const void* handle, const DelegateLog& delegateLog);
    void UnregisterDelegate(const void* handle);

protected:

    bool m_useTimestamp;
    int m_frameNumber;

    struct DelegateInfos
    {
        DelegateLog delegateLog;
        const void* handle;
    };
    std::vector<DelegateInfos> m_delegates;
};

LoggerEngine* GetLogEngine();

}   // namespace gugu
