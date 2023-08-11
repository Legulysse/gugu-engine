////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "DataBinding/DataBinding.h"
#include "DataBinding/DataBindingImpl.h"

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

            if (GUGU_UTEST_CHECK(enumInfos != nullptr))
            {
                GUGU_UTEST_CHECK(enumInfos->values.size() == 5);

                GUGU_UTEST_CHECK(enumInfos == EWeaponType::GetDataEnumInfos());
                GUGU_UTEST_CHECK(EWeaponType::GetSize() == 5);

                std::vector<EWeaponType::Type> enumValues;
                EWeaponType::GetEnumValues(enumValues);

                GUGU_UTEST_CHECK(enumValues.size() == 5);
            }
        }

        GUGU_UTEST_SUBSECTION("Classes");
        {
            const DS_Character* billySheet = GetResources()->GetDatasheetObject<DS_Character>("Billy.character");
            if (GUGU_UTEST_CHECK(billySheet != nullptr))
            {
                GUGU_UTEST_CHECK(billySheet->name == "Billy");
                GUGU_UTEST_CHECK(billySheet->health == 250);
                GUGU_UTEST_CHECK(billySheet->stamina == 200);
                GUGU_UTEST_CHECK(billySheet->speed == 100);
                GUGU_UTEST_CHECK(billySheet->weapon == EWeaponType::Crossbow);
            }

            const DS_Character* paulaSheet = GetResources()->GetDatasheetObject<DS_Character>("Paula.character");
            if (GUGU_UTEST_CHECK(paulaSheet != nullptr))
            {
                GUGU_UTEST_CHECK(paulaSheet->name == "Paula");
                GUGU_UTEST_CHECK(paulaSheet->health == 200);
                GUGU_UTEST_CHECK(paulaSheet->stamina == 250);
                GUGU_UTEST_CHECK(paulaSheet->speed == 125);
                GUGU_UTEST_CHECK(paulaSheet->weapon == EWeaponType::Mace);
            }

            const DS_Item* appleSheet = GetResources()->GetDatasheetObject<DS_Item>("Apple.item");
            if (GUGU_UTEST_CHECK(appleSheet != nullptr))
            {
                GUGU_UTEST_CHECK(appleSheet->name == "Apple");
            }

            const DS_Item* bananaSheet = GetResources()->GetDatasheetObject<DS_Item>("Banana.item");
            if (GUGU_UTEST_CHECK(bananaSheet != nullptr))
            {
                GUGU_UTEST_CHECK(bananaSheet->name == "Banana");
            }
        }
    }

    GUGU_UTEST_SECTION("Datasaves");
    {
        // Setup
        GUGU_UTEST_SILENT_CHECK(!DirectoryExists("User"));
        GUGU_UTEST_SILENT_CHECK(EnsureDirectoryExists("User"));

        static const std::string emptySaveExpectedResultString = {
            "<Datasave serializationVersion=\"1\" bindingVersion=\"1\">"
                "<Data name=\"readTutorial\" value=\"false\"/>"
                "<Data name=\"score\" value=\"-1\"/>"
                "<Data name=\"walkedDistance\" value=\"0\"/>"
                "<Data name=\"name\" value=\"DEFAULT\"/>"
                "<Data name=\"singleWeapon\" value=\"Unknown\"/>"
                "<Data name=\"gridPosition\" x=\"0\" y=\"0\"/>"
                "<Data name=\"position\" x=\"0\" y=\"0\"/>"
                "<Data name=\"emptyCharacter\" value=\"\"/>"
                "<Data name=\"singleCharacter\" value=\"\"/>"
                "<Data name=\"emptyItem\" type=\"\"/>"
                "<Data name=\"singleItem\" type=\"\"/>"
                "<Data name=\"multipleBools\"/>"
                "<Data name=\"multipleScores\"/>"
                "<Data name=\"multipleFloats\"/>"
                "<Data name=\"multipleNames\"/>"
                "<Data name=\"multipleWeapons\"/>"
                "<Data name=\"multipleGridPositions\"/>"
                "<Data name=\"multiplePositions\"/>"
                "<Data name=\"multipleCharacters\"/>"
                "<Data name=\"multipleItems\"/>"
            "</Datasave>"
        };

        static const std::string filledSaveExpectedResultString = {
            "<Datasave serializationVersion=\"1\" bindingVersion=\"1\">"
                "<Data name=\"readTutorial\" value=\"true\"/>"
                "<Data name=\"score\" value=\"99\"/>"
                "<Data name=\"walkedDistance\" value=\"12500\"/>"
                "<Data name=\"name\" value=\"Hello World\"/>"
                "<Data name=\"singleWeapon\" value=\"Axe\"/>"
                "<Data name=\"gridPosition\" x=\"15\" y=\"30\"/>"
                "<Data name=\"position\" x=\"45\" y=\"60\"/>"
                "<Data name=\"emptyCharacter\" value=\"\"/>"
                "<Data name=\"singleCharacter\" value=\"Billy.character\"/>"
                "<Data name=\"emptyItem\" type=\"\"/>"
                "<Data name=\"singleItem\" type=\"itemSave\">"
                    "<Data name=\"item\" value=\"Apple.item\"/>"
                    "<Data name=\"quantity\" value=\"10\"/>"
                "</Data>"
                "<Data name=\"multipleBools\">"
                    "<Child value=\"true\"/>"
                    "<Child value=\"false\"/>"
                    "<Child value=\"true\"/>"
                "</Data>"
                "<Data name=\"multipleScores\">"
                    "<Child value=\"1\"/>"
                    "<Child value=\"2\"/>"
                    "<Child value=\"3\"/>"
                    "<Child value=\"4\"/>"
                "</Data>"
                "<Data name=\"multipleFloats\">"
                    "<Child value=\"100\"/>"
                    "<Child value=\"200\"/>"
                    "<Child value=\"300\"/>"
                "</Data>"
                "<Data name=\"multipleNames\">"
                    "<Child value=\"One\"/>"
                    "<Child value=\"Two\"/>"
                    "<Child value=\"Three\"/>"
                    "<Child value=\"Four\"/>"
                "</Data>"
                "<Data name=\"multipleWeapons\">"
                    "<Child value=\"Crossbow\"/>"
                    "<Child value=\"Axe\"/>"
                    "<Child value=\"Mace\"/>"
                    "<Child value=\"Sword\"/>"
                "</Data>"
                "<Data name=\"multipleGridPositions\">"
                    "<Child x=\"1000\" y=\"1500\"/>"
                    "<Child x=\"2000\" y=\"2500\"/>"
                "</Data>"
                "<Data name=\"multiplePositions\">"
                    "<Child x=\"100\" y=\"150\"/>"
                    "<Child x=\"200\" y=\"250\"/>"
                "</Data>"
                "<Data name=\"multipleCharacters\">"
                    "<Child value=\"Billy.character\"/>"
                    "<Child value=\"Paula.character\"/>"
                "</Data>"
                "<Data name=\"multipleItems\">"
                    "<Child type=\"itemSave\">"
                        "<Data name=\"item\" value=\"Apple.item\"/>"
                        "<Data name=\"quantity\" value=\"15\"/>"
                    "</Child>"
                    "<Child type=\"itemSave\">"
                        "<Data name=\"item\" value=\"Banana.item\"/>"
                        "<Data name=\"quantity\" value=\"7\"/>"
                    "</Child>"
                "</Data>"
            "</Datasave>"
        };

        GUGU_UTEST_SUBSECTION("Write");
        {
            DS_GameSave emptyGameSave;
            GUGU_UTEST_CHECK(emptyGameSave.SaveToFile("User/EmptyTestSave.xml"));

            std::string emptySaveResultString;
            GUGU_UTEST_CHECK(emptyGameSave.SaveToString(emptySaveResultString));
            GUGU_UTEST_CHECK(emptySaveResultString == emptySaveExpectedResultString);

            DS_ItemSave* singleItemSave = new DS_ItemSave();
            singleItemSave->item = GetResources()->GetDatasheetObject<DS_Item>("Apple.item");
            singleItemSave->quantity = 10;

            DS_ItemSave* appleItemSave = new DS_ItemSave();
            appleItemSave->item = GetResources()->GetDatasheetObject<DS_Item>("Apple.item");
            appleItemSave->quantity = 15;

            DS_ItemSave* bananaItemSave = new DS_ItemSave();
            bananaItemSave->item = GetResources()->GetDatasheetObject<DS_Item>("Banana.item");
            bananaItemSave->quantity = 7;

            DS_GameSave filledGameSave;
            filledGameSave.readTutorial = true;
            filledGameSave.score = 99;
            filledGameSave.walkedDistance = 12500.f;
            filledGameSave.name = "Hello World";
            filledGameSave.singleWeapon = EWeaponType::Axe;
            filledGameSave.gridPosition = Vector2i(15, 30);
            filledGameSave.position = Vector2f(45.f, 60.f);
            filledGameSave.emptyCharacter = nullptr;
            filledGameSave.singleCharacter = GetResources()->GetDatasheetObject<DS_Character>("Billy.character");
            filledGameSave.emptyItem = nullptr;
            filledGameSave.singleItem = singleItemSave;
            filledGameSave.multipleBools = { true, false, true };
            filledGameSave.multipleScores = { 1, 2, 3, 4 };
            filledGameSave.multipleFloats = { 100.f, 200.f, 300.f };
            filledGameSave.multipleNames = { "One", "Two", "Three", "Four" };
            filledGameSave.multipleWeapons = { EWeaponType::Crossbow, EWeaponType::Axe, EWeaponType::Mace, EWeaponType::Sword };
            filledGameSave.multipleGridPositions = { Vector2i(1000, 1500), Vector2i(2000, 2500) };
            filledGameSave.multiplePositions = { Vector2f(100.f, 150.f), Vector2f(200.f, 250.f) };
            filledGameSave.multipleCharacters = {
                GetResources()->GetDatasheetObject<DS_Character>("Billy.character"),
                GetResources()->GetDatasheetObject<DS_Character>("Paula.character"),
            };
            filledGameSave.multipleItems = { appleItemSave, bananaItemSave };

            GUGU_UTEST_CHECK(filledGameSave.SaveToFile("User/FilledTestSave.xml"));

            std::string filledSaveResultString;
            GUGU_UTEST_CHECK(filledGameSave.SaveToString(filledSaveResultString));
            GUGU_UTEST_CHECK(filledSaveResultString == filledSaveExpectedResultString);
        }

        GUGU_UTEST_SUBSECTION("Read");
        {
            DS_GameSave emptyGameSave;
            GUGU_UTEST_CHECK(emptyGameSave.LoadFromFile("User/EmptyTestSave.xml"));

            std::string resultString;
            GUGU_UTEST_CHECK(emptyGameSave.SaveToString(resultString));
            GUGU_UTEST_CHECK(resultString == emptySaveExpectedResultString);

            GUGU_UTEST_CHECK(emptyGameSave.score == -1);
            GUGU_UTEST_CHECK(emptyGameSave.name == "DEFAULT");
            GUGU_UTEST_CHECK(emptyGameSave.singleWeapon == EWeaponType::Unknown);
            GUGU_UTEST_CHECK(emptyGameSave.gridPosition == Vector2i(0, 0));
            GUGU_UTEST_CHECK(emptyGameSave.position == Vector2f(0.f, 0.f));
            GUGU_UTEST_CHECK(emptyGameSave.emptyCharacter == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.singleCharacter == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.emptyItem == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.singleItem == nullptr);
            GUGU_UTEST_CHECK(emptyGameSave.multipleScores.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleNames.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleWeapons.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleGridPositions.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multiplePositions.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleCharacters.empty());
            GUGU_UTEST_CHECK(emptyGameSave.multipleItems.empty());

            DS_GameSave filledGameSave;
            GUGU_UTEST_CHECK(filledGameSave.LoadFromFile("User/FilledTestSave.xml"));

            std::string filledSaveResultString;
            GUGU_UTEST_CHECK(filledGameSave.SaveToString(filledSaveResultString));
            GUGU_UTEST_CHECK(filledSaveResultString == filledSaveExpectedResultString);

            GUGU_UTEST_CHECK(filledGameSave.readTutorial == true);
            GUGU_UTEST_CHECK(filledGameSave.score == 99);
            GUGU_UTEST_CHECK(filledGameSave.walkedDistance == 12500.f);
            GUGU_UTEST_CHECK(filledGameSave.name == "Hello World");
            GUGU_UTEST_CHECK(filledGameSave.singleWeapon == EWeaponType::Axe);
            GUGU_UTEST_CHECK(filledGameSave.gridPosition == Vector2i(15, 30));
            GUGU_UTEST_CHECK(filledGameSave.position == Vector2f(45.f, 60.f));
            GUGU_UTEST_CHECK(filledGameSave.emptyCharacter == nullptr);
            GUGU_UTEST_CHECK(filledGameSave.singleCharacter == GetResources()->GetDatasheetObject<DS_Character>("Billy.character"));
            GUGU_UTEST_CHECK(filledGameSave.emptyItem == nullptr);
            
            if (GUGU_UTEST_CHECK(filledGameSave.singleItem != nullptr))
            {
                GUGU_UTEST_CHECK(filledGameSave.singleItem->item == GetResources()->GetDatasheetObject<DS_Item>("Apple.item"));
                GUGU_UTEST_CHECK(filledGameSave.singleItem->quantity == 10);
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleBools.size() == 3))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleBools[0] == true);
                GUGU_UTEST_CHECK(filledGameSave.multipleBools[1] == false);
                GUGU_UTEST_CHECK(filledGameSave.multipleBools[2] == true);
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleScores.size() == 4))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleScores[0] == 1);
                GUGU_UTEST_CHECK(filledGameSave.multipleScores[1] == 2);
                GUGU_UTEST_CHECK(filledGameSave.multipleScores[2] == 3);
                GUGU_UTEST_CHECK(filledGameSave.multipleScores[3] == 4);
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleFloats.size() == 3))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleFloats[0] == 100.f);
                GUGU_UTEST_CHECK(filledGameSave.multipleFloats[1] == 200.f);
                GUGU_UTEST_CHECK(filledGameSave.multipleFloats[2] == 300.f);
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleNames.size() == 4))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleNames[0] == "One");
                GUGU_UTEST_CHECK(filledGameSave.multipleNames[1] == "Two");
                GUGU_UTEST_CHECK(filledGameSave.multipleNames[2] == "Three");
                GUGU_UTEST_CHECK(filledGameSave.multipleNames[3] == "Four");
            }
            
            if (GUGU_UTEST_CHECK(filledGameSave.multipleWeapons.size() == 4))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleWeapons[0] == EWeaponType::Crossbow);
                GUGU_UTEST_CHECK(filledGameSave.multipleWeapons[1] == EWeaponType::Axe);
                GUGU_UTEST_CHECK(filledGameSave.multipleWeapons[2] == EWeaponType::Mace);
                GUGU_UTEST_CHECK(filledGameSave.multipleWeapons[3] == EWeaponType::Sword);
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleGridPositions.size() == 2))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleGridPositions[0] == Vector2i(1000, 1500));
                GUGU_UTEST_CHECK(filledGameSave.multipleGridPositions[1] == Vector2i(2000, 2500));
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multiplePositions.size() == 2))
            {
                GUGU_UTEST_CHECK(filledGameSave.multiplePositions[0] == Vector2f(100.f, 150.f));
                GUGU_UTEST_CHECK(filledGameSave.multiplePositions[1] == Vector2f(200.f, 250.f));
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleCharacters.size() == 2))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleCharacters[0] == GetResources()->GetDatasheetObject<DS_Character>("Billy.character"));
                GUGU_UTEST_CHECK(filledGameSave.multipleCharacters[1] == GetResources()->GetDatasheetObject<DS_Character>("Paula.character"));
            }

            if (GUGU_UTEST_CHECK(filledGameSave.multipleItems.size() == 2))
            {
                GUGU_UTEST_CHECK(filledGameSave.multipleItems[0]->item == GetResources()->GetDatasheetObject<DS_Item>("Apple.item"));
                GUGU_UTEST_CHECK(filledGameSave.multipleItems[0]->quantity == 15);
                GUGU_UTEST_CHECK(filledGameSave.multipleItems[1]->item == GetResources()->GetDatasheetObject<DS_Item>("Banana.item"));
                GUGU_UTEST_CHECK(filledGameSave.multipleItems[1]->quantity == 7);
            }
        }

        // Reset
        GUGU_UTEST_SILENT_CHECK(RemoveDirectoryTree("User"));
    }

    //----------------------------------------------

    GUGU_UTEST_FINALIZE();
}

}   // namespace tests
