////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"

#include "DatasheetBinding.h"

#include "Gugu/Engine.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Audio/ManagerAudio.h"
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

    DatasheetBinding_Register();

    const DS_General* pGeneralA = GetResources()->GetDatasheetObject<DS_General>("Robert.general");
    const DS_General* pGeneralB = GetResources()->GetDatasheetObject<DS_General>("Joffrey.general");
    const DS_Faction* pFaction = GetResources()->GetDatasheetObject<DS_Faction>("Yumeda.faction");
    const DS_Troop* pTroop = GetResources()->GetDatasheetObject<DS_Troop>("Dragon.troop");
    const DS_Skill* pSkill = GetResources()->GetDatasheetObject<DS_Skill>("Fireball.skill");

    ConditionContext context;
    context.playerLevel = 10;
    bool testCondition = pGeneralA->playableCondition && pGeneralA->playableCondition->IsValid(context);

#if 1
    // Test : creation d'une data à la main
    DS_General* pGeneralC = new DS_General;
    pGeneralC->m_life = 42;
    pGeneralC->playableCondition = new DS_ConditionPlayerLevel;
    pGeneralC->playableCondition->intendedResult = true;
    pGeneralC->m_sprites.resize(1);
    pGeneralC->m_sprites[0] = new DS_SpriteInfo;
    pGeneralC->m_sprites[0]->m_animSet = "toto.xml";

    // Test : copie depuis une datasheet
    pGeneralC->m_life = pGeneralA->m_life;

    pGeneralC->playableCondition.DeleteData();
    pGeneralC->playableCondition = pGeneralA->playableCondition.DeepCopy();

    // Test : delete
    delete pGeneralC->playableCondition;
    pGeneralC->playableCondition = nullptr;
    pGeneralC->playableCondition.DeleteData();  // same as delete + set to null

    // Clear test
    SafeDelete(pGeneralC);
#endif

#if 0
    // Test : contournement du const
    auto instanceptr = pGeneralA->playableCondition;            // no compile
    instanceptr->intendedResult = true;                         // --

    // Test : copie brute const vers non-const
    pGeneralC->playableCondition = pGeneralA->playableCondition;// no compile

    // Test: copie de pointeur (à interdire ? forcer deep copie ? weak_ptr ? laisser tel quel ?)
    pGeneralC->playableCondition = &*pGeneralC->playableCondition;// compile

    // Test : edition des data const
    pGeneralA->m_life = 42;                                     // no compile
    pGeneralA->playableCondition = new DS_ConditionPlayerLevel; // no compile
    pGeneralA->playableCondition->intendedResult = true;        // no compile
    pGeneralA->m_sprites.resize(1);                             // no compile
    pGeneralA->m_sprites[0] = new DS_SpriteInfo;                // no compile
    pGeneralA->m_sprites[0]->m_animSet = "toto.xml";            // no compile

    pGeneralA->playableCondition = pGeneralB->playableCondition;// no compile

    // Test : delete des data const
    delete pGeneralA->playableCondition;                        // NOT OK (compile)
    pGeneralA->playableCondition = nullptr;                     // no compile
    pGeneralA->playableCondition.DeleteData();                  // no compile
#endif

    // Enums methods
    size_t iSizeWeaponTypesA = EWeaponType::GetSize();
    size_t iSizeWeaponTypesB = 0;
    const DatasheetEnum* pEnum = EWeaponType::GetDatasheetEnum();
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
