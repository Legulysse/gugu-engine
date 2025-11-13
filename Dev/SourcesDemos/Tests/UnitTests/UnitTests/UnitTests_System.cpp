////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Container.h"
#include "Gugu/System/EnumFlag.h"
#include "Gugu/System/Path.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/UUID.h"
#include "Gugu/System/Hash.h"
#include "Gugu/System/Memory.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_System(UnitTestResults* results)
{
    GUGU_UTEST_INIT("System", "UnitTests_System.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Memory");
    {
        {
            int* ptr = nullptr;
            GUGU_UTEST_CHECK_NULL(ptr);

            SafeDelete(ptr);
            GUGU_UTEST_CHECK_NULL(ptr);

            ptr = new int;
            GUGU_UTEST_CHECK_NOT_NULL(ptr);

            SafeDelete(ptr);
            GUGU_UTEST_CHECK_NULL(ptr);
        }

        {
            int* ptr = nullptr;
            GUGU_UTEST_CHECK_NULL(ptr);

            SafeDeleteArray(ptr);
            GUGU_UTEST_CHECK_NULL(ptr);

            ptr = new int[10];
            GUGU_UTEST_CHECK_NOT_NULL(ptr);

            SafeDeleteArray(ptr);
            GUGU_UTEST_CHECK_NULL(ptr);
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Path");
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
                GUGU_UTEST_CHECK(fileInfo.HasExtension("FILE.TXT"));
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
                GUGU_UTEST_CHECK(fileInfo.HasExtension("TXT"));
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
            enum class ETestEnum { Value1 = 1, Value10 = 10, };
            enum class ETestEnumFlag { ValueA = 1 << 0, ValueB = 1 << 1, ValueC = 1 << 2, };

            GUGU_UTEST_CHECK_EQUAL(ToString(42), "42");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5f), "42.5");
            GUGU_UTEST_CHECK_EQUAL(ToString(42.5), "42.5");
            GUGU_UTEST_CHECK_EQUAL(ToStringf(42.5f, 2), "42.50");
            GUGU_UTEST_CHECK_EQUAL(ToStringf(42.5, 2), "42.50");
            GUGU_UTEST_CHECK_EQUAL(ToString(true), "1");
            GUGU_UTEST_CHECK_EQUAL(ToString(false), "0");
            GUGU_UTEST_CHECK_EQUAL(ToString("42"), "42");
            GUGU_UTEST_CHECK_EQUAL(ToString(std::string("42")), "42");
            GUGU_UTEST_CHECK_EQUAL(ToString(ETestEnum::Value1), "1");
            GUGU_UTEST_CHECK_EQUAL(ToString(ETestEnum::Value10), "10");
            GUGU_UTEST_CHECK_EQUAL(ToString(ETestEnumFlag::ValueA), "1");
            GUGU_UTEST_CHECK_EQUAL(ToString(ETestEnumFlag::ValueC), "4");
            GUGU_UTEST_CHECK_EQUAL(ToString((int)ETestEnumFlag::ValueA | (int)ETestEnumFlag::ValueC), "5");

            int* fakePtr = (int*)0x00000ff123456789;
            GUGU_UTEST_CHECK_EQUAL(ToString(fakePtr), "00000FF123456789");
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
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("a", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWith("a", "a"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("A", "a"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("a", "A"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWith("hello world", "hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello world", "HeLlO"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello world", " hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello world", "world"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello world", "WoRlD"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("hello", "hello world"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWith("world", "hello world"));

            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("a", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWith("a", "a"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("A", "a"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("a", "A"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWith("hello world", "world"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello world", "WoRlD"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello world", "world "));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello world", "hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello world", "HeLlO"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("hello", "hello world"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWith("world", "hello world"));

            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("a", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWithInsensitive("a", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWithInsensitive("A", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWithInsensitive("a", "A"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWithInsensitive("hello world", "hello"));
            GUGU_UTEST_CHECK_TRUE(StdStringStartsWithInsensitive("hello world", "HeLlO"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("hello world", " hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("hello world", "world"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("hello world", "WoRlD"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("hello", "hello world"));
            GUGU_UTEST_CHECK_FALSE(StdStringStartsWithInsensitive("world", "hello world"));

            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("a", ""));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWithInsensitive("a", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWithInsensitive("A", "a"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWithInsensitive("a", "A"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWithInsensitive("hello world", "world"));
            GUGU_UTEST_CHECK_TRUE(StdStringEndsWithInsensitive("hello world", "WoRlD"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("hello world", "world "));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("hello world", "hello"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("hello world", "HeLlO"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("hello", "hello world"));
            GUGU_UTEST_CHECK_FALSE(StdStringEndsWithInsensitive("world", "hello world"));
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
            GUGU_UTEST_CHECK_EQUAL(StringFormat("My age is {0}.", ToStringf(42.555f, 1)), "My age is 42.6.");

            FormatParameters params;
            params.Add("name", "Jean-Paul");
            params.Add("gold", 77);
            GUGU_UTEST_CHECK_EQUAL(StringFormat("hello {name}, do you have {gold} gold ?", params), "hello Jean-Paul, do you have 77 gold ?");
        }

        GUGU_UTEST_SUBSECTION("NumberFormat");
        {
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1, 0), "1");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(123, 3), "123");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1, 3), "001");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1, 6), "000 001");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(-1, 0), "-1");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(-123, 3), "-123");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(-1, 3), "-001");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(-1, 6), "-000 001");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1.05f, 0), "1.05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(123.05f, 3), "123.05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1.05f, 3), "001.05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat(1.05f, 6), "000 001.05");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat("1,05", 0, "."), "1,05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat("123,05", 3, "."), "123,05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat("1,05", 3, "."), "001,05");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormat("1,05", 6, "."), "000.001,05");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 0, 2), "1.06");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(123.0572f, 3, 2), "123.06");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 3, 2), "001.06");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 6, 2), "000 001.06");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 0, 3), "1.057");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(123.0572f, 3, 3), "123.057");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 3, 3), "001.057");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 6, 3), "000 001.057");

            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 0, 4), "1.0572");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(123.0572f, 3, 4), "123.0572");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 3, 4), "001.0572");
            GUGU_UTEST_CHECK_EQUAL(StringNumberFormatf(1.0572f, 6, 4), "000 001.0572");
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
            // Manipulations on single values.
            {
                std::vector<int> containerA = { 10, 20, 30, 40, 50 };

                GUGU_UTEST_CHECK(StdVectorIndexOf(containerA, 10) == 0);
                GUGU_UTEST_CHECK(StdVectorIndexOf(containerA, 30) == 2);
                GUGU_UTEST_CHECK(StdVectorIndexOf(containerA, 50) == 4);
                GUGU_UTEST_CHECK(StdVectorIndexOf(containerA, 0) == system::InvalidIndex);

                std::string valueA = "AAA";
                std::string valueB = "BBB";
                std::string valueC = "CCC";
                std::string valueD = "DDD";
                std::vector<std::string> containerB{ valueC };
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "CCC");

                StdVectorPushFront(containerB, valueA);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
                GUGU_UTEST_CHECK_EQUAL(containerB[1], "CCC");

                StdVectorInsertAt(containerB, 1, valueB);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
                GUGU_UTEST_CHECK_EQUAL(containerB[1], "BBB");
                GUGU_UTEST_CHECK_EQUAL(containerB[2], "CCC");

                GUGU_UTEST_CHECK_TRUE(StdVectorContains(containerB, valueC));
                GUGU_UTEST_CHECK_FALSE(StdVectorContains(containerB, valueD));
                GUGU_UTEST_CHECK(StdVectorFind(containerB, valueC) != containerB.end());
                GUGU_UTEST_CHECK(StdVectorFind(containerB, valueD) == containerB.end());

                containerB.push_back(valueD);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 4);
                StdVectorRemove(containerB, valueC);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 3);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "AAA");
                GUGU_UTEST_CHECK_EQUAL(containerB[1], "BBB");
                GUGU_UTEST_CHECK_EQUAL(containerB[2], "DDD");

                StdVectorRemoveAt(containerB, 0);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 2);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "BBB");
                GUGU_UTEST_CHECK_EQUAL(containerB[1], "DDD");

                containerB.push_back(valueA);
                containerB.push_back(valueC);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 4);
                StdVectorRemoveAt(containerB, 2, 2);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 2);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "BBB");
                GUGU_UTEST_CHECK_EQUAL(containerB[1], "DDD");

                StdVectorRemoveIf(containerB, [&](const std::string& item) { return item == valueB; });
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 1);
                GUGU_UTEST_CHECK_EQUAL(containerB[0], "DDD");

                GUGU_UTEST_CHECK_FALSE(StdVectorContains(containerB, valueC));
                GUGU_UTEST_CHECK_TRUE(StdVectorContains(containerB, valueD));
                GUGU_UTEST_CHECK(StdVectorFind(containerB, valueC) == containerB.end());
                GUGU_UTEST_CHECK(StdVectorFind(containerB, valueD) != containerB.end());

                containerB.push_back(valueD);
                containerB.push_back(valueD);
                containerB.push_back(valueD);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 4);
                StdVectorRemoveFirst(containerB, valueD);
                GUGU_UTEST_CHECK_EQUAL(containerB.size(), 3);
            }

            // Manipulations on pointers.
            {
                std::vector<int*> containerC;
                containerC.push_back(new int(1));
                containerC.push_back(new int(2));
                containerC.push_back(new int(3));
                containerC.push_back(new int(4));
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 4);
                ClearStdVector(containerC);
                GUGU_UTEST_CHECK_TRUE(containerC.empty());

                containerC.push_back(new int(1));
                containerC.push_back(new int(2));
                containerC.push_back(new int(3));
                containerC.push_back(new int(4));

                StdVectorDeleteIf(containerC, [](int* item) { return item != nullptr && *item == 3; });
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 4);
                GUGU_UTEST_CHECK_EQUAL(*containerC[0], 1);
                GUGU_UTEST_CHECK_EQUAL(*containerC[1], 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[2], nullptr);
                GUGU_UTEST_CHECK_EQUAL(*containerC[3], 4);

                StdVectorDeleteAndRemoveIf(containerC, [](int* item) { return item != nullptr && *item == 1; });
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 3);
                GUGU_UTEST_CHECK_EQUAL(*containerC[0], 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], nullptr);
                GUGU_UTEST_CHECK_EQUAL(*containerC[2], 4);

                StdVectorDeleteAndRemoveAt(containerC, 2);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 2);
                GUGU_UTEST_CHECK_EQUAL(*containerC[0], 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], nullptr);

                StdVectorRemoveIfNull(containerC);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 1);
                GUGU_UTEST_CHECK_EQUAL(*containerC[0], 2);

                // Finalize.
                ClearStdVector(containerC);
            }

            // Manipulations on multiple containers.
            {
                std::vector<int> containerA = { 1, 2, 3, 4, 5 };
                std::vector<int> containerB = { 2, 4, 6, 8, 10 };
                std::vector<int> containerC;
                StdVectorDifference(containerA, containerB, containerC);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 3);
                GUGU_UTEST_CHECK_EQUAL(containerC[0], 1);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], 3);
                GUGU_UTEST_CHECK_EQUAL(containerC[2], 5);

                containerC.clear();
                StdVectorDifference(containerB, containerA, containerC);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 3);
                GUGU_UTEST_CHECK_EQUAL(containerC[0], 6);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], 8);
                GUGU_UTEST_CHECK_EQUAL(containerC[2], 10);

                containerC.clear();
                StdVectorIntersection(containerA, containerB, containerC);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[0], 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], 4);

                containerC.clear();
                StdVectorAppend({ 1, 2, 3 }, containerC);
                StdVectorAppend({ 4, 5, 6 }, containerC);
                GUGU_UTEST_CHECK_EQUAL(containerC.size(), 6);
                GUGU_UTEST_CHECK_EQUAL(containerC[0], 1);
                GUGU_UTEST_CHECK_EQUAL(containerC[1], 2);
                GUGU_UTEST_CHECK_EQUAL(containerC[2], 3);
                GUGU_UTEST_CHECK_EQUAL(containerC[3], 4);
                GUGU_UTEST_CHECK_EQUAL(containerC[4], 5);
                GUGU_UTEST_CHECK_EQUAL(containerC[5], 6);
            }
        }

        GUGU_UTEST_SUBSECTION("Set");
        {
            std::set<int> containerA;
            containerA.insert(10);
            containerA.insert(20);
            containerA.insert(30);

            GUGU_UTEST_CHECK_TRUE(StdSetContains(containerA, 10));
            GUGU_UTEST_CHECK_TRUE(StdSetContains(containerA, 20));
            GUGU_UTEST_CHECK_TRUE(StdSetContains(containerA, 30));
            GUGU_UTEST_CHECK_FALSE(StdSetContains(containerA, 40));
        }

        GUGU_UTEST_SUBSECTION("List");
        {
            std::list<int*> containerC;
            containerC.push_back(new int(1));
            containerC.push_back(new int(2));
            containerC.push_back(new int(3));
            containerC.push_back(new int(4));
            GUGU_UTEST_CHECK_EQUAL(containerC.size(), 4);
            ClearStdList(containerC);
            GUGU_UTEST_CHECK_TRUE(containerC.empty());
        }

        GUGU_UTEST_SUBSECTION("Map");
        {
            std::map<std::string, int> containerA;
            containerA["A"] = 10;
            containerA["B"] = 20;
            containerA["C"] = 30;

            {
                int result = 0;
                GUGU_UTEST_CHECK_EQUAL(result, 0);

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValue(containerA, std::string("A"), result)))
                {
                    GUGU_UTEST_CHECK_EQUAL(result, 10);
                }

                if (GUGU_UTEST_CHECK_FALSE(StdMapTryGetValue(containerA, std::string("AA"), result)))
                {
                    GUGU_UTEST_CHECK_EQUAL(result, 10); // The result value will be unchanged through the call to TryGet.
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValue(containerA, std::string("B"), result)))
                {
                    GUGU_UTEST_CHECK_EQUAL(result, 20);
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValue(containerA, std::string("C"), result)))
                {
                    GUGU_UTEST_CHECK_EQUAL(result, 30);
                }
            }

            {
                const int* resultRef = nullptr;
                GUGU_UTEST_CHECK_EQUAL(resultRef, nullptr);

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueConstRef(containerA, std::string("A"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 10);
                }

                if (GUGU_UTEST_CHECK_FALSE(StdMapTryGetValueConstRef(containerA, std::string("AA"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 10); // The result value will be unchanged through the call to TryGet.
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueConstRef(containerA, std::string("B"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 20);
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueConstRef(containerA, std::string("C"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 30);
                }
            }

            {
                int* resultRef = nullptr;
                GUGU_UTEST_CHECK_EQUAL(resultRef, nullptr);

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueRef(containerA, std::string("A"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 10);
                }

                if (GUGU_UTEST_CHECK_FALSE(StdMapTryGetValueRef(containerA, std::string("AA"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 10); // The result value will be unchanged through the call to TryGet.
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueRef(containerA, std::string("B"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 20);
                }

                if (GUGU_UTEST_CHECK_TRUE(StdMapTryGetValueRef(containerA, std::string("C"), resultRef)))
                {
                    GUGU_UTEST_CHECK_EQUAL(*resultRef, 30);
                }
            }

            std::map<int, int*> containerC;
            containerC.insert(std::make_pair(1, new int(1)));
            containerC.insert(std::make_pair(2, new int(2)));
            containerC.insert(std::make_pair(3, new int(3)));
            containerC.insert(std::make_pair(4, new int(4)));
            GUGU_UTEST_CHECK_EQUAL(containerC.size(), 4);
            ClearStdMap(containerC);
            GUGU_UTEST_CHECK_TRUE(containerC.empty());
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("Hash");
    {
        Hash hashA("");
        Hash hashB("Hello World");

        GUGU_UTEST_CHECK(hashA == Hash(""));
        GUGU_UTEST_CHECK(hashB == Hash("Hello World"));
        GUGU_UTEST_CHECK(Hash("") == Hash(""));
        GUGU_UTEST_CHECK(Hash("Hello World") == Hash("Hello World"));
        GUGU_UTEST_CHECK(hashA != hashB);
        GUGU_UTEST_CHECK(hashA != Hash("Hello World"));
        GUGU_UTEST_CHECK(hashB != Hash(""));
        GUGU_UTEST_CHECK(hashA.ToInt() != 0);
        GUGU_UTEST_CHECK(hashB.ToInt() != 0);
        GUGU_UTEST_CHECK(hashA.ToInt() != hashB.ToInt());
        GUGU_UTEST_CHECK(ToString(hashA) == ToString(hashA.ToInt()));

        std::vector<Hash> vector_hash;
        std::set<Hash> set_hash;
        std::map<Hash, std::string> map_hash;

        vector_hash.push_back(Hash());
        set_hash.insert(Hash());
        map_hash.insert(std::make_pair(Hash(), ""));
        map_hash[Hash()] = "";
        std::sort(vector_hash.begin(), vector_hash.end());
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("UUID");
    {
        UUID uuidA = GenerateUUID();
        UUID uuidB = GenerateUUID();
        UUID uuidAA = uuidA;

        GUGU_UTEST_CHECK_NOT_EQUAL(uuidA, uuidB);
        GUGU_UTEST_CHECK_NOT_EQUAL(uuidAA, uuidB);
        GUGU_UTEST_CHECK_EQUAL(uuidA, uuidAA);
        GUGU_UTEST_CHECK(uuidA != uuidB);
        GUGU_UTEST_CHECK(uuidAA != uuidB);
        GUGU_UTEST_CHECK(uuidA == uuidAA);

        GUGU_UTEST_CHECK_EQUAL(ToString(uuidA), uuidA.ToString());
        GUGU_UTEST_CHECK_EQUAL(ToString(uuidB), uuidB.ToString());
        GUGU_UTEST_CHECK_NOT_EQUAL(ToString(uuidA), ToString(uuidB));
        GUGU_UTEST_CHECK_NOT_EQUAL(uuidA.ToString(), uuidB.ToString());

        GUGU_UTEST_CHECK_EQUAL(UUID::FromString(ToString(uuidA)), uuidA);
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString(ToString(uuidB)), uuidB);
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString(uuidA.ToString()), uuidA);
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString(uuidB.ToString()), uuidB);
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString(ToString(uuidA)), uuidB);
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString(ToString(uuidB)), uuidA);
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString(uuidA.ToString()), uuidB);
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString(uuidB.ToString()), uuidA);

        // Test leading zeroes.
        // 0x0a9468dfa5c24e4d 0xb107820bc502ccea -> 00762349546700426829 12756307456611765482
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString("0a9468dfa5c24e4db107820bc502ccea").ToString(), "0a9468dfa5c24e4db107820bc502ccea");

        // Test uppercase.
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString("9052F2915FAC4B75A0C93547D1A0ADB3"), UUID::FromString("9052f2915fac4b75a0c93547d1a0adb3"));
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString("38c33747ec1d4dc784267b0ac2f0030b"), UUID::FromString("9052f2915fac4b75a0c93547d1a0adb3"));
        
        // Test invalid size.
        GUGU_UTEST_CHECK_NOT_EQUAL(UUID::FromString("11112222333344445555666677778888"), UUID());
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString("0"), UUID());
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString("1111"), UUID());
        GUGU_UTEST_CHECK_EQUAL(UUID::FromString("1111222233334444555566667777888811112222333344445555666677778888"), UUID());

        std::vector<std::string> uuids;
        bool validSize = true;
        bool validChars = true;
        bool allDifferent = true;

        for (size_t n = 0; n < 100; ++n)
        {
            std::string uuid = GenerateUUIDAsString();
            uuids.push_back(uuid);

            validSize &= uuid.size() == 32;

            for (size_t i = 0; i < uuid.size(); ++i)
            {
                validChars &= (uuid[i] >= '0' && uuid[i] <= '9') || (uuid[i] >= 'a' && uuid[i] <= 'f');
            }
        }

        for (size_t i = 0; i < uuids.size(); ++i)
        {
            for (size_t ii = 0; ii < uuids.size(); ++ii)
            {
                allDifferent &= i == ii || uuids[i] != uuids[ii];
            }
        }

        GUGU_UTEST_CHECK(validSize);
        GUGU_UTEST_CHECK(validChars);
        GUGU_UTEST_CHECK(allDifferent);

        std::vector<UUID> vector_uuid;
        std::set<UUID> set_uuid;
        std::map<UUID, std::string> map_uuid;

        vector_uuid.push_back(GenerateUUID());
        set_uuid.insert(GenerateUUID());
        map_uuid.insert(std::make_pair(GenerateUUID(), ""));
        map_uuid[GenerateUUID()] = "";
        std::sort(vector_uuid.begin(), vector_uuid.end());
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("BitFlag");
    {
        GUGU_UTEST_SUBSECTION("EnumClass");
        {
            enum class EBitFlag : uint8
            {
                Flag_None = 0,
                Flag_A = 1 << 0,
                Flag_B = 1 << 1,
                Flag_C = 1 << 2,
                Flag_D = 1 << 3,
                Flag_E = 1 << 4,
                Flag_F = 1 << 5,
                //Flag_G = 1 << 6,
                //Flag_H = 1 << 7,
            };

            EBitFlag flags = CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C);

            GUGU_UTEST_CHECK_EQUAL((uint8)flags, 7);
            GUGU_UTEST_CHECK_EQUAL(flags, (EBitFlag)7);
            GUGU_UTEST_CHECK_EQUAL(CombineFlags(flags, EBitFlag::Flag_D), CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C, EBitFlag::Flag_D));
            GUGU_UTEST_CHECK_NOT_EQUAL(CombineFlags(flags, EBitFlag::Flag_D), flags);

            UnsetFlag(flags, EBitFlag::Flag_A);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(EBitFlag::Flag_B, EBitFlag::Flag_C));
            UnsetFlag(flags, EBitFlag::Flag_B);
            GUGU_UTEST_CHECK_EQUAL(flags, EBitFlag::Flag_C);
            UnsetFlag(flags, EBitFlag::Flag_C);
            GUGU_UTEST_CHECK_EQUAL(flags, EBitFlag::Flag_None);

            SetFlag(flags, EBitFlag::Flag_A);
            GUGU_UTEST_CHECK_EQUAL(flags, EBitFlag::Flag_A);
            SetFlag(flags, EBitFlag::Flag_B);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B));
            SetFlag(flags, EBitFlag::Flag_C);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C));

            ToggleFlag(flags, EBitFlag::Flag_D);
            GUGU_UTEST_CHECK_NOT_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C));
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C, EBitFlag::Flag_D));

            ToggleFlag(flags, EBitFlag::Flag_D);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C));
            GUGU_UTEST_CHECK_NOT_EQUAL(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C, EBitFlag::Flag_D));

            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, EBitFlag::Flag_A));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, EBitFlag::Flag_B));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, EBitFlag::Flag_C));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, EBitFlag::Flag_D));

            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_C)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(EBitFlag::Flag_B, EBitFlag::Flag_C)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_B, EBitFlag::Flag_C, EBitFlag::Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(EBitFlag::Flag_A, EBitFlag::Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(EBitFlag::Flag_B, EBitFlag::Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(EBitFlag::Flag_C, EBitFlag::Flag_D)));

            GUGU_UTEST_CHECK_TRUE(HasAnyFlag(flags, CombineFlags(EBitFlag::Flag_C, EBitFlag::Flag_D)));
            GUGU_UTEST_CHECK_TRUE(HasAnyFlag(flags, EBitFlag::Flag_C));
            GUGU_UTEST_CHECK_FALSE(HasAnyFlag(flags, EBitFlag::Flag_D));
        }

        GUGU_UTEST_SUBSECTION("Integer");
        {
            int Flag_None = 0;
            int Flag_A = 1 << 0;
            int Flag_B = 1 << 1;
            int Flag_C = 1 << 2;
            int Flag_D = 1 << 3;
            int Flag_E = 1 << 4;
            int Flag_F = 1 << 5;

            int flags = CombineFlags(Flag_A, Flag_B, Flag_C);

            GUGU_UTEST_CHECK_EQUAL(flags, 7);
            GUGU_UTEST_CHECK_EQUAL(flags, (Flag_A | Flag_B | Flag_C));
            GUGU_UTEST_CHECK_EQUAL(CombineFlags(flags, Flag_D), CombineFlags(Flag_A, Flag_B, Flag_C, Flag_D));
            GUGU_UTEST_CHECK_NOT_EQUAL(CombineFlags(flags, Flag_D), flags);

            UnsetFlag(flags, Flag_A);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(Flag_B, Flag_C));
            UnsetFlag(flags, Flag_B);
            GUGU_UTEST_CHECK_EQUAL(flags, Flag_C);
            UnsetFlag(flags, Flag_C);
            GUGU_UTEST_CHECK_EQUAL(flags, Flag_None);

            SetFlag(flags, Flag_A);
            GUGU_UTEST_CHECK_EQUAL(flags, Flag_A);
            SetFlag(flags, Flag_B);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(Flag_A, Flag_B));
            SetFlag(flags, Flag_C);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(Flag_A, Flag_B, Flag_C));

            ToggleFlag(flags, Flag_D);
            GUGU_UTEST_CHECK_NOT_EQUAL(flags, CombineFlags(Flag_A, Flag_B, Flag_C));
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(Flag_A, Flag_B, Flag_C, Flag_D));

            ToggleFlag(flags, Flag_D);
            GUGU_UTEST_CHECK_EQUAL(flags, CombineFlags(Flag_A, Flag_B, Flag_C));
            GUGU_UTEST_CHECK_NOT_EQUAL(flags, CombineFlags(Flag_A, Flag_B, Flag_C, Flag_D));

            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, Flag_A));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, Flag_B));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, Flag_C));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, Flag_D));

            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(Flag_A, Flag_B, Flag_C)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(Flag_A, Flag_B)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(Flag_A, Flag_C)));
            GUGU_UTEST_CHECK_TRUE(HasFlag(flags, CombineFlags(Flag_B, Flag_C)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(Flag_A, Flag_B, Flag_C, Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(Flag_A, Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(Flag_B, Flag_D)));
            GUGU_UTEST_CHECK_FALSE(HasFlag(flags, CombineFlags(Flag_C, Flag_D)));

            GUGU_UTEST_CHECK_TRUE(HasAnyFlag(flags, CombineFlags(Flag_C, Flag_D)));
            GUGU_UTEST_CHECK_TRUE(HasAnyFlag(flags, Flag_C));
            GUGU_UTEST_CHECK_FALSE(HasAnyFlag(flags, Flag_D));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
