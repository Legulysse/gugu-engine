#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/System/String.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Vector2.h"
#include "Gugu/Debug/Logger.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

class UnitTestResults
{
public:

    UnitTestResults(const std::string& logPathFile);

    void AddTestResults(const std::string& testHeader, size_t testCount, size_t successCount);
    void PrintResults();

    void AddExpectedErrorCount(size_t count);
    void AddExpectedWarningCount(size_t count);
    size_t GetExpectedErrorCount() const;
    size_t GetExpectedWarningCount() const;

private:

    Logger m_logger;

    size_t m_testCount = 0;
    size_t m_successCount = 0;
    size_t m_expectedErrorCount = 0;
    size_t m_expectedWarningCount = 0;
};

class UnitTestHandler
{
public:

    UnitTestHandler(const std::string& header, const std::string& logPathFile, UnitTestResults* testResults);
    void Finalize();
    void PrintResults();

    void BeginSection(const std::string& title);
    void BeginSubSection(const std::string& title);

    void AddExpectedErrorCount(size_t count);
    void AddExpectedWarningCount(size_t count);

    bool RunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line);
    bool SilentRunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line);
    void RunPerformanceTest(size_t warmupLoops, size_t loops, const std::function<void()>& executionMethod);
    
    template<typename T1, typename T2>
    bool RunTestCompare(const T1& left, const T2& right, bool expectedResult, const std::string& expression, const std::string& file, size_t line)
    {
        // Note: Using T on both types requires strict type equality, which can be really annoying for writing tests.
        return RunTestCompareImpl((left == right) == expectedResult, ToString(left), ToString(right), expression, file, line);
    }

    template<typename T>
    bool RunTestCompare(const T& left, const T& right, float epsilon, bool expectedResult, const std::string& expression, const std::string& file, size_t line)
    {
        return RunTestCompareImpl(ApproxEqual(left, right, epsilon) == expectedResult, ToString(left), ToString(right), expression, file, line);
    }

private:

    bool RunTestCompareImpl(bool result, const std::string& left, const std::string& right, const std::string& expression, const std::string& file, size_t line);

    void FinalizeSection();
    void FinalizeSubSection();

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

#define GUGU_UTEST_ADD_EXPECTED_ERROR_COUNT(COUNT)              \
    unitTestHandler.AddExpectedErrorCount(COUNT)

#define GUGU_UTEST_ADD_EXPECTED_WARNING_COUNT(COUNT)            \
    unitTestHandler.AddExpectedWarningCount(COUNT)

#define GUGU_UTEST_CHECK(EXPRESSION)                            \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION),            \
        GUGU_STRINGIZE(EXPRESSION),                             \
        __FILE__, __LINE__)

#define GUGU_UTEST_SILENT_CHECK(EXPRESSION)                     \
    unitTestHandler.SilentRunTestCheck((bool)(EXPRESSION),      \
        GUGU_STRINGIZE(EXPRESSION),                             \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_TRUE(EXPRESSION)                       \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION) == true,    \
        GUGU_STRINGIZE(EXPRESSION) " == true",                  \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_FALSE(EXPRESSION)                      \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION) == false,   \
        GUGU_STRINGIZE(EXPRESSION) " == false",                 \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_NULL(EXPRESSION)                       \
    unitTestHandler.RunTestCheck(EXPRESSION == nullptr,         \
        GUGU_STRINGIZE(EXPRESSION) " == nullptr",               \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_NOT_NULL(EXPRESSION)                   \
    unitTestHandler.RunTestCheck(EXPRESSION != nullptr,         \
        GUGU_STRINGIZE(EXPRESSION) " != nullptr",               \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_EQUAL(LEFT, RIGHT)                     \
    unitTestHandler.RunTestCompare(LEFT, RIGHT, true,           \
        GUGU_STRINGIZE(LEFT) " == " GUGU_STRINGIZE(RIGHT),      \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_NOT_EQUAL(LEFT, RIGHT)                 \
    unitTestHandler.RunTestCompare(LEFT, RIGHT, false,          \
        GUGU_STRINGIZE(LEFT) " != " GUGU_STRINGIZE(RIGHT),      \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_APPROX_EQUAL(LEFT, RIGHT, EPSILON)             \
    unitTestHandler.RunTestCompare(LEFT, RIGHT, EPSILON, true,          \
        GUGU_STRINGIZE(LEFT) " == " GUGU_STRINGIZE(RIGHT),              \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_APPROX_NOT_EQUAL(LEFT, RIGHT, EPSILON)         \
    unitTestHandler.RunTestCompare(LEFT, RIGHT, EPSILON, false,         \
        GUGU_STRINGIZE(LEFT) " != " GUGU_STRINGIZE(RIGHT),              \
        __FILE__, __LINE__)

#define GUGU_UTEST_PERFORMANCE(LOOPS, EXECUTION_METHOD)                             \
    unitTestHandler.RunPerformanceTest(0, LOOPS, EXECUTION_METHOD);

#define GUGU_UTEST_PERFORMANCE_WITH_WARMUP(WARMUP_LOOPS, LOOPS, EXECUTION_METHOD)   \
    unitTestHandler.RunPerformanceTest(WARMUP_LOOPS, LOOPS, EXECUTION_METHOD);

}   // namespace gugu
