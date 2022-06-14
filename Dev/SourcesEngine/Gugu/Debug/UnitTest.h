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
        m_totalTestCount = 0;
        m_totalSuccessCount = 0;

        m_runningSubSection = false;
        m_subSectionTestCount = 0;
        m_subSectionSuccessCount = 0;

        m_logger.SetFilePath(logPathFile);
        m_logger.SetConsoleOutput(true, true);
        m_logger.SetAutoflush(true);

        m_logger.Print("******** Unit Tests : " + name + " ********");
    }

    void PrintResults()
    {
        FinalizeSubSection();

        m_logger.Print("******** Results ********");
        m_logger.Print("Nb Tests : " + ToString(m_totalTestCount));
        m_logger.Print("Nb Succeeded : " + ToString(m_totalSuccessCount));

        if (m_totalTestCount - m_totalSuccessCount > 0)
        {
            m_logger.Print("Nb Failed : " + ToString(m_totalTestCount - m_totalSuccessCount));
        }
    }

    void BeginSection(const std::string& title)
    {
        FinalizeSubSection();

        m_logger.Print("**** Main Section : " + title + " ****");
    }

    void BeginSubSection(const std::string& title)
    {
        FinalizeSubSection();

        m_runningSubSection = true;
        m_pendingSubSectionTitle = title;
        m_subSectionTestCount = 0;
        m_subSectionSuccessCount = 0;
    }

    bool LogTestResult(bool result, const std::string& expression, const std::string& file, size_t line)
    {
        ++m_totalTestCount;
        ++m_subSectionTestCount;

        if (result)
        {
            ++m_totalSuccessCount;
            ++m_subSectionSuccessCount;
        }
        else
        {
            m_logger.Print(StringFormat("Failed: {1}({2}) -> {0}", expression, file, line));
        }

        return result;
    }

private:

    void FinalizeSubSection()
    {
        if (m_runningSubSection)
        {
            m_logger.Print(StringFormat("Sub Section : {0} ({1}/{2})", m_pendingSubSectionTitle, m_subSectionSuccessCount, m_subSectionTestCount));
            m_pendingSubSectionTitle = "";

            m_runningSubSection = false;
        }
    }

private:

    Logger m_logger;
    size_t m_totalTestCount;
    size_t m_totalSuccessCount;

    bool m_runningSubSection;
    std::string m_pendingSubSectionTitle;
    size_t m_subSectionTestCount;
    size_t m_subSectionSuccessCount;
};

#define GUGU_UTEST_INIT(NAME, LOG_FILE)     \
    UnitTestHandler unitTestHandler;        \
    unitTestHandler.Init(NAME, LOG_FILE)

#define GUGU_UTEST_FINALIZE()               \
    unitTestHandler.PrintResults()

#define GUGU_UTEST_SECTION(NAME)            \
    unitTestHandler.BeginSection(NAME)

#define GUGU_UTEST_SUBSECTION(NAME)         \
    unitTestHandler.BeginSubSection(NAME)

#define GUGU_UTEST_CHECK(EXPRESSION)            \
    unitTestHandler.LogTestResult((bool)(EXPRESSION),   \
        GUGU_STRINGIZE(EXPRESSION),             \
        __FILE__,                               \
        __LINE__)                              

}   // namespace gugu
