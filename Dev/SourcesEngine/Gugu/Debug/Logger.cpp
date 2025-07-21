////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Time.h"

#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

//----------------------------------------------
// Logger

Logger::Logger()
{
    m_filePath = "";
    m_autoFlush = true;
    m_consoleOutput = false;
    m_ideConsoleOutput = false;
    m_isActive = true;
}

Logger::~Logger()
{
    Flush();
}

void Logger::SetFilePath(const std::string& filePath)
{
    sf::Lock lock(m_mutex);

    m_filePath = filePath;

    if (!m_filePath.empty())
    {
        std::ofstream file;
        file.open(m_filePath.c_str(), std::ios::out | std::ios::trunc);

        if (file)
        {
            file.close();
        }
    }
}

void Logger::SetAutoflush(bool autoFlush)
{
    m_autoFlush = autoFlush;
}

void Logger::SetConsoleOutput(bool output)
{
    m_consoleOutput = output;
}

void Logger::SetIDEConsoleOutput(bool output)
{
    m_ideConsoleOutput = output;
}

void Logger::SetActive(bool active)
{
    m_isActive = active;
}

void Logger::Flush()
{
    sf::Lock lock(m_mutex);

    FlushImpl();
}

void Logger::Print(const std::string& text)
{
    if (!m_isActive)
        return;

    sf::Lock lock(m_mutex);

    m_buffer << text;

    PrintImpl_End();
}

void Logger::PrintImpl_End()
{
    m_buffer << std::endl;

    if (m_autoFlush)
    {
        FlushImpl();
    }
}

void Logger::FlushImpl()
{
#if !defined(GUGU_PRODUCTION_BUILD)
    if (m_consoleOutput)
    {
        WriteInConsole(m_buffer.str());
    }

    if (m_ideConsoleOutput)
    {
        WriteInIDEConsole(m_buffer.str());
    }
#endif

    if (!m_filePath.empty())
    {
        std::ofstream file;
        file.open(m_filePath.c_str(), std::ios::out | std::ios::app);

        if (file)
        {
            file << m_buffer.str();
            file.close();
        }
    }

    m_buffer.str("");
    m_buffer.clear();
}

//----------------------------------------------
// LoggerEngine

LoggerEngine::LoggerEngine()
    : m_useTimestamp(true)
    , m_frameNumber(0)
{
}

LoggerEngine::~LoggerEngine()
{
}

void LoggerEngine::Print(ELog::Type level, ELogEngine::Type category, const std::string& text)
{
    if (!m_isActive)
        return;

    sf::Lock lock(m_mutex);

    // Timestamp
    std::string timestamp = GetTimestampAsString();

    if (m_useTimestamp)
    {
        m_buffer << '[';
        m_buffer << timestamp;
        m_buffer << "] ";
    }

    // Frame
    m_buffer << '[';
    m_buffer << StringNumberFormat(m_frameNumber, 3);
    m_buffer << "] ";

    // Level
    static const std::map<ELog::Type, std::string> logLevelAsStr
    {
        { ELog::Debug, "[Debug] " },
        { ELog::Info, "[Info] " },
        { ELog::Warning, "[Warning] " },
        { ELog::Error, "[Error] " },
    };

    m_buffer << logLevelAsStr.at(level);

    // Category
    static const std::map<ELogEngine::Type, std::string> logCategoryAsStr
    {
        { ELogEngine::Engine, "[Engine] " },
        { ELogEngine::Editor, "[Editor] " },
        { ELogEngine::Resources, "[Resources] " },
        { ELogEngine::Audio, "[Audio] " },
        { ELogEngine::Network, "[Network] " },
        { ELogEngine::Element, "[Element] " },
        { ELogEngine::Databinding, "[Databinding] " },
    };

    m_buffer << logCategoryAsStr.at(category);

    // Finalize
    m_buffer << text;

    PrintImpl_End();

    // Callbacks
    for (const auto& delegateInfos : m_delegates)
    {
        delegateInfos.delegateLog(timestamp, level, category, text);
    }
}

void LoggerEngine::SetUseTimestamp(bool useTimestamp)
{
    m_useTimestamp = useTimestamp;
}

void LoggerEngine::IncrementFrameNumber()
{
    m_frameNumber = m_frameNumber == 999 ? 1 : m_frameNumber + 1;
}

void LoggerEngine::RegisterDelegate(const Handle& handle, const DelegateLog& delegateLog)
{
    DelegateInfos infos;
    infos.delegateLog = delegateLog;
    infos.handle = handle;

    m_delegates.push_back(infos);
}

void LoggerEngine::UnregisterDelegate(const Handle& handle)
{
    StdVectorRemoveIf(m_delegates, [&](const auto& item) { return item.handle == handle; });
}


LoggerEngine* GetLogEngine()
{
    return GetEngine()->GetLogEngine();
}

}   // namespace gugu
