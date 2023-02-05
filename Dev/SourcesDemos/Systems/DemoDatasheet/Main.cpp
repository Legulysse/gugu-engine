////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "DataBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

#if defined(GUGU_ENV_VISUAL )

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

#endif

using namespace demoproject;
using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

int main(int argc, char* argv[])
{
#if defined(GUGU_ENV_VISUAL )

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    //----------------------------------------------

    //Init engine
    EngineConfig config;
    config.applicationName = "GuguEngine Demo Datasheets";
    config.pathAssets = "Assets";
    config.defaultFont = "Spaceranger.ttf";
    config.debugFont = "Roboto-Regular.ttf";
    config.gameWindow = EGameWindow::None;

    GetEngine()->Init(config);

    //----------------------------------------------

    DataBinding_Register();

    // Datasheets
    const DS_General* pGeneralA = GetResources()->GetDatasheetObject<DS_General>("Robert.general");
    const DS_General* pGeneralB = GetResources()->GetDatasheetObject<DS_General>("Joffrey.general");
    const DS_Faction* pFaction = GetResources()->GetDatasheetObject<DS_Faction>("Yumeda.faction");
    const DS_Troop* pTroop = GetResources()->GetDatasheetObject<DS_Troop>("Dragon.troop");
    const DS_Skill* pSkill = GetResources()->GetDatasheetObject<DS_Skill>("Fireball.skill");

    ConditionContext context;
    context.playerLevel = 10;
    bool testCondition = pGeneralA->playableCondition && pGeneralA->playableCondition->IsValid(context);

    // Enums methods
    size_t iSizeWeaponTypesA = EWeaponType::GetSize();
    size_t iSizeWeaponTypesB = 0;
    const DataEnumInfos* pEnum = EWeaponType::GetDataEnumInfos();
    if (pEnum)
    {
        iSizeWeaponTypesB = pEnum->values.size();
    }

    std::vector<EWeaponType::Type> vecWeaponTypes;
    EWeaponType::GetEnumValues(vecWeaponTypes);
    size_t iSizeWeaponTypesC = vecWeaponTypes.size();

    //TODO: Get all datasheets by type

    // Datasaves
    DS_GameSave* gameSave = new DS_GameSave;
    gameSave->LoadFromFile("User/gamesave.xml");

    gameSave->experience += 1;
    gameSave->name = "hello world";
    gameSave->names.push_back(StringFormat("hello {0}", gameSave->names.size()));
    gameSave->stats.push_back((int)gameSave->stats.size());
    gameSave->weapon = EWeaponType::Axe;
    gameSave->weapons.push_back((EWeaponType::Type)(gameSave->weapons.size() % EWeaponType::GetSize()));
    gameSave->general = GetResources()->GetDatasheetObject<DS_General>("Robert.general");
    gameSave->generals.push_back(GetResources()->GetDatasheetObject<DS_General>("Joffrey.general"));
    gameSave->generals.push_back(nullptr);

    if (gameSave->singleItem == nullptr)
    {
        DS_ItemSave* itemSave = new DS_ItemSave;
        itemSave->item = nullptr;
        itemSave->quantity = 10;
        gameSave->singleItem = itemSave;
    }

    gameSave->singleItem->quantity += 1;

    DS_ItemSave* itemSaveB = new DS_ItemSave;
    itemSaveB->item = nullptr;
    itemSaveB->quantity = 10;
    gameSave->multipleItems.push_back(itemSaveB);

    gameSave->multipleItems.push_back(nullptr);

    EnsureDirectoryExists("User");
    gameSave->SaveToFile("User/gamesave.xml");

    SafeDelete(gameSave);

    //----------------------------------------------

    GetEngine()->Release();

    return 0;
}
