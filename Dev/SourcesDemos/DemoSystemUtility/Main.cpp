////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "Gugu/System/SystemUtility.h"

#include <SFML/System/Clock.hpp>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //--------

    int64 iTimestamp = GetTimestamp();

    std::string strUpperCase = StdStringToUpper("hello world 42");
    std::string strLowerCase = StdStringToLower("HELLO WORLD 42");

    std::string strFormatA = StringFormat("My name is {0}. Yes, {0}.", "plop");
    std::string strFormatB = StringFormat("My age is {0}.", 42);
    std::string strFormatC = StringFormat("{3}, {2}, {1}, {0} !", "go", 1, 2, 3);
    std::string strFormatD = StringFormat("{1}{0}{2}", "a", "b", "c");
    std::string strFormatE = StringFormat("{1}{1}{0}", 7.f, 0.f);
    std::string strFormatF = StringFormat("{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10}", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    std::string strFormatG = StringFormat("hello {0}, do you have {1} gold ?", "Jean-Jacques", 98521);
    std::string strFormatH = StringFormat("hello {0}, do you have {1} gold ?", "Jean-Jacques");
    std::string strFormatI = StringFormat("My age is {0}.", 42.5f);
    std::string strFormatJ = StringFormat("My age is {0}.", ToString(42.555f));
    std::string strFormatK = StringFormat("My age is {0}.", ToString(42.555f, 1));

    std::string strNumberFormatA = StringNumberFormat(12345678);
    std::string strNumberFormatB = StringNumberFormat(12345678, ",");

    FormatParameters paramsAA;
    paramsAA.Add("name", "Jean-Paul");
    paramsAA.Add("gold", 77);
    std::string strFormatAA = StringFormat("hello {name}, do you have {gold} gold ?", paramsAA);

    //--------

    size_t nbLoops = 100000;

    {
        sf::Clock kClock;
        std::string strFormat = "";

        for (size_t i = 0; i < nbLoops; ++i)
        {
            FormatParameters params;
            params.Add("gold0", 987521);
            params.Add("gold1", 987521);
            params.Add("gold2", 987521);
            params.Add("gold3", 987521);
            params.Add("gold4", 987521);
            params.Add("gold5", 987521);
            strFormat = StringFormat("hello {gold0}, do you have {gold1} {gold2} {gold3} {gold4} {gold5} gold ?", params);
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsole(StringFormat("{0} : {1} ms", strFormat, ms));
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        for (size_t i = 0; i < nbLoops; ++i)
        {
            strFormat = StringFormat("hello {0}, do you have {1} gold ? {2} {3} {4} {5}", 9875.21f, 9875.21f, 9875.21f, 9875.21f, 9875.21f, 9875.21f);
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsole(StringFormat("{0} : {1} ms", strFormat, ms));
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        for (size_t i = 0; i < nbLoops; ++i)
        {
            strFormat = StringFormat("hello {0}, do you have {1} gold ? {2} {3} {4} {5}", 987521, 987521, 987521, 987521, 987521, 987521);
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsole(StringFormat("{0} : {1} ms", strFormat, ms));
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        for (size_t i = 0; i < nbLoops; ++i)
        {
            strFormat = StringFormat("hello {0}, do you have {1} gold ? {2} {3} {4} {5}", 7, 7, 7, 7, 7, 7);
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsole(StringFormat("{0} : {1} ms", strFormat, ms));
    }

    {
        sf::Clock kClock;
        std::string strFormat = "";

        for (size_t i = 0; i < nbLoops; ++i)
        {
            strFormat = StringFormat("hello {0}, do you have {1} gold ? {2} {3} {4} {5}", "Jean-Jacques", "Jean-Jacques", "Jean-Jacques", "Jean-Jacques", "Jean-Jacques", "Jean-Jacques");
        }

        sf::Time result = kClock.getElapsedTime();
        sf::Int32 ms = result.asMilliseconds();

        WriteInConsole(StringFormat("{0} : {1} ms", strFormat, ms));
    }

    //--------

    return 0;
}
