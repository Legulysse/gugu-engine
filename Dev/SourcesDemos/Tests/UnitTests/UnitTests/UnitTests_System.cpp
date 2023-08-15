////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_System(UnitTestResults* results)
{
    GUGU_UTEST_INIT("System", "UnitTests_System.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Path Utility");
    {
        GUGU_UTEST_SUBSECTION("NormalizePath");
        {
            GUGU_UTEST_CHECK(NormalizePath("") == "");
            GUGU_UTEST_CHECK(NormalizePath("/") == "");
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world") == "hello/world");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world") == "../hello/world");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../..") == "");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../../../..") == "../..");
            GUGU_UTEST_CHECK(NormalizePath("../../hello/my/../world/../") == "../../hello");
            GUGU_UTEST_CHECK(NormalizePath(".") == "");
            GUGU_UTEST_CHECK(NormalizePath("./") == "");
            GUGU_UTEST_CHECK(NormalizePath("../") == "..");
            GUGU_UTEST_CHECK(NormalizePath("/..") == "..");
            GUGU_UTEST_CHECK(NormalizePath("/../") == "..");
            GUGU_UTEST_CHECK(NormalizePath("/../") == "..");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world/..") == "../hello");
            GUGU_UTEST_CHECK(NormalizePath("///////") == "");
            GUGU_UTEST_CHECK(NormalizePath(".///////") == "");
            GUGU_UTEST_CHECK(NormalizePath(".//.///.//") == "");
            GUGU_UTEST_CHECK(NormalizePath(".folder/hello../..../..world/....") == ".folder/hello../..../..world/....");
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world.xml") == "hello/world.xml");
        }

        GUGU_UTEST_SUBSECTION("EnsureTrailingPathSeparator");
        {
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("") == "/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("/") == "/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("//") == "//");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("///") == "///");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator(".") == "./");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("./") == "./");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("..") == "../");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("hello/world") == "hello/world/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("hello/my.world") == "hello/my.world/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("world") == "world/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("hello/world/") == "hello/world/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("hello/my.world/") == "hello/my.world/");
            GUGU_UTEST_CHECK(EnsureTrailingPathSeparator("world/") == "world/");
        }

        GUGU_UTEST_SUBSECTION("DirectoryPartFromPath");
        {
            GUGU_UTEST_CHECK(DirectoryPartFromPath("") == "");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("hello/world/") == "hello/world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("hello/world") == "hello");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("/world/") == "/world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("/world") == "");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("world/") == "world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("./hello/my/../world/") == "./hello/my/../world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("./hello/my/../world.xml") == "./hello/my/..");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("./hello/my/../world") == "./hello/my/..");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("./hello/my/../my.world/") == "./hello/my/../my.world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("./hello/my/../my.world/file.xml") == "./hello/my/../my.world");
            GUGU_UTEST_CHECK(DirectoryPartFromPath("file.xml") == "");
            GUGU_UTEST_CHECK(DirectoryPartFromPath(NormalizePath("./hello/my/../world.xml")) == "hello");
            GUGU_UTEST_CHECK(DirectoryPartFromPath(NormalizePath("./hello/my/../world")) == "hello");
            GUGU_UTEST_CHECK(DirectoryPartFromPath(NormalizePath("./hello/my/../my.world/file.xml")) == "hello/my.world");
        }

        GUGU_UTEST_SUBSECTION("NamePartFromPath");
        {
            GUGU_UTEST_CHECK(NamePartFromPath("") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("hello/world/") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("hello/world") == "world");
            GUGU_UTEST_CHECK(NamePartFromPath("/world/") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("/world") == "world");
            GUGU_UTEST_CHECK(NamePartFromPath("world/") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("./hello/my/../world/") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("./hello/my/../world.xml") == "world.xml");
            GUGU_UTEST_CHECK(NamePartFromPath("./hello/my/../world") == "world");
            GUGU_UTEST_CHECK(NamePartFromPath("./hello/my/../my.world/") == "");
            GUGU_UTEST_CHECK(NamePartFromPath("./hello/my/../my.world/file.xml") == "file.xml");
            GUGU_UTEST_CHECK(NamePartFromPath("file.xml") == "file.xml");
            GUGU_UTEST_CHECK(NamePartFromPath(NormalizePath("./hello/my/../world.xml")) == "world.xml");
            GUGU_UTEST_CHECK(NamePartFromPath(NormalizePath("./hello/my/../world")) == "world");
            GUGU_UTEST_CHECK(NamePartFromPath(NormalizePath("./hello/my/../my.world/file.xml")) == "file.xml");
        }

        GUGU_UTEST_SUBSECTION("CombinePaths");
        {
            GUGU_UTEST_CHECK(CombinePaths("", "") == "");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "/world/") == "hello/world");
            GUGU_UTEST_CHECK(CombinePaths("/", "/") == "");
            GUGU_UTEST_CHECK(CombinePaths("../hello", "../world") == "../world");
            GUGU_UTEST_CHECK(CombinePaths("../hello/..", "world") == "../world");
            GUGU_UTEST_CHECK(CombinePaths("hello/my", "./world") == "hello/my/world");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "") == "hello");
            GUGU_UTEST_CHECK(CombinePaths("", "/world/") == "world");
            GUGU_UTEST_CHECK(CombinePaths("/../", "/../") == "../..");
            GUGU_UTEST_CHECK(CombinePaths("..", "..") == "../..");
            GUGU_UTEST_CHECK(CombinePaths("..", "file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePaths("..", "./file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePaths("../hello/..", "./file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "/world/file.txt") == "hello/world/file.txt");
            GUGU_UTEST_CHECK(CombinePaths("", "file.txt") == "file.txt");
            GUGU_UTEST_CHECK(CombinePaths("hello", "") == "hello");
        }

        GUGU_UTEST_SUBSECTION("PathStartsWith");
        {
            GUGU_UTEST_CHECK(!PathStartsWith("", ""));
            GUGU_UTEST_CHECK(!PathStartsWith("Assets", ""));
            GUGU_UTEST_CHECK(!PathStartsWith("", "Assets"));
            GUGU_UTEST_CHECK(!PathStartsWith("/", "/"));
            GUGU_UTEST_CHECK(!PathStartsWith("/Assets", "/"));
            GUGU_UTEST_CHECK(!PathStartsWith("/", "/Assets"));
            GUGU_UTEST_CHECK(PathStartsWith("./", "."));
            GUGU_UTEST_CHECK(PathStartsWith(".", "./"));
            GUGU_UTEST_CHECK(!PathStartsWith("", "."));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory", "Assets"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory", "Assets/"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory/", "Assets"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory/", "Assets/"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory", "Assets/Directory"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory", "Assets/Directory/"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory/", "Assets/Directory"));
            GUGU_UTEST_CHECK(PathStartsWith("Assets/Directory/", "Assets/Directory/"));
            GUGU_UTEST_CHECK(!PathStartsWith("Assets/Directory", "Assets/DirectoryBis"));
            GUGU_UTEST_CHECK(!PathStartsWith("Assets/DirectoryBis", "Assets/Directory"));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("FileInfo");
    {
        GUGU_UTEST_SUBSECTION("Construction");
        {
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("hello/world.txt") == FileInfo::FromString_utf8(CombinePaths("hello", "world.txt")));
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("hello/world.txt") == FileInfo::FromString_utf8(CombinePaths("", "hello/world.txt")));
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("world.txt") == FileInfo::FromString_utf8(CombinePaths("", "world.txt")));

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8("hello/world.txt");

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "hello/world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "hello/world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "hello");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8(CombinePaths("hello", "world.txt"));

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "hello/world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "hello/world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "hello");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8(CombinePaths("", "world.txt"));

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8(CombinePaths("", ".txt"));

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == ".txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == ".txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == ".txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8(CombinePaths("", "world.file.txt"));

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "world.file.txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "world.file.txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world.file.txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "file.txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
                GUGU_UTEST_CHECK(fileInfo.HasExtension("file.txt"));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8(CombinePaths("", "world"));

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "");
                GUGU_UTEST_CHECK(!fileInfo.HasExtension("world"));
                GUGU_UTEST_CHECK(!fileInfo.HasExtension(""));
            }

            {
                const FileInfo fileInfo = FileInfo::FromString_utf8("world.txt");

                GUGU_UTEST_CHECK(fileInfo.GetFileSystemPath() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetFilePath_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetDirectoryPath_utf8() == "");
                GUGU_UTEST_CHECK(fileInfo.GetFileName_utf8() == "world.txt");
                GUGU_UTEST_CHECK(fileInfo.GetPrettyName() == "world");
                GUGU_UTEST_CHECK(fileInfo.GetExtension() == "txt");
                GUGU_UTEST_CHECK(fileInfo.GetAllExtensions() == "txt");
                GUGU_UTEST_CHECK(fileInfo.HasExtension("txt"));
            }
        }
        
        GUGU_UTEST_SUBSECTION("Comparison");
        {
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("aaa/111") == FileInfo::FromString_utf8("aaa/111"));
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("aaa/111") != FileInfo::FromString_utf8("aaa/222"));
            GUGU_UTEST_CHECK(FileInfo::FromString_utf8("bbb/111") != FileInfo::FromString_utf8("aaa/111"));

            std::vector<FileInfo> fileInfoArray
            {
                FileInfo::FromString_utf8("aaa/111"),
                FileInfo::FromString_utf8("bbb/222"),
                FileInfo::FromString_utf8("bbb/111"),
                FileInfo::FromString_utf8("aaa/333"),
            };

            std::sort(fileInfoArray.begin(), fileInfoArray.end());

            GUGU_UTEST_CHECK(fileInfoArray[0] == FileInfo::FromString_utf8("aaa/111"));
            GUGU_UTEST_CHECK(fileInfoArray[1] == FileInfo::FromString_utf8("aaa/333"));
            GUGU_UTEST_CHECK(fileInfoArray[2] == FileInfo::FromString_utf8("bbb/111"));
            GUGU_UTEST_CHECK(fileInfoArray[3] == FileInfo::FromString_utf8("bbb/222"));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("String");
    {
        GUGU_UTEST_SUBSECTION("ToString");
        {
            GUGU_UTEST_CHECK_EQUAL(ToString(42), "42");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5f), "42.5");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5), "42.5");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5f, 2), "42.50");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5, 2), "42.50");
            GUGU_UTEST_CHECK_EQUAL(ToString(true), "1");
            GUGU_UTEST_CHECK_EQUAL(ToString(false), "0");
            GUGU_UTEST_CHECK_EQUAL(ToString("42"), "42");
            GUGU_UTEST_CHECK_EQUAL(ToString(std::string("42")), "42");
        }

        GUGU_UTEST_SUBSECTION("FromString");
        {
            GUGU_UTEST_CHECK_EQUAL(FromString<int>("42"), 42);
            GUGU_UTEST_CHECK_EQUAL(FromString<float>("42.50"), 42.5f);
            GUGU_UTEST_CHECK_EQUAL(FromString<double>("42.50"), 42.5);
            GUGU_UTEST_CHECK_EQUAL(FromString<bool>("1"), true);
            GUGU_UTEST_CHECK_EQUAL(FromString<bool>("0"), false);
            GUGU_UTEST_CHECK_NOT_EQUAL(FromString<bool>("true"), true); // Converting from text-based true/false will always return false.
            GUGU_UTEST_CHECK_EQUAL(FromString<bool>("false"), false);   // Converting from text-based true/false will always return false.
            GUGU_UTEST_CHECK_EQUAL(FromString<std::string>("42"), "42");
        }

        GUGU_UTEST_SUBSECTION("Equals");
        {
            const char* testCharA = "";
            const char* testCharB = "";
            GUGU_UTEST_CHECK(StringEquals(testCharA, ""));
            GUGU_UTEST_CHECK(StringEquals(testCharB, ""));
            GUGU_UTEST_CHECK(StringEquals(testCharA, testCharB));
            GUGU_UTEST_CHECK(StringEquals(testCharA, std::string(testCharB)));
            GUGU_UTEST_CHECK(StringEquals(testCharA, std::string_view(testCharB)));
            GUGU_UTEST_CHECK(StringEquals(std::string_view(testCharA), std::string(testCharB)));
            GUGU_UTEST_CHECK(StringEquals(std::string(testCharA), std::string(testCharB)));
            GUGU_UTEST_CHECK(StringEquals(std::string_view(testCharA), std::string_view(testCharB)));

            GUGU_UTEST_CHECK(StringEquals("hello", "hello"));
            GUGU_UTEST_CHECK(StringEquals(std::string("hello"), "hello"));
            GUGU_UTEST_CHECK(StringEquals(std::string_view("hello"), "hello"));
            GUGU_UTEST_CHECK(!StringEquals("hello", "world"));
            GUGU_UTEST_CHECK(!StringEquals(std::string("hello"), "world"));
            GUGU_UTEST_CHECK(!StringEquals(std::string_view("hello"), "world"));
            GUGU_UTEST_CHECK(!StringEquals("hello", "hello world"));
            GUGU_UTEST_CHECK(!StringEquals(std::string("hello"), "hello world"));
            GUGU_UTEST_CHECK(!StringEquals(std::string_view("hello"), "hello world"));
        }

        GUGU_UTEST_SUBSECTION("ToUpper/ToLower");
        {
            GUGU_UTEST_CHECK_EQUAL(StdStringToUpper("hello world 42"), "HELLO WORLD 42");
            GUGU_UTEST_CHECK_EQUAL(StdStringToLower("HELLO WORLD 42"), "hello world 42");
        }

        GUGU_UTEST_SUBSECTION("StartsWith/EndsWith");
        {
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWith("hello world 42", "hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello world 42", "42"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWith("hello world 42", "42"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello world 42", "hello"));
        }

        GUGU_UTEST_SUBSECTION("Format");
        {
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My name is {0}. Yes, {0}.", "plop"), "My name is plop. Yes, plop.");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My age is {0}.", 42), "My age is 42.");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("{3}, {2}, {1}, {0} !", "go", 1, 2, 3), "3, 2, 1, go !");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("{1}{0}{2}", "a", "b", "c"), "bac");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("{1}{1}{0}", 7.f, 0.f), "007");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10}", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10), "0 1 2 3 4 5 6 7 8 9 10");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("hello {0}, do you have {1} gold ?", "Jean-Jacques", 98521), "hello Jean-Jacques, do you have 98521 gold ?");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("hello {0}, do you have {1} gold ?", "Jean-Jacques"), "hello Jean-Jacques, do you have {1} gold ?");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My age is {0}.", 42.5f), "My age is 42.5.");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My age is {0}.", ToString(42.555f)), "My age is 42.555.");
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My age is {0}.", ToString(42.555f, 1)), "My age is 42.6.");

            FormatParameters params;
            params.Add("name", "Jean-Paul");
            params.Add("gold", 77);
            GUGU_UTEST_CHECK_EQUAL(StringFormat("hello {name}, do you have {gold} gold ?", params), "hello Jean-Paul, do you have 77 gold ?");
        }

        GUGU_UTEST_SUBSECTION("NumberFormat");
        {
            GUGU_UTEST_CHECK(StringNumberFormat(1) == "1");
            GUGU_UTEST_CHECK(StringNumberFormat(123, 3) == "123");
            GUGU_UTEST_CHECK(StringNumberFormat(1, 3) == "001");
            GUGU_UTEST_CHECK(StringNumberFormat(1, 6) == "000 001");

            GUGU_UTEST_CHECK(StringNumberFormat(-1) == "-1");
            GUGU_UTEST_CHECK(StringNumberFormat(-123, 3) == "-123");
            GUGU_UTEST_CHECK(StringNumberFormat(-1, 3) == "-001");
            GUGU_UTEST_CHECK(StringNumberFormat(-1, 6) == "-000 001");

            GUGU_UTEST_CHECK(StringNumberFormat(1.05f) == "1.05");
            GUGU_UTEST_CHECK(StringNumberFormat(123.05f, 3) == "123.05");
            GUGU_UTEST_CHECK(StringNumberFormat(1.05f, 3) == "001.05");
            GUGU_UTEST_CHECK(StringNumberFormat(1.05f, 6) == "000 001.05");

            GUGU_UTEST_CHECK(StringNumberFormat("1,05", 0, ".") == "1,05");
            GUGU_UTEST_CHECK(StringNumberFormat("123,05", 3, ".") == "123,05");
            GUGU_UTEST_CHECK(StringNumberFormat("1,05", 3, ".") == "001,05");
            GUGU_UTEST_CHECK(StringNumberFormat("1,05", 6, ".") == "000.001,05");
        }

        GUGU_UTEST_SUBSECTION("Split");
        {
            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", " ", tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 2))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hello");
                    GUGU_UTEST_CHECK(tokens[1] == "world");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("  hello   world  ", " ", tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 2))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hello");
                    GUGU_UTEST_CHECK(tokens[1] == "world");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", "ll", tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 2))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "he");
                    GUGU_UTEST_CHECK(tokens[1] == "o world");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", "o", tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 3))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hell");
                    GUGU_UTEST_CHECK(tokens[1] == " w");
                    GUGU_UTEST_CHECK(tokens[2] == "rld");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", ' ', tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 2))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hello");
                    GUGU_UTEST_CHECK(tokens[1] == "world");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("  hello   world  ", ' ', tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 2))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hello");
                    GUGU_UTEST_CHECK(tokens[1] == "world");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", 'l', tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 3))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "he");
                    GUGU_UTEST_CHECK(tokens[1] == "o wor");
                    GUGU_UTEST_CHECK(tokens[2] == "d");
                }
            }

            {
                std::vector<std::string> tokens;
                StdStringSplit("hello world", 'o', tokens);

                if (GUGU_UTEST_CHECK(tokens.size() == 3))
                {
                    GUGU_UTEST_CHECK(tokens[0] == "hell");
                    GUGU_UTEST_CHECK(tokens[1] == " w");
                    GUGU_UTEST_CHECK(tokens[2] == "rld");
                }
            }
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("File System");
    {
        GUGU_UTEST_SUBSECTION("Directories");
        {
            GUGU_UTEST_CHECK(DirectoryExists("Assets/TestDirectory"));
            GUGU_UTEST_CHECK(FileExists("Assets/TestDirectory/TestEmptyFile.txt"));

            GUGU_UTEST_CHECK(!DirectoryExists("Assets/TestDirectory_NONE"));
            GUGU_UTEST_CHECK(!FileExists("Assets/TestDirectory/TestEmptyFile_NONE.txt"));
            GUGU_UTEST_CHECK(!FileExists("Assets/TestDirectory"));
            GUGU_UTEST_CHECK(!DirectoryExists("Assets/TestDirectory/TestEmptyFile.txt"));

            GUGU_UTEST_CHECK(!DirectoryExists("User"));
            GUGU_UTEST_CHECK(!DirectoryExists("User/TestDirectory"));

            GUGU_UTEST_CHECK(!RemoveTargetDirectory("User"));
            GUGU_UTEST_CHECK(!RemoveDirectoryTree("User"));

            GUGU_UTEST_CHECK(EnsureDirectoryExists("User/TestDirectory"));
            GUGU_UTEST_CHECK(EnsureDirectoryExists("User/TestDirectory"));
            GUGU_UTEST_CHECK(EnsureDirectoryExists("User"));

            GUGU_UTEST_CHECK(DirectoryExists("User"));
            GUGU_UTEST_CHECK(DirectoryExists("User/TestDirectory"));

            GUGU_UTEST_CHECK(!RemoveTargetDirectory("User"));
            GUGU_UTEST_CHECK(RemoveTargetDirectory("User/TestDirectory"));
            GUGU_UTEST_CHECK(RemoveTargetDirectory("User"));

            GUGU_UTEST_CHECK(!DirectoryExists("User"));
            GUGU_UTEST_CHECK(!DirectoryExists("User/TestDirectory"));

            GUGU_UTEST_CHECK(EnsureDirectoryExists("User/TestDirectory"));
            GUGU_UTEST_CHECK(RemoveDirectoryTree("User"));

            GUGU_UTEST_CHECK(!DirectoryExists("User"));
            GUGU_UTEST_CHECK(!DirectoryExists("User/TestDirectory"));
        }

        GUGU_UTEST_SUBSECTION("Parsing");
        {
            std::vector<std::string> directories;
            GetDirectories("", directories, true);

            GUGU_UTEST_CHECK(StdVectorContains<std::string>(directories, "Assets/TestDirectory"));

            directories.clear();
            GetDirectories("Assets", directories, true);

            GUGU_UTEST_CHECK(StdVectorContains<std::string>(directories, "Assets/TestDirectory"));

            std::vector<FileInfo> filePaths;
            GetFiles("", filePaths, true);

            GUGU_UTEST_CHECK(StdVectorContains(filePaths, FileInfo::FromString_utf8("Assets/TestDirectory/TestEmptyFile.txt")));

            filePaths.clear();
            GetFiles("Assets", filePaths, true);

            GUGU_UTEST_CHECK(StdVectorContains(filePaths, FileInfo::FromString_utf8("Assets/TestDirectory/TestEmptyFile.txt")));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Containers");
    {
        GUGU_UTEST_SUBSECTION("Vector");
        {
            std::vector<int> container = { 10, 20, 30, 40, 50 };

            GUGU_UTEST_CHECK(StdVectorIndexOf(container, 10) == 0);
            GUGU_UTEST_CHECK(StdVectorIndexOf(container, 30) == 2);
            GUGU_UTEST_CHECK(StdVectorIndexOf(container, 50) == 4);
            GUGU_UTEST_CHECK(StdVectorIndexOf(container, 0) == system::InvalidIndex);

            std::string valueA = "AAA";
            std::string valueB = "BBB";
            std::string valueC = "CCC";
            std::vector<std::string> containerB { valueC };
            GUGU_UTEST_CHECK_EQUAL(containerB[0], "CCC");

            StdVectorPushFront(containerB, valueA);
            GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
            GUGU_UTEST_CHECK_EQUAL(containerB[1], "CCC");

            StdVectorInsertAt(containerB, 1, valueB);
            GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
            GUGU_UTEST_CHECK_EQUAL(containerB[1], "BBB");
            GUGU_UTEST_CHECK_EQUAL(containerB[2], "CCC");

            GUGU_UTEST_CHECK(StdVectorContains(containerB, valueC));
            GUGU_UTEST_CHECK(StdVectorFind(containerB, valueC) != containerB.end());

            StdVectorRemove(containerB, valueC);
            GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
            GUGU_UTEST_CHECK_EQUAL(containerB[1], "BBB");

            StdVectorRemoveAt(containerB, 0);
            GUGU_UTEST_CHECK_EQUAL(containerB[0], "BBB");

            StdVectorRemoveIf(containerB, [&](const std::string& item) { return item == valueB; });
            GUGU_UTEST_CHECK(containerB.empty());

            GUGU_UTEST_CHECK_FALSE(StdVectorContains(containerB, valueC));
            GUGU_UTEST_CHECK(StdVectorFind(containerB, valueC) == containerB.end());
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
