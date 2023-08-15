#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
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

private:

    Logger m_logger;

    size_t m_testCount = 0;
    size_t m_successCount = 0;
};

class UnitTestHandler
{
public:

    UnitTestHandler(const std::string& header, const std::string& logPathFile, UnitTestResults* testResults);
    void Finalize();
    void PrintResults();

    void BeginSection(const std::string& title);
    void BeginSubSection(const std::string& title);

    bool RunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line);
    bool SilentRunTestCheck(bool result, const std::string& expression, const std::string& file, size_t line);
    bool RunTestCompare(bool result, const std::string& left, const std::string& right, const std::string& expression, const std::string& file, size_t line);

private:

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

#define GUGU_UTEST_CHECK(EXPRESSION)                            \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION),            \
        GUGU_STRINGIZE(EXPRESSION),                             \
        __FILE__, __LINE__)

#define GUGU_UTEST_SILENT_CHECK(EXPRESSION)                     \
    unitTestHandler.SilentRunTestCheck((bool)(EXPRESSION),      \
        GUGU_STRINGIZE(EXPRESSION), __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_TRUE(EXPRESSION)                       \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION) == true,    \
        GUGU_STRINGIZE(EXPRESSION) " == true",                  \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_FALSE(EXPRESSION)                      \
    unitTestHandler.RunTestCheck((bool)(EXPRESSION) == false,   \
        GUGU_STRINGIZE(EXPRESSION) " == false",                 \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_EQUAL(LEFT, RIGHT)                     \
    unitTestHandler.RunTestCompare(LEFT == RIGHT,               \
        ToString(LEFT), ToString(RIGHT),                        \
        GUGU_STRINGIZE(LEFT) " == " GUGU_STRINGIZE(RIGHT),      \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_NOT_EQUAL(LEFT, RIGHT)                 \
    unitTestHandler.RunTestCompare(LEFT != RIGHT,               \
        ToString(LEFT), ToString(RIGHT),                        \
        GUGU_STRINGIZE(LEFT) " != " GUGU_STRINGIZE(RIGHT),      \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_APPROX_EQUAL(LEFT, RIGHT, EPSILON)             \
    unitTestHandler.RunTestCompare(ApproxEqual(LEFT, RIGHT, EPSILON),   \
        ToString(LEFT), ToString(RIGHT),                                \
        GUGU_STRINGIZE(LEFT) " == " GUGU_STRINGIZE(RIGHT),              \
        __FILE__, __LINE__)

#define GUGU_UTEST_CHECK_NOT_APPROX_EQUAL(LEFT, RIGHT, EPSILON)         \
    unitTestHandler.RunTestCompare(!ApproxEqual(LEFT, RIGHT, EPSILON),  \
        ToString(LEFT), ToString(RIGHT),                                \
        GUGU_STRINGIZE(LEFT) " != " GUGU_STRINGIZE(RIGHT),              \
        __FILE__, __LINE__)

}   // namespace gugu
