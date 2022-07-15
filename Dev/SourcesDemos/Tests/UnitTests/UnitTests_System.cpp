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
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world.xml", false) == "hello/world.xml");

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
            GUGU_UTEST_CHECK(NormalizePath("./hello/my/../world.xml", true) == "hello/world.xml/");
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

        GUGU_UTEST_SUBSECTION("FileFromPathFile");
        {
            GUGU_UTEST_CHECK(FileFromPathFile("") == "");
            GUGU_UTEST_CHECK(FileFromPathFile("./hello/my/../world/") == "");
            GUGU_UTEST_CHECK(FileFromPathFile("./hello/my/../world.xml") == "world.xml");
            GUGU_UTEST_CHECK(FileFromPathFile("./hello/my/../world") == "world");
            GUGU_UTEST_CHECK(FileFromPathFile("./hello/my/../my.world/") == "");
            GUGU_UTEST_CHECK(FileFromPathFile("./hello/my/../my.world/file.xml") == "file.xml");
            GUGU_UTEST_CHECK(FileFromPathFile("file.xml") == "file.xml");
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

    GUGU_UTEST_SECTION("FileInfo");
    {
        GUGU_UTEST_SUBSECTION("Construction");
        {
            GUGU_UTEST_CHECK(FileInfo("hello/world.txt") == FileInfo("hello", "world.txt"));
            GUGU_UTEST_CHECK(FileInfo("world.txt") == FileInfo("", "world.txt"));

            FileInfo infoA("hello", "world.txt");

            GUGU_UTEST_CHECK(infoA.GetPath(false) == "hello");
            GUGU_UTEST_CHECK(infoA.GetPath(true) == "hello/");
            GUGU_UTEST_CHECK(infoA.GetName() == "world.txt");
            GUGU_UTEST_CHECK(infoA.GetPathName() == "hello/world.txt");
            GUGU_UTEST_CHECK(infoA.GetPrettyName() == "world");
            GUGU_UTEST_CHECK(infoA.GetExtension() == "txt");
            GUGU_UTEST_CHECK(infoA.IsExtension("txt"));

            FileInfo infoB("", "world.txt");

            GUGU_UTEST_CHECK(infoB.GetPath(false) == "");
            GUGU_UTEST_CHECK(infoB.GetPath(true) == "");
            GUGU_UTEST_CHECK(infoB.GetName() == "world.txt");
            GUGU_UTEST_CHECK(infoB.GetPathName() == "world.txt");
            GUGU_UTEST_CHECK(infoB.GetPrettyName() == "world");
            GUGU_UTEST_CHECK(infoB.GetExtension() == "txt");
            GUGU_UTEST_CHECK(infoB.IsExtension("txt"));

            FileInfo infoC("", ".txt");

            GUGU_UTEST_CHECK(infoC.GetPath(false) == "");
            GUGU_UTEST_CHECK(infoC.GetPath(true) == "");
            GUGU_UTEST_CHECK(infoC.GetName() == ".txt");
            GUGU_UTEST_CHECK(infoC.GetPathName() == ".txt");
            GUGU_UTEST_CHECK(infoC.GetPrettyName() == "");
            GUGU_UTEST_CHECK(infoC.GetExtension() == "txt");
            GUGU_UTEST_CHECK(infoC.IsExtension("txt"));

            FileInfo infoD("", "world.file.txt");

            GUGU_UTEST_CHECK(infoD.GetPath(false) == "");
            GUGU_UTEST_CHECK(infoD.GetPath(true) == "");
            GUGU_UTEST_CHECK(infoD.GetName() == "world.file.txt");
            GUGU_UTEST_CHECK(infoD.GetPathName() == "world.file.txt");
            GUGU_UTEST_CHECK(infoD.GetPrettyName() == "world");
            GUGU_UTEST_CHECK(infoD.GetExtension() == "txt");
            GUGU_UTEST_CHECK(infoD.IsExtension("txt"));
            GUGU_UTEST_CHECK(infoD.IsExtension("file.txt"));
        }

        GUGU_UTEST_SUBSECTION("Comparison");
        {
            GUGU_UTEST_CHECK(FileInfo("aaa", "111") == FileInfo("aaa", "111"));
            GUGU_UTEST_CHECK(FileInfo("aaa", "111") != FileInfo("aaa", "222"));
            GUGU_UTEST_CHECK(FileInfo("bbb", "111") != FileInfo("aaa", "111"));

            std::vector<FileInfo> fileInfoArray
            {
                FileInfo("aaa", "111"),
                FileInfo("bbb", "222"),
                FileInfo("bbb", "111"),
                FileInfo("aaa", "333"),
            };

            std::sort(fileInfoArray.begin(), fileInfoArray.end());

            GUGU_UTEST_CHECK(fileInfoArray[0] == FileInfo("aaa", "111"));
            GUGU_UTEST_CHECK(fileInfoArray[1] == FileInfo("aaa", "333"));
            GUGU_UTEST_CHECK(fileInfoArray[2] == FileInfo("bbb", "111"));
            GUGU_UTEST_CHECK(fileInfoArray[3] == FileInfo("bbb", "222"));
        }
    }

    //----------------------------------------------

    GUGU_UTEST_SECTION("String");
    {
        GUGU_UTEST_SUBSECTION("StringNumberFormat");
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
        }
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}
