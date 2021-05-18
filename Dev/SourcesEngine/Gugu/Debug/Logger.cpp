////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// Includes

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
    m_consoleOutput = true;
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

void Logger::SetConsoleOutput(bool _bConsole)
{
    m_consoleOutput = _bConsole;
}

void Logger::SetActive(bool _bActive)
{
    m_isActive = _bActive;
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
    if (m_useTimestamp)
    {
        m_buffer << '[';
        m_buffer << GetTimestampAsString();
        m_buffer << "] ";
    }

    if (_eLogLevel == ELog::Info)
        m_buffer << "[Info] ";
    else if (_eLogLevel == ELog::Debug)
        m_buffer << "[Debug] ";
    else if (_eLogLevel == ELog::Warning)
        m_buffer << "[Warning] ";
    else if (_eLogLevel == ELog::Error)
        m_buffer << "[Error] ";

    if (!_strCategory.empty())
    {
        m_buffer << '[';
        m_buffer << _strCategory;
        m_buffer << "] ";
    }

    m_buffer << _strText;
    m_buffer << std::endl;

    if (m_autoFlush)
        FlushImpl();
}

void Logger::FlushImpl()
{
    if (m_consoleOutput)
    {
        std::cout << m_buffer.str();
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

    sf::Lock oLocker(m_mutex);

    std::string strCategory;
    if (_eLogEngineCategory == ELogEngine::Engine)
        strCategory = "Engine";
    else if (_eLogEngineCategory == ELogEngine::Resources)
        strCategory = "Resources";
    else if (_eLogEngineCategory == ELogEngine::Audio)
        strCategory = "Audio";

    PrintImpl(_eLogLevel, strCategory, _strText);
}

}   // namespace gugu
