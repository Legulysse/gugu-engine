#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/Debug/Logger.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class UnitTestResults
{
public:

    UnitTestResults(const std::string& logPathFile)
    {
        m_logger.SetFilePath(logPathFile);
        m_logger.SetConsoleOutput(true, true);
        m_logger.SetAutoflush(true);

        m_logger.Print("");
        m_logger.Print("**************** Unit Tests - Begin ****************");
    }

    void AddTestResults(const std::string& testHeader, size_t testCount, size_t successCount)
    {
        m_logger.SetConsoleOutput(false, false);
        m_logger.Print(StringFormat("Tests : {0} ({1}/{2})", testHeader, successCount, testCount));
        m_logger.SetConsoleOutput(true, true);

        m_testCount += testCount;
        m_successCount += successCount;
    }

    void PrintResults()
    {
        m_logger.Print("");
        m_logger.Print("**************** Unit Tests - End ****************");
        m_logger.Print("Nb Tests : " + ToString(m_testCount));
        m_logger.Print("Nb Succeeded : " + ToString(m_successCount));

        if (m_testCount - m_successCount > 0)
        {
            m_logger.Print("Nb Failed : " + ToString(m_testCount - m_successCount));
        }

        m_logger.Print("**************************************************");
        m_logger.Print("");
    }

private:

    Logger m_logger;

    size_t m_testCount = 0;
    size_t m_successCount = 0;
};

class UnitTestHandler
{
public:

    UnitTestHandler(const std::string& header, const std::string& logPathFile, UnitTestResults* testResults)
    {
        m_header = header;
        m_testResults = testResults;

        m_totalTestCount = 0;
        m_totalSuccessCount = 0;

        m_runningSection = false;
        m_runningSubSection = false;
        m_subSectionTestCount = 0;
        m_subSectionSuccessCount = 0;

        m_logger.SetFilePath(logPathFile);
        m_logger.SetConsoleOutput(true, true);
        m_logger.SetAutoflush(true);

        m_logger.Print("");
        m_logger.Print("-------- Unit Tests : " + m_header + " --------");
    }

    void Finalize()
    {
        FinalizeSection();

        PrintResults();

        if (m_testResults)
        {
            m_testResults->AddTestResults(m_header, m_totalTestCount, m_totalSuccessCount);
        }
    }

    void PrintResults()
    {
        if (m_testResults)
        {
            m_logger.SetConsoleOutput(false, false);
        }

        m_logger.Print("");
        m_logger.Print("-------- Results --------");
        m_logger.Print("Nb Tests : " + ToString(m_totalTestCount));
        m_logger.Print("Nb Succeeded : " + ToString(m_totalSuccessCount));

        if (m_totalTestCount - m_totalSuccessCount > 0)
        {
            m_logger.Print("Nb Failed : " + ToString(m_totalTestCount - m_totalSuccessCount));
        }

        m_logger.Print("-------------------------");
        m_logger.Print("");

        if (m_testResults)
        {
            m_logger.SetConsoleOutput(true, true);
        }
    }

    void BeginSection(const std::string& title)
    {
        FinalizeSection();

        m_runningSection = true;

        m_logger.Print("Section : " + title);
    }

    void BeginSubSection(const std::string& title)
    {
        if (!m_runningSubSection && m_subSectionTestCount > 0)
        {
            m_logger.Print(StringFormat("WARNING: Mixing tests outside of a subsection followed by a subsection will mess results"));
        }

        FinalizeSubSection();

        m_runningSubSection = true;
        m_pendingSubSectionTitle = title;
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
            m_logger.Print(StringFormat("Test Failed: {1}({2}) -> {0}", expression, file, line));
        }

        return result;
    }

private:

    void FinalizeSection()
    {
        FinalizeSubSection();

        if (m_runningSection && m_subSectionTestCount > 0)
        {
            m_logger.Print(StringFormat("Sub Section : {0} ({1}/{2})", "<Default>", m_subSectionSuccessCount, m_subSectionTestCount));

            m_subSectionTestCount = 0;
            m_subSectionSuccessCount = 0;
        }

        m_runningSection = false;
    }

    void FinalizeSubSection()
    {
        if (m_runningSubSection && m_subSectionTestCount > 0)
        {
            m_logger.Print(StringFormat("Sub Section : {0} ({1}/{2})", m_pendingSubSectionTitle, m_subSectionSuccessCount, m_subSectionTestCount));
            m_pendingSubSectionTitle = "";

            m_subSectionTestCount = 0;
            m_subSectionSuccessCount = 0;
        }

        m_runningSubSection = false;
    }

private:

    std::string m_header;
    Logger m_logger;
    UnitTestResults* m_testResults;

    size_t m_totalTestCount;
    size_t m_totalSuccessCount;

    bool m_runningSection;
    bool m_runningSubSection;
    std::string m_pendingSubSectionTitle;
    size_t m_subSectionTestCount;
    size_t m_subSectionSuccessCount;
};

#define GUGU_UTEST_INIT(NAME, LOG_FILE, RESULTS_PTR)            \
    UnitTestHandler unitTestHandler(NAME, LOG_FILE, RESULTS_PTR)

#define GUGU_UTEST_FINALIZE()                                   \
    unitTestHandler.Finalize()

#define GUGU_UTEST_SECTION(NAME)                                \
    unitTestHandler.BeginSection(NAME)

#define GUGU_UTEST_SUBSECTION(NAME)                             \
    unitTestHandler.BeginSubSection(NAME)

#define GUGU_UTEST_CHECK(EXPRESSION)                            \
    unitTestHandler.LogTestResult((bool)(EXPRESSION),           \
        GUGU_STRINGIZE(EXPRESSION),                             \
        __FILE__,                                               \
        __LINE__)                              

}   // namespace gugu
