////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/Debug/UnitTest.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

void RunUnitTests_System()
{
    GUGU_UTEST_INIT("System", "UnitTests_System.log");

    //----------------------------------------------

    GUGU_UTEST_SECTION("Path Utility");
    {
        GUGU_UTEST_SUBSECTION("NormalizePath");
        {
            GUGU_UTEST_CHECK(NormalizePath("", false) == "");
            GUGU_UTEST_CHECK(NormalizePath("/", false) == "");
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world", false) == "hello/world");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world", false) == "../hello/world");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../..", false) == "");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../../../..", false) == "../..");
            GUGU_UTEST_CHECK(NormalizePath("../../hello/my/../world/../", false) == "../../hello");
            GUGU_UTEST_CHECK(NormalizePath("./", false) == "");
            GUGU_UTEST_CHECK(NormalizePath("../", false) == "..");
            GUGU_UTEST_CHECK(NormalizePath("/..", false) == "..");
            GUGU_UTEST_CHECK(NormalizePath("/../", false) == "..");
            GUGU_UTEST_CHECK(NormalizePath("/../", false) == "..");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world/..", false) == "../hello");
            GUGU_UTEST_CHECK(NormalizePath("///////", false) == "");
            GUGU_UTEST_CHECK(NormalizePath(".///////", false) == "");
            GUGU_UTEST_CHECK(NormalizePath(".//.///.//", false) == "");
            GUGU_UTEST_CHECK(NormalizePath(".folder/hello../..../..world/....", false) == ".folder/hello../..../..world/....");

            GUGU_UTEST_CHECK(NormalizePath("", true) == "");
            GUGU_UTEST_CHECK(NormalizePath("/", true) == "");
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world", true) == "hello/world/");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world", true) == "../hello/world/");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../..", true) == "");
            GUGU_UTEST_CHECK(NormalizePath("hello/world/../../../..", true) == "../../");
            GUGU_UTEST_CHECK(NormalizePath("../../hello/my/../world/../", true) == "../../hello/");
            GUGU_UTEST_CHECK(NormalizePath("./", true) == "");
            GUGU_UTEST_CHECK(NormalizePath("../", true) == "../");
            GUGU_UTEST_CHECK(NormalizePath("/..", true) == "../");
            GUGU_UTEST_CHECK(NormalizePath("/../", true) == "../");
            GUGU_UTEST_CHECK(NormalizePath("/../", true) == "../");
            GUGU_UTEST_CHECK(NormalizePath("../hello/world/..", true) == "../hello/");
            GUGU_UTEST_CHECK(NormalizePath("///////", true) == "");
            GUGU_UTEST_CHECK(NormalizePath(".///////", true) == "");
            GUGU_UTEST_CHECK(NormalizePath(".//.///.//", true) == "");
            GUGU_UTEST_CHECK(NormalizePath(".folder/hello../..../..world/....", true) == ".folder/hello../..../..world/..../");
        }

        GUGU_UTEST_SUBSECTION("PathFromPathFile");
        {
            GUGU_UTEST_CHECK(PathFromPathFile("", false) == "");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world/", false) == "hello/world");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world.xml", false) == "hello");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world", false) == "hello/world");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../my.world/", false) == "hello/my.world");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../my.world/file.xml", false) == "hello/my.world");
            GUGU_UTEST_CHECK(PathFromPathFile("file.xml", false) == "");

            GUGU_UTEST_CHECK(PathFromPathFile("", true) == "");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world/", true) == "hello/world/");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world.xml", true) == "hello/");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../world", true) == "hello/world/");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../my.world/", true) == "hello/my.world/");
            GUGU_UTEST_CHECK(PathFromPathFile("./hello/my/../my.world/file.xml", true) == "hello/my.world/");
            GUGU_UTEST_CHECK(PathFromPathFile("file.xml", true) == "");
        }

        GUGU_UTEST_SUBSECTION("CombinePaths");
        {
            GUGU_UTEST_CHECK(CombinePaths("", "", false) == "");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "/world/", false) == "hello/world");
            GUGU_UTEST_CHECK(CombinePaths("/", "/", false) == "");
            GUGU_UTEST_CHECK(CombinePaths("../hello", "../world", false) == "../world");
            GUGU_UTEST_CHECK(CombinePaths("../hello/..", "world", false) == "../world");
            GUGU_UTEST_CHECK(CombinePaths("hello/my", "./world", false) == "hello/my/world");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "", false) == "hello");
            GUGU_UTEST_CHECK(CombinePaths("", "/world/", false) == "world");
            GUGU_UTEST_CHECK(CombinePaths("/../", "/../", false) == "../..");
            GUGU_UTEST_CHECK(CombinePaths("..", "..", false) == "../..");

            GUGU_UTEST_CHECK(CombinePaths("", "", true) == "");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "/world/", true) == "hello/world/");
            GUGU_UTEST_CHECK(CombinePaths("/", "/", true) == "");
            GUGU_UTEST_CHECK(CombinePaths("../hello", "../world", true) == "../world/");
            GUGU_UTEST_CHECK(CombinePaths("../hello/..", "world", true) == "../world/");
            GUGU_UTEST_CHECK(CombinePaths("hello/my", "./world", true) == "hello/my/world/");
            GUGU_UTEST_CHECK(CombinePaths("/hello/", "", true) == "hello/");
            GUGU_UTEST_CHECK(CombinePaths("", "/world/", true) == "world/");
            GUGU_UTEST_CHECK(CombinePaths("/../", "/../", true) == "../../");
            GUGU_UTEST_CHECK(CombinePaths("..", "..", true) == "../../");
        }

        GUGU_UTEST_SUBSECTION("CombinePathFile");
        {
            GUGU_UTEST_CHECK(CombinePathFile("..", "file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePathFile("..", "./file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePathFile("../hello/..", "./file.txt") == "../file.txt");
            GUGU_UTEST_CHECK(CombinePathFile("/hello/", "/world/file.txt") == "hello/world/file.txt");
            GUGU_UTEST_CHECK(CombinePathFile("", "file.txt") == "file.txt");
            GUGU_UTEST_CHECK(CombinePathFile("hello", "") == "hello");
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}
