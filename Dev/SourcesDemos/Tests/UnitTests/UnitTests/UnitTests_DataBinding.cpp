////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "AllUnitTests.h"

////////////////////////////////////////////////////////////////
// Includes

#include "DataBinding/DataBinding.h"
#include "DataBinding/DataBindingImpl.h"

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Datasheet.h"
#include "Gugu/System/Platform.h"

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
                GUGU_UTEST_CHECK(enumInfos == EWeaponType::GetDataEnumInfos());
                GUGU_UTEST_CHECK(EWeaponType::GetSize() == 5);

                if (GUGU_UTEST_CHECK(enumInfos->values.size() == 5))
                {
                    GUGU_UTEST_CHECK(enumInfos->values[0] == "Unknown");
                    GUGU_UTEST_CHECK(enumInfos->values[1] == "Sword");
                    GUGU_UTEST_CHECK(enumInfos->values[2] == "Mace");
                    GUGU_UTEST_CHECK(enumInfos->values[3] == "Axe");
                    GUGU_UTEST_CHECK(enumInfos->values[4] == "Crossbow");
                }

                std::vector<EWeaponType::Type> enumValues;
                EWeaponType::GetEnumValues(enumValues);

                if (GUGU_UTEST_CHECK(enumValues.size() == 5))
                {
                    GUGU_UTEST_CHECK(enumValues[0] == EWeaponType::Unknown);
                    GUGU_UTEST_CHECK(enumValues[1] == EWeaponType::Sword);
                    GUGU_UTEST_CHECK(enumValues[2] == EWeaponType::Mace);
                    GUGU_UTEST_CHECK(enumValues[3] == EWeaponType::Axe);
                    GUGU_UTEST_CHECK(enumValues[4] == EWeaponType::Crossbow);
                }
            }
        }

        GUGU_UTEST_SUBSECTION("Classes");
        {
            const DS_Item* baseSheet = GetResources()->GetDatasheetObject<DS_Item>("BaseConsumable.item");
            if (GUGU_UTEST_CHECK(baseSheet != nullptr))
            {
                GUGU_UTEST_CHECK(baseSheet->name == "CONSUMABLE");
                GUGU_UTEST_CHECK(baseSheet->size == Vector2i(0, 0));
                GUGU_UTEST_CHECK(baseSheet->scale == Vector2f(0.f, 0.f));
            }

            const DS_Item* appleSheet = GetResources()->GetDatasheetObject<DS_Item>("Apple.item");
            if (GUGU_UTEST_CHECK(appleSheet != nullptr))
            {
                GUGU_UTEST_CHECK(appleSheet->name == "Apple");
                GUGU_UTEST_CHECK(appleSheet->size == Vector2i(32, 48));
                GUGU_UTEST_CHECK(appleSheet->scale == Vector2f(1.f, 1.2f));
            }

            const DS_Item* bananaSheet = GetResources()->GetDatasheetObject<DS_Item>("Banana.item");
            if (GUGU_UTEST_CHECK(bananaSheet != nullptr))
            {
                GUGU_UTEST_CHECK(bananaSheet->name == "Banana");
                GUGU_UTEST_CHECK(bananaSheet->size == Vector2i(0, 0));
                GUGU_UTEST_CHECK(bananaSheet->scale == Vector2f(0.f, 0.f));
            }
        }

        GUGU_UTEST_SUBSECTION("Polymorphism");
        {
            const DS_Character* paulaSheet = GetResources()->GetDatasheetObject<DS_Character>("Paula.character");
            if (GUGU_UTEST_CHECK(paulaSheet != nullptr))
            {
                GUGU_UTEST_CHECK(paulaSheet->name == "Paula");
                GUGU_UTEST_CHECK(paulaSheet->health == 200);
                GUGU_UTEST_CHECK(paulaSheet->stamina == 250);
                GUGU_UTEST_CHECK(paulaSheet->speed == 125);
                GUGU_UTEST_CHECK(paulaSheet->weapon == EWeaponType::Mace);

                if (GUGU_UTEST_CHECK(paulaSheet->unlocked != nullptr))
                {
                    const DS_ConditionAnd* conditionAnd = dynamic_cast<const DS_ConditionAnd*>(paulaSheet->unlocked);
                    if (GUGU_UTEST_CHECK(conditionAnd != nullptr))
                    {
                        GUGU_UTEST_CHECK(conditionAnd->intendedResult == true);

                        if (GUGU_UTEST_CHECK_EQUAL(conditionAnd->conditions.size(), 2))
                        {
                            const DS_ConditionPlayerLevel* conditionPlayerLevel = dynamic_cast<const DS_ConditionPlayerLevel*>(conditionAnd->conditions[0]);
                            if (GUGU_UTEST_CHECK(conditionPlayerLevel != nullptr))
                            {
                                GUGU_UTEST_CHECK(conditionPlayerLevel->intendedResult == true);
                                GUGU_UTEST_CHECK(conditionPlayerLevel->minLevel == 5);
                                GUGU_UTEST_CHECK(conditionPlayerLevel->maxLevel == 10);
                            }

                            const DS_ConditionCheckFlag* conditionCheckFlag = dynamic_cast<const DS_ConditionCheckFlag*>(conditionAnd->conditions[1]);
                            if (GUGU_UTEST_CHECK(conditionCheckFlag != nullptr))
                            {
                                GUGU_UTEST_CHECK(conditionCheckFlag->intendedResult == false);
                                GUGU_UTEST_CHECK(conditionCheckFlag->flag == "TutorialInProgress");
                                GUGU_UTEST_CHECK(conditionCheckFlag->raised == true);
                            }
                        }
                    }

                    ConditionContext context;

                    std::set<std::string> withRaisedFlag{ "TutorialInProgress" };
                    std::set<std::string> withoutRaisedFlag{ "" };

                    context.playerLevel = 5;
                    context.raisedFlags = withoutRaisedFlag;
                    GUGU_UTEST_CHECK(paulaSheet->unlocked->IsValid(context));
                    context.playerLevel = 10;
                    context.raisedFlags = withoutRaisedFlag;
                    GUGU_UTEST_CHECK(paulaSheet->unlocked->IsValid(context));

                    context.playerLevel = 4;
                    context.raisedFlags = withoutRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));
                    context.playerLevel = 11;
                    context.raisedFlags = withoutRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));

                    context.playerLevel = 5;
                    context.raisedFlags = withRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));
                    context.playerLevel = 10;
                    context.raisedFlags = withRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));

                    context.playerLevel = 4;
                    context.raisedFlags = withRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));
                    context.playerLevel = 11;
                    context.raisedFlags = withRaisedFlag;
                    GUGU_UTEST_CHECK_FALSE(paulaSheet->unlocked->IsValid(context));
                }
            }
        }

        GUGU_UTEST_SUBSECTION("Inheritance and Overrides");
        {
            const DS_Character* billySheet = GetResources()->GetDatasheetObject<DS_Character>("Billy.character");
            if (GUGU_UTEST_CHECK(billySheet != nullptr))
            {
                GUGU_UTEST_CHECK(billySheet->name == "Billy");
                GUGU_UTEST_CHECK(billySheet->health == 250);
                GUGU_UTEST_CHECK(billySheet->stamina == 200);
                GUGU_UTEST_CHECK(billySheet->speed == 100);
                GUGU_UTEST_CHECK(billySheet->weapon == EWeaponType::Crossbow);

                if (GUGU_UTEST_CHECK(billySheet->attackSkill != nullptr))
                {
                    const DS_EffectDamage* effectDamage = dynamic_cast<const DS_EffectDamage*>(billySheet->attackSkill);
                    if (GUGU_UTEST_CHECK(effectDamage != nullptr))
                    {
                        GUGU_UTEST_CHECK_EQUAL(effectDamage->damage, 120);
                    }
                }

                if (GUGU_UTEST_CHECK(billySheet->supportSkill != nullptr))
                {
                    const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(billySheet->supportSkill);
                    if (GUGU_UTEST_CHECK(effectArea != nullptr))
                    {
                        GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 100);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.5f, math::Epsilon3);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 3.f, math::Epsilon3);
                        GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                        const DS_EffectHeal* tickEffectHeal = dynamic_cast<const DS_EffectHeal*>(effectArea->areaTickEffect);
                        if (GUGU_UTEST_CHECK(tickEffectHeal != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->heal, 20);

                            if (GUGU_UTEST_CHECK_NOT_NULL(tickEffectHeal->entityVfx))
                            {
                                GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->entityVfx->GetDatasheet()->GetID(), "TestArea.vfx");
                            }
                        }

                        const DS_EffectHeal* enterEffectHeal = dynamic_cast<const DS_EffectHeal*>(effectArea->areaEnterEffect);
                        if (GUGU_UTEST_CHECK(enterEffectHeal != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(enterEffectHeal->heal, 50);
                        }
                    }
                }
            }

            const DS_Character* billyLvl2Sheet = GetResources()->GetDatasheetObject<DS_Character>("Billy_Lvl_2.character");
            if (GUGU_UTEST_CHECK(billyLvl2Sheet != nullptr))
            {
                GUGU_UTEST_CHECK(billyLvl2Sheet->name == "Billy");
                GUGU_UTEST_CHECK(billyLvl2Sheet->health == 250);
                GUGU_UTEST_CHECK(billyLvl2Sheet->stamina == 220);
                GUGU_UTEST_CHECK(billyLvl2Sheet->speed == 110);
                GUGU_UTEST_CHECK(billyLvl2Sheet->weapon == EWeaponType::Crossbow);

                if (GUGU_UTEST_CHECK(billyLvl2Sheet->attackSkill != nullptr))
                {
                    const DS_EffectGroup* effectgroup = dynamic_cast<const DS_EffectGroup*>(billyLvl2Sheet->attackSkill);
                    if (GUGU_UTEST_CHECK(effectgroup != nullptr))
                    {
                        if (GUGU_UTEST_CHECK_EQUAL(effectgroup->effects.size(), 2))
                        {
                            const DS_EffectDamage* effectDamage = dynamic_cast<const DS_EffectDamage*>(effectgroup->effects[0]);
                            if (GUGU_UTEST_CHECK(effectDamage != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectDamage->damage, 120);
                            }

                            const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(effectgroup->effects[1]);
                            if (GUGU_UTEST_CHECK(effectArea != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 50);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.1f, math::Epsilon3);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 1.f, math::Epsilon3);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaEnterEffect, nullptr);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                                const DS_EffectDamage* tickEffectDamage = dynamic_cast<const DS_EffectDamage*>(effectArea->areaTickEffect);
                                if (GUGU_UTEST_CHECK(tickEffectDamage != nullptr))
                                {
                                    GUGU_UTEST_CHECK_EQUAL(tickEffectDamage->damage, 10);
                                }
                            }
                        }
                    }
                }

                if (GUGU_UTEST_CHECK(billyLvl2Sheet->supportSkill != nullptr))
                {
                    const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(billyLvl2Sheet->supportSkill);
                    if (GUGU_UTEST_CHECK(effectArea != nullptr))
                    {
                        GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 130);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.5f, math::Epsilon3);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 4.f, math::Epsilon3);
                        GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                        const DS_EffectHeal* tickEffectHeal = dynamic_cast<const DS_EffectHeal*>(effectArea->areaTickEffect);
                        if (GUGU_UTEST_CHECK(tickEffectHeal != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->heal, 25);

                            if (GUGU_UTEST_CHECK_NOT_NULL(tickEffectHeal->entityVfx))
                            {
                                GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->entityVfx->GetDatasheet()->GetID(), "TestArea2.vfx");
                            }
                        }

                        const DS_EffectBuff* enterEffectBuff = dynamic_cast<const DS_EffectBuff*>(effectArea->areaEnterEffect);
                        if (GUGU_UTEST_CHECK(enterEffectBuff != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->buff, "Regen");
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->value, 10);
                        }
                    }
                }
            }

            const DS_Character* billyLvl3Sheet = GetResources()->GetDatasheetObject<DS_Character>("Billy_Lvl_3.character");
            if (GUGU_UTEST_CHECK(billyLvl3Sheet != nullptr))
            {
                GUGU_UTEST_CHECK(billyLvl3Sheet->name == "Billy");
                GUGU_UTEST_CHECK(billyLvl3Sheet->health == 250);
                GUGU_UTEST_CHECK(billyLvl3Sheet->stamina == 240);
                GUGU_UTEST_CHECK(billyLvl3Sheet->speed == 120);
                GUGU_UTEST_CHECK(billyLvl3Sheet->weapon == EWeaponType::Crossbow);

                if (GUGU_UTEST_CHECK(billyLvl3Sheet->attackSkill != nullptr))
                {
                    const DS_EffectGroup* effectgroup = dynamic_cast<const DS_EffectGroup*>(billyLvl3Sheet->attackSkill);
                    if (GUGU_UTEST_CHECK(effectgroup != nullptr))
                    {
                        if (GUGU_UTEST_CHECK_EQUAL(effectgroup->effects.size(), 2))
                        {
                            const DS_EffectDamage* effectDamage = dynamic_cast<const DS_EffectDamage*>(effectgroup->effects[0]);
                            if (GUGU_UTEST_CHECK(effectDamage != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectDamage->damage, 130);
                            }

                            const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(effectgroup->effects[1]);
                            if (GUGU_UTEST_CHECK(effectArea != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 75);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.1f, math::Epsilon3);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 1.5f, math::Epsilon3);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaEnterEffect, nullptr);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                                const DS_EffectDamage* tickEffectDamage = dynamic_cast<const DS_EffectDamage*>(effectArea->areaTickEffect);
                                if (GUGU_UTEST_CHECK(tickEffectDamage != nullptr))
                                {
                                    GUGU_UTEST_CHECK_EQUAL(tickEffectDamage->damage, 12);
                                }
                            }
                        }
                    }
                }

                if (GUGU_UTEST_CHECK(billyLvl3Sheet->supportSkill != nullptr))
                {
                    const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(billyLvl3Sheet->supportSkill);
                    if (GUGU_UTEST_CHECK(effectArea != nullptr))
                    {
                        GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 160);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.5f, math::Epsilon3);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 4.f, math::Epsilon3);
                        GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                        const DS_EffectHeal* tickEffectHeal = dynamic_cast<const DS_EffectHeal*>(effectArea->areaTickEffect);
                        if (GUGU_UTEST_CHECK(tickEffectHeal != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->heal, 25);

                            if (GUGU_UTEST_CHECK_NOT_NULL(tickEffectHeal->entityVfx))
                            {
                                GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->entityVfx->GetDatasheet()->GetID(), "TestArea2.vfx");
                            }
                        }

                        const DS_EffectBuff* enterEffectBuff = dynamic_cast<const DS_EffectBuff*>(effectArea->areaEnterEffect);
                        if (GUGU_UTEST_CHECK(enterEffectBuff != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->buff, "Regen");
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->value, 12);
                        }
                    }
                }
            }

            const DS_Character* billyLvl3bisSheet = GetResources()->GetDatasheetObject<DS_Character>("Billy_Lvl_3bis.character");
            if (GUGU_UTEST_CHECK(billyLvl3bisSheet != nullptr))
            {
                GUGU_UTEST_CHECK(billyLvl3bisSheet->name == "Billy");
                GUGU_UTEST_CHECK(billyLvl3bisSheet->health == 250);
                GUGU_UTEST_CHECK(billyLvl3bisSheet->stamina == 240);
                GUGU_UTEST_CHECK(billyLvl3bisSheet->speed == 120);
                GUGU_UTEST_CHECK(billyLvl3bisSheet->weapon == EWeaponType::Crossbow);

                if (GUGU_UTEST_CHECK(billyLvl3bisSheet->attackSkill != nullptr))
                {
                    const DS_EffectGroup* effectgroup = dynamic_cast<const DS_EffectGroup*>(billyLvl3bisSheet->attackSkill);
                    if (GUGU_UTEST_CHECK(effectgroup != nullptr))
                    {
                        if (GUGU_UTEST_CHECK_EQUAL(effectgroup->effects.size(), 2))
                        {
                            const DS_EffectDamage* effectDamage = dynamic_cast<const DS_EffectDamage*>(effectgroup->effects[0]);
                            if (GUGU_UTEST_CHECK(effectDamage != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectDamage->damage, 130);
                            }

                            const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(effectgroup->effects[1]);
                            if (GUGU_UTEST_CHECK(effectArea != nullptr))
                            {
                                GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 75);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.1f, math::Epsilon3);
                                GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 1.5f, math::Epsilon3);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaEnterEffect, nullptr);
                                GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                                const DS_EffectDamage* tickEffectDamage = dynamic_cast<const DS_EffectDamage*>(effectArea->areaTickEffect);
                                if (GUGU_UTEST_CHECK(tickEffectDamage != nullptr))
                                {
                                    GUGU_UTEST_CHECK_EQUAL(tickEffectDamage->damage, 12);
                                }
                            }
                        }
                    }
                }

                if (GUGU_UTEST_CHECK(billyLvl3bisSheet->supportSkill != nullptr))
                {
                    const DS_EffectArea* effectArea = dynamic_cast<const DS_EffectArea*>(billyLvl3bisSheet->supportSkill);
                    if (GUGU_UTEST_CHECK(effectArea != nullptr))
                    {
                        GUGU_UTEST_CHECK_EQUAL(effectArea->radius, 160);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->tick, 0.5f, math::Epsilon3);
                        GUGU_UTEST_CHECK_APPROX_EQUAL(effectArea->duration, 4.f, math::Epsilon3);
                        GUGU_UTEST_CHECK_EQUAL(effectArea->areaExitEffect, nullptr);

                        const DS_EffectHeal* tickEffectHeal = dynamic_cast<const DS_EffectHeal*>(effectArea->areaTickEffect);
                        if (GUGU_UTEST_CHECK(tickEffectHeal != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->heal, 25);

                            if (GUGU_UTEST_CHECK_NOT_NULL(tickEffectHeal->entityVfx))
                            {
                                GUGU_UTEST_CHECK_EQUAL(tickEffectHeal->entityVfx->GetDatasheet()->GetID(), "TestArea2.vfx");
                            }
                        }

                        const DS_EffectBuff* enterEffectBuff = dynamic_cast<const DS_EffectBuff*>(effectArea->areaEnterEffect);
                        if (GUGU_UTEST_CHECK(enterEffectBuff != nullptr))
                        {
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->buff, "Regen");
                            GUGU_UTEST_CHECK_EQUAL(enterEffectBuff->value, 12);
                        }
                    }
                }
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
