////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "DataBinding.h"
#include "DataBindingImpl.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

void RunUnitTests_DataBinding(UnitTestResults* results)
{
    GUGU_UTEST_INIT("DataBinding", "UnitTests_DataBinding.log", results);

    //----------------------------------------------

    GUGU_UTEST_SECTION("Setup");
    {
        GUGU_UTEST_CHECK(GetResources()->GetDataEnumInfos("weaponType") == nullptr);

        DataBinding_Register();

        GUGU_UTEST_CHECK(GetResources()->GetDataEnumInfos("weaponType") != nullptr);
    }

    GUGU_UTEST_SECTION("Datasheets");
    {
        GUGU_UTEST_SUBSECTION("Enums");
        {
            const DataEnumInfos* enumInfos = GetResources()->GetDataEnumInfos("weaponType");

            GUGU_UTEST_CHECK(enumInfos != nullptr);
            GUGU_UTEST_CHECK(enumInfos->values.size() == 5);

            GUGU_UTEST_CHECK(enumInfos == EWeaponType::GetDataEnumInfos());
            GUGU_UTEST_CHECK(EWeaponType::GetSize() == 5);

            std::vector<EWeaponType::Type> enumValues;
            EWeaponType::GetEnumValues(enumValues);

            GUGU_UTEST_CHECK(enumValues.size() == 5);
        }

        GUGU_UTEST_SUBSECTION("Classes");
        {
            // TODO
        }
    }

    GUGU_UTEST_SECTION("Datasaves");
    {
        // Setup
        GUGU_UTEST_SILENT_CHECK(!DirectoryExists("User"));
        GUGU_UTEST_SILENT_CHECK(EnsureDirectoryExists("User"));

        GUGU_UTEST_SUBSECTION("Write");
        {
            DS_GameSave emptyGameSave;

            std::string resultString;
            GUGU_UTEST_CHECK(emptyGameSave.SaveToString(resultString));
            GUGU_UTEST_CHECK(emptyGameSave.SaveToFile("User/TestSave.xml"));

            static const std::string expectedResultString = {
                    "<Datasave serializationVersion=\"1\" bindingVersion=\"1\">"
                        "<Data name=\"score\" value=\"-1\"/>"
                        "<Data name=\"name\" value=\"DEFAULT\"/>"
                        "<Data name=\"multipleScores\"/>"
                        "<Data name=\"multipleNames\"/>"
                        "<Data name=\"singleWeapon\" value=\"Unknown\"/>"
                        "<Data name=\"multipleWeapons\"/>"
                        "<Data name=\"multipleCharacters\"/>"
                        "<Data name=\"multipleItems\"/>"
                    "</Datasave>"
                };

            GUGU_UTEST_CHECK(resultString == expectedResultString);
        }

        GUGU_UTEST_SUBSECTION("Read");
        {
            DS_GameSave emptyGameSave;
            GUGU_UTEST_CHECK(emptyGameSave.LoadFromFile("User/TestSave.xml"));

            GUGU_UTEST_CHECK(emptyGameSave.score == -1);
            GUGU_UTEST_CHECK(emptyGameSave.name == "DEFAULT");
            GUGU_UTEST_CHECK(emptyGameSave.multipleScores.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleNames.empty());
            GUGU_UTEST_CHECK(emptyGameSave.singleWeapon == EWeaponType::Unknown);
            GUGU_UTEST_CHECK(emptyGameSave.multipleWeapons.empty());
            GUGU_UTEST_CHECK(emptyGameSave.emptyCharacter == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.singleCharacter == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.multipleCharacters.empty());
            GUGU_UTEST_CHECK(emptyGameSave.emptyItem == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.singleItem == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.multipleItems.empty());
        }

        // Reset
        GUGU_UTEST_SILENT_CHECK(RemoveDirectoryTree("User"));
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
