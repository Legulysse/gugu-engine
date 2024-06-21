////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "DataBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/String.h"
#include "Gugu/System/Platform.h"
#include "Gugu/System/Memory.h"

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
    config.defaultFont = "Roboto-Regular.ttf";
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
    {
        // Integration example
        class PlayerSceneCharacter
        {
        private:

            DS_PlayerSave* m_data;    // reference to serialized data, actually owned by the game save

        public:

            PlayerSceneCharacter(DS_PlayerSave* data)
            {
                m_data = data;
            }

            void ReceiveMoney(int value) const
            {
                m_data->money += value;
            }

            void ReceiveItem(DS_ItemSave* item)
            {
                m_data->inventory.push_back(item);
            }
        };

        // New save
        DS_GameSave* newGameSave = new DS_GameSave;
        newGameSave->player = new DS_PlayerSave;

        // Saving
        newGameSave->SaveToFile("User/Save.xml");

        // Loading
        DS_GameSave* gameSave = new DS_GameSave;
        gameSave->LoadFromFile("User/Save.xml");

        // Gameplay
        DS_ItemSave* itemSave = new DS_ItemSave;
        itemSave->item = nullptr;
        itemSave->quantity = 10;

        gameSave->player->money += 250;
        gameSave->player->inventory.push_back(itemSave);

        // Clear
        SafeDelete(newGameSave);
        SafeDelete(gameSave);
    }

    //----------------------------------------------

    GetEngine()->Release();

    return 0;
}
