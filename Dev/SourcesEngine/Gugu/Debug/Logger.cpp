////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Engine.h"
#include "Gugu/System/SystemUtility.h"

#include <iostream>
#include <fstream>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
Logger::Logger()
{
    m_filePath = "";
    m_autoFlush = true;
    m_useTimestamp = true;
    m_consoleOutput = false;
    m_consoleOutputIDE = false;
    m_isActive = true;
}

Logger::~Logger()
{
    Flush();
}

void Logger::SetFile(const std::string& _strFilePath)
{
    sf::Lock oLocker(m_mutex);

    m_filePath = _strFilePath;

    if (!m_filePath.empty())
    {
        std::ofstream oFile;
        oFile.open(m_filePath.c_str(), std::ios::out | std::ios::trunc);

        if (oFile)
        {
            oFile.close();
        }
    }
}

void Logger::SetAutoflush(bool _bFlush)
{
    m_autoFlush = _bFlush;
}

void Logger::SetUseTimestamp(bool _bUseTimestamp)
{
    m_useTimestamp = _bUseTimestamp;
}

void Logger::SetConsoleOutput(bool _bConsole, bool outputInIDE)
{
    m_consoleOutput = _bConsole;
    m_consoleOutputIDE = outputInIDE;
}

void Logger::SetActive(bool _bActive)
{
    m_isActive = _bActive;
}

void Logger::RegisterDelegate(void* handle, const DelegateLog& delegateLog)
{
    DelegateInfos infos;
    infos.delegateLog = delegateLog;
    infos.handle = handle;

    m_delegates.push_back(infos);
}

void Logger::UnregisterDelegate(void* handle)
{
    StdVectorRemoveIf(m_delegates, [&](const auto& item) { return item.handle == handle; });
}

void Logger::Flush()
{
    sf::Lock oLocker(m_mutex);

    FlushImpl();
}

void Logger::Print(ELog::Type _eLogLevel, const std::string& _strText)
{
    if (!m_isActive)
        return;

    sf::Lock oLocker(m_mutex);

    PrintImpl(_eLogLevel, "", _strText);
}

void Logger::PrintImpl(ELog::Type _eLogLevel, const std::string& _strCategory, const std::string& _strText)
{
    static const std::map<ELog::Type, std::string> logLevelAsStr
    {
        { ELog::Debug, "[Debug] " },
        { ELog::Info, "[Info] " },
        { ELog::Warning, "[Warning] " },
        { ELog::Error, "[Error] " },
    };

    std::string timestamp = GetTimestampAsString();

    if (m_useTimestamp)
    {
        m_buffer << '[';
        m_buffer << timestamp;
        m_buffer << "] ";
    }

    m_buffer << logLevelAsStr.at(_eLogLevel);

    if (!_strCategory.empty())
    {
        m_buffer << '[';
        m_buffer << _strCategory;
        m_buffer << "] ";
    }

    m_buffer << _strText;
    m_buffer << std::endl;

    if (m_autoFlush)
    {
        FlushImpl();
    }

    for (const auto& delegateInfos : m_delegates)
    {
        delegateInfos.delegateLog(timestamp, _eLogLevel, _strCategory,_strText);
    }
}

void Logger::FlushImpl()
{
    if (m_consoleOutput)
    {
        // TODO: disable in release builds ?
        WriteInConsole(m_buffer.str(), m_consoleOutputIDE);
    }

    if (!m_filePath.empty())
    {
        std::ofstream oFile;
        oFile.open(m_filePath.c_str(), std::ios::out | std::ios::app);

        if (oFile)
        {
            oFile << m_buffer.str();
            oFile.close();
        }
    }

    m_buffer.str("");
    m_buffer.clear();
}


void LoggerEngine::Print(ELog::Type _eLogLevel, ELogEngine::Type _eLogEngineCategory, const std::string& _strText)
{
    if (!m_isActive)
        return;

    static const std::map<ELogEngine::Type, std::string> logCategoryAsStr
    {
        { ELogEngine::Engine, "Engine" },
        { ELogEngine::Resources, "Resources" },
        { ELogEngine::Audio, "Audio" },
    };

    sf::Lock oLocker(m_mutex);

    PrintImpl(_eLogLevel, logCategoryAsStr.at(_eLogEngineCategory), _strText);
}

LoggerEngine* GetLogEngine()
{
    return GetEngine()->GetLogEngine();
}

}   // namespace gugu
