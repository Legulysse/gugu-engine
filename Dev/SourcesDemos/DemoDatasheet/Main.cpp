////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "DatasheetBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Audio/ManagerAudio.h"

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

    DatasheetBinding_Register();

    //TODO: Rename Class to Sheet ?
    DS_General* pGeneralA = GetResources()->GetDatasheet<DS_General>("Robert.general");
    DS_General* pGeneralB = GetResources()->GetDatasheet<DS_General>("Joffrey.general");
    DS_Faction* pFaction = GetResources()->GetDatasheet<DS_Faction>("Yumeda.faction");
    DS_Troop* pTroop = GetResources()->GetDatasheet<DS_Troop>("Dragon.troop");
    DS_Skill* pSkill = GetResources()->GetDatasheet<DS_Skill>("Fireball.skill");

    // Enums methods
    size_t iSizeWeaponTypesA = EWeaponType::GetSize();
    size_t iSizeWeaponTypesB = 0;
    DatasheetEnum* pEnum = EWeaponType::GetDatasheetEnum();
    if (pEnum)
    {
        iSizeWeaponTypesB = pEnum->values.size();
    }

    std::vector<EWeaponType::Type> vecWeaponTypes;
    EWeaponType::GetEnumValues(vecWeaponTypes);
    size_t iSizeWeaponTypesC = vecWeaponTypes.size();

    //TODO: Get all datasheets by type

    //----------------------------------------------

    GetEngine()->Release();

    return 0;
}
