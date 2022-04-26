#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class UnitTestHandler
{
public:

    void Init(const std::string& name, const std::string& logPathFile)
    {
        m_nbTests = 0;
        m_nbSuccess = 0;

        m_logger.SetFile(logPathFile);
        m_logger.SetConsoleOutput(true, true);
        m_logger.SetAutoflush(true);

        m_logger.Print(ELog::Info, "**** Unit Tests : " + name + " ****");
    }

    void PrintResults()
    {
        m_logger.Print(ELog::Info, "**** Results ****");
        m_logger.Print(ELog::Info, "Nb Tests : " + ToString(m_nbTests));
        m_logger.Print(ELog::Info, "Nb Succeeded : " + ToString(m_nbSuccess));

        if (m_nbTests - m_nbSuccess > 0)
        {
            m_logger.Print(ELog::Info, "Nb Failed : " + ToString(m_nbTests - m_nbSuccess));
        }
    }

    void BeginSection(const std::string& name)
    {
        m_logger.Print(ELog::Info, "**** Main Section : " + name + " ****");
    }

    void BeginSubSection(const std::string& name)
    {
        m_logger.Print(ELog::Info, "**** Sub Section : " + name + " ****");
    }

    void LogTestResult(bool result, const std::string& expression, const std::string& file, size_t line)
    {
        ++m_nbTests;

        if (result)
        {
            ++m_nbSuccess;

            //m_logger.Print(ELog::Info, expression);
        }
        else
        {
            m_logger.Print(ELog::Error, StringFormat("{1} {2} : {0}", expression, file, line));
        }
    }

private:

    Logger m_logger;
    size_t m_nbTests;
    size_t m_nbSuccess;
};

#define GUGU_UTEST_INIT(NAME, LOG_FILE)     \
    UnitTestHandler unitTestHandler;        \
    unitTestHandler.Init(NAME, LOG_FILE);

#define GUGU_UTEST_FINALIZE()               \
    unitTestHandler.PrintResults();

#define GUGU_UTEST_SECTION(NAME)            \
    unitTestHandler.BeginSection(NAME);

#define GUGU_UTEST_SUBSECTION(NAME)         \
    unitTestHandler.BeginSubSection(NAME);

#define GUGU_UTEST_CHECK(EXPRESSION)            \
{                                               \
    bool testResult = EXPRESSION;               \
    unitTestHandler.LogTestResult(testResult,   \
        GUGU_STRINGIZE(EXPRESSION),             \
        __FILE__,                               \
        __LINE__);                              \
}

}   // namespace gugu