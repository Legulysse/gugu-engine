////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/Debug/UnitTest.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"
#include "Gugu/System/String.h"

#include <SFML/System/Clock.hpp>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

UnitTestResults::UnitTestResults(const std::string& logPathFile)
{
    m_logger.SetFilePath(logPathFile);
    m_logger.SetIDEConsoleOutput(true);
    m_logger.SetAutoflush(true);

    m_logger.Print("");
    m_logger.Print("**************** Unit Tests - Begin ****************");
}

void UnitTestResults::AddTestResults(const std::string& testHeader, size_t testCount, size_t successCount)
{
    m_logger.SetIDEConsoleOutput(false);
    m_logger.Print(StringFormat("Tests : {0} ({1}/{2})", testHeader, successCount, testCount));
    m_logger.SetIDEConsoleOutput(true);

    m_testCount += testCount;
    m_successCount += successCount;
}

void UnitTestResults::AddExpectedErrorCount(size_t count)
{
    m_expectedErrorCount += count;
}

void UnitTestResults::AddExpectedWarningCount(size_t count)
{
    m_expectedWarningCount += count;
}

size_t UnitTestResults::GetExpectedErrorCount() const
{
    return m_expectedErrorCount;
}

size_t UnitTestResults::GetExpectedWarningCount() const
{
    return m_expectedWarningCount;
}

void UnitTestResults::PrintResults()
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

UnitTestHandler::UnitTestHandler(const std::string& header, const std::string& logPathFile, UnitTestResults* testResults)
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
    m_logger.SetIDEConsoleOutput(true);
    m_logger.SetAutoflush(true);

    m_logger.Print("");
    m_logger.Print("-------- Unit Tests : " + m_header + " --------");
}

void UnitTestHandler::Finalize()
{
    FinalizeSection();

    PrintResults();

    if (m_testResults)
    {
        m_testResults->AddTestResults(m_header, m_totalTestCount, m_totalSuccessCount);
    }
}

void UnitTestHandler::PrintResults()
{
    if (m_testResults)
    {
        m_logger.SetIDEConsoleOutput(false);
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
        m_logger.SetIDEConsoleOutput(true);
    }
}

void UnitTestHandler::BeginSection(const std::string& title)
{
    FinalizeSection();

    m_runningSection = true;

    m_logger.Print("Section : " + title);
}

void UnitTestHandler::BeginSubSection(const std::string& title)
{
    if (!m_runningSubSection && m_subSectionTestCount > 0)
    {
        m_logger.Print(StringFormat("WARNING: Mixing tests outside of a subsection followed by a subsection will mess results"));
    }

    FinalizeSubSection();

    m_runningSubSection = true;
    m_pendingSubSectionTitle = title;
}

void UnitTestHandler::AddExpectedErrorCount(size_t count)
{
    if (m_testResults)
    {
        m_testResults->AddExpectedErrorCount(count);
    }
}

void UnitTestHandler::AddExpectedWarningCount(size_t count)
{
    if (m_testResults)
    {
        m_testResults->AddExpectedWarningCount(count);
    }
}

bool UnitTestHandler::RunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line)
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

bool UnitTestHandler::SilentRunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line)
{
    if (!result)
    {
        ++m_totalTestCount;

        m_logger.Print(StringFormat("Test Failed: {1}({2}) -> {0}", expression, file, line));
    }

    return result;
}

bool UnitTestHandler::RunTestCompare(bool result, const std::string& left, const std::string& right, const std::string& expression, const std::string& file, size_t line)
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
        m_logger.Print(StringFormat("Test Failed: {1}({2}) -> Compared {3} to {4} : {0}", expression, file, line, left, right));
    }

    return result;
}

void UnitTestHandler::RunPerformanceTest(size_t warmupLoops, size_t loops, const std::function<void()>& executionMethod)
{
    for (size_t i = 0; i < warmupLoops; ++i)
    {
        executionMethod();
    }

    sf::Clock clock;

    for (size_t i = 0; i < loops; ++i)
    {
        executionMethod();
    }

    float totalTime = (float)clock.getElapsedTime().asMicroseconds() / 1000.f;
    float avgTime = totalTime / (float)loops;

    m_logger.Print(StringFormat("Performance Test : avg: {0} ms, total: {1} ms ({2} iterations)", avgTime, totalTime, loops));
}

void UnitTestHandler::FinalizeSection()
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

void UnitTestHandler::FinalizeSubSection()
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

}   // namespace gugu
