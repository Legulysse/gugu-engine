////////////////////////////////////////////////////////////////
// Header

#include "DataBinding.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace tests {

////////////////////////////////////////////////////////////////
namespace EWeaponType
{
    void Register()
    {
        gugu::DataEnumInfos* enumInfos = new gugu::DataEnumInfos;
        enumInfos->values.reserve(5);
        enumInfos->values.push_back("Unknown");
        enumInfos->values.push_back("Sword");
        enumInfos->values.push_back("Mace");
        enumInfos->values.push_back("Axe");
        enumInfos->values.push_back("Crossbow");

        gugu::GetResources()->RegisterDataEnumInfos("weaponType", enumInfos);
    }

    const gugu::DataEnumInfos* GetDataEnumInfos()
    {
        return gugu::GetResources()->GetDataEnumInfos("weaponType");
    }

    void GetEnumValues(std::vector<EWeaponType::Type>& enumValues)
    {
        enumValues.reserve(5);
        enumValues.push_back(EWeaponType::Unknown);
        enumValues.push_back(EWeaponType::Sword);
        enumValues.push_back(EWeaponType::Mace);
        enumValues.push_back(EWeaponType::Axe);
        enumValues.push_back(EWeaponType::Crossbow);
    }

    size_t GetSize()
    {
        const gugu::DataEnumInfos* enumInfos = gugu::GetResources()->GetDataEnumInfos("weaponType");
        if (enumInfos)
            return enumInfos->values.size();
        return 0;
    }
}

////////////////////////////////////////////////////////////////
DS_Entity::DS_Entity()
{
    name = "Entity Name";
    health = 100;
}

DS_Entity::~DS_Entity()
{
}

void DS_Entity::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadInt(context, "health", health);
}

////////////////////////////////////////////////////////////////
DS_Character::DS_Character()
{
    stamina = 100;
    speed = 100.f;
    weapon = EWeaponType::Sword;
    unlocked = nullptr;
    actionOnDeath = nullptr;
    attackSkill = nullptr;
    supportSkill = nullptr;
}

DS_Character::~DS_Character()
{
    unlocked = nullptr;
    actionOnDeath = nullptr;
    attackSkill = nullptr;
    supportSkill = nullptr;
}

void DS_Character::ParseMembers(gugu::DataParseContext& context)
{
    DS_Entity::ParseMembers(context);

    gugu::binding::ReadInt(context, "stamina", stamina);
    gugu::binding::ReadFloat(context, "speed", speed);
    gugu::binding::ReadEnum(context, "weapon", "weaponType", weapon);
    gugu::binding::ReadDatasheetInstance(context, "unlocked", "condition", unlocked);
    gugu::binding::ReadDatasheetInstance(context, "actionOnDeath", "action", actionOnDeath);
    gugu::binding::ReadDatasheetInstance(context, "attackSkill", "effect", attackSkill);
    gugu::binding::ReadDatasheetInstance(context, "supportSkill", "effect", supportSkill);
}

////////////////////////////////////////////////////////////////
DS_Faction::DS_Faction()
{
    name = "";
    leader = nullptr;
}

DS_Faction::~DS_Faction()
{
    leader = nullptr;
}

void DS_Faction::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadDatasheetReference(context, "leader", leader);
}

////////////////////////////////////////////////////////////////
DS_Item::DS_Item()
{
    name = "";
    size = gugu::Vector2::Zero_i;
    scale = gugu::Vector2::Zero_f;
}

DS_Item::~DS_Item()
{
}

void DS_Item::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadVector2(context, "size", size);
    gugu::binding::ReadVector2(context, "scale", scale);
}

////////////////////////////////////////////////////////////////
DS_Effect::DS_Effect()
{
}

DS_Effect::~DS_Effect()
{
}

void DS_Effect::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
DS_EffectGroup::DS_EffectGroup()
{
}

DS_EffectGroup::~DS_EffectGroup()
{
    effects.clear();
}

void DS_EffectGroup::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadDatasheetInstanceArray(context, "effects", "effect", effects);
}

////////////////////////////////////////////////////////////////
DS_EffectProjectile::DS_EffectProjectile()
{
    speed = 0.f;
    lifetime = 0.f;
    hitEffect = nullptr;
    endEffect = nullptr;
}

DS_EffectProjectile::~DS_EffectProjectile()
{
    hitEffect = nullptr;
    endEffect = nullptr;
}

void DS_EffectProjectile::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadFloat(context, "speed", speed);
    gugu::binding::ReadFloat(context, "lifetime", lifetime);
    gugu::binding::ReadDatasheetInstance(context, "hitEffect", "effect", hitEffect);
    gugu::binding::ReadDatasheetInstance(context, "endEffect", "effect", endEffect);
}

////////////////////////////////////////////////////////////////
DS_EffectArea::DS_EffectArea()
{
    radius = 0;
    tick = 0.f;
    duration = 0.f;
    areaTickEffect = nullptr;
    areaEnterEffect = nullptr;
    areaExitEffect = nullptr;
}

DS_EffectArea::~DS_EffectArea()
{
    areaTickEffect = nullptr;
    areaEnterEffect = nullptr;
    areaExitEffect = nullptr;
}

void DS_EffectArea::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadInt(context, "radius", radius);
    gugu::binding::ReadFloat(context, "tick", tick);
    gugu::binding::ReadFloat(context, "duration", duration);
    gugu::binding::ReadDatasheetInstance(context, "areaTickEffect", "effect", areaTickEffect);
    gugu::binding::ReadDatasheetInstance(context, "areaEnterEffect", "effect", areaEnterEffect);
    gugu::binding::ReadDatasheetInstance(context, "areaExitEffect", "effect", areaExitEffect);
}

////////////////////////////////////////////////////////////////
DS_EffectDamage::DS_EffectDamage()
{
    damage = 0;
    entityVfx = nullptr;
}

DS_EffectDamage::~DS_EffectDamage()
{
    entityVfx = nullptr;
}

void DS_EffectDamage::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadInt(context, "damage", damage);
    gugu::binding::ReadDatasheetReference(context, "entityVfx", entityVfx);
}

////////////////////////////////////////////////////////////////
DS_EffectHeal::DS_EffectHeal()
{
    heal = 0;
    entityVfx = nullptr;
}

DS_EffectHeal::~DS_EffectHeal()
{
    entityVfx = nullptr;
}

void DS_EffectHeal::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadInt(context, "heal", heal);
    gugu::binding::ReadDatasheetReference(context, "entityVfx", entityVfx);
}

////////////////////////////////////////////////////////////////
DS_EffectBuff::DS_EffectBuff()
{
    buff = "";
    value = 0;
    entityVfx = nullptr;
}

DS_EffectBuff::~DS_EffectBuff()
{
    entityVfx = nullptr;
}

void DS_EffectBuff::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadString(context, "buff", buff);
    gugu::binding::ReadInt(context, "value", value);
    gugu::binding::ReadDatasheetReference(context, "entityVfx", entityVfx);
}

////////////////////////////////////////////////////////////////
DS_VFX::DS_VFX()
{
    particle = "";
}

DS_VFX::~DS_VFX()
{
}

void DS_VFX::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "particle", particle);
}

////////////////////////////////////////////////////////////////
DS_Condition::DS_Condition()
{
    intendedResult = true;
}

DS_Condition::~DS_Condition()
{
}

void DS_Condition::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadBool(context, "intendedResult", intendedResult);
}

////////////////////////////////////////////////////////////////
DS_ConditionAnd::DS_ConditionAnd()
{
}

DS_ConditionAnd::~DS_ConditionAnd()
{
    conditions.clear();
}

void DS_ConditionAnd::ParseMembers(gugu::DataParseContext& context)
{
    DS_Condition::ParseMembers(context);

    gugu::binding::ReadDatasheetInstanceArray(context, "conditions", "condition", conditions);
}

////////////////////////////////////////////////////////////////
DS_ConditionPlayerLevel::DS_ConditionPlayerLevel()
{
    minLevel = -1;
    maxLevel = -1;
}

DS_ConditionPlayerLevel::~DS_ConditionPlayerLevel()
{
}

void DS_ConditionPlayerLevel::ParseMembers(gugu::DataParseContext& context)
{
    DS_Condition::ParseMembers(context);

    gugu::binding::ReadInt(context, "minLevel", minLevel);
    gugu::binding::ReadInt(context, "maxLevel", maxLevel);
}

////////////////////////////////////////////////////////////////
DS_ConditionCheckFlag::DS_ConditionCheckFlag()
{
    flag = "";
    raised = false;
}

DS_ConditionCheckFlag::~DS_ConditionCheckFlag()
{
}

void DS_ConditionCheckFlag::ParseMembers(gugu::DataParseContext& context)
{
    DS_Condition::ParseMembers(context);

    gugu::binding::ReadString(context, "flag", flag);
    gugu::binding::ReadBool(context, "raised", raised);
}

////////////////////////////////////////////////////////////////
DS_Action::DS_Action()
{
}

DS_Action::~DS_Action()
{
}

void DS_Action::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
DS_ActionList::DS_ActionList()
{
}

DS_ActionList::~DS_ActionList()
{
    actions.clear();
}

void DS_ActionList::ParseMembers(gugu::DataParseContext& context)
{
    DS_Action::ParseMembers(context);

    gugu::binding::ReadDatasheetInstanceArray(context, "actions", "action", actions);
}

////////////////////////////////////////////////////////////////
DS_ActionIfCondition::DS_ActionIfCondition()
{
    condition = nullptr;
    actionIfTrue = nullptr;
    actionIfFalse = nullptr;
}

DS_ActionIfCondition::~DS_ActionIfCondition()
{
    condition = nullptr;
    actionIfTrue = nullptr;
    actionIfFalse = nullptr;
}

void DS_ActionIfCondition::ParseMembers(gugu::DataParseContext& context)
{
    DS_Action::ParseMembers(context);

    gugu::binding::ReadDatasheetInstance(context, "condition", "condition", condition);
    gugu::binding::ReadDatasheetInstance(context, "actionIfTrue", "action", actionIfTrue);
    gugu::binding::ReadDatasheetInstance(context, "actionIfFalse", "action", actionIfFalse);
}

////////////////////////////////////////////////////////////////
DS_ActionSetFlag::DS_ActionSetFlag()
{
    flag = "";
    raised = false;
}

DS_ActionSetFlag::~DS_ActionSetFlag()
{
}

void DS_ActionSetFlag::ParseMembers(gugu::DataParseContext& context)
{
    DS_Action::ParseMembers(context);

    gugu::binding::ReadString(context, "flag", flag);
    gugu::binding::ReadBool(context, "raised", raised);
}

////////////////////////////////////////////////////////////////
DS_GameSave::DS_GameSave()
{
    readTutorial = false;
    score = -1;
    walkedDistance = 0.f;
    name = "DEFAULT";
    singleWeapon = EWeaponType::Unknown;
    gridPosition = gugu::Vector2::Zero_i;
    position = gugu::Vector2::Zero_f;
    emptyCharacter = nullptr;
    singleCharacter = nullptr;
    emptyItem = nullptr;
    singleItem = nullptr;
}

DS_GameSave::~DS_GameSave()
{
    emptyCharacter = nullptr;
    singleCharacter = nullptr;
    SafeDelete(emptyItem);
    SafeDelete(singleItem);
    multipleCharacters.clear();
    ClearStdVector(multipleItems);
}

void DS_GameSave::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadBool(context, "readTutorial", readTutorial);
    gugu::binding::ReadInt(context, "score", score);
    gugu::binding::ReadFloat(context, "walkedDistance", walkedDistance);
    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadEnum(context, "singleWeapon", "weaponType", singleWeapon);
    gugu::binding::ReadVector2(context, "gridPosition", gridPosition);
    gugu::binding::ReadVector2(context, "position", position);
    gugu::binding::ReadDatasheetReference(context, "emptyCharacter", emptyCharacter);
    gugu::binding::ReadDatasheetReference(context, "singleCharacter", singleCharacter);
    gugu::binding::ReadDatasaveInstance(context, "emptyItem", "itemSave", emptyItem);
    gugu::binding::ReadDatasaveInstance(context, "singleItem", "itemSave", singleItem);
    gugu::binding::ReadBoolArray(context, "multipleBools", multipleBools);
    gugu::binding::ReadIntArray(context, "multipleScores", multipleScores);
    gugu::binding::ReadFloatArray(context, "multipleFloats", multipleFloats);
    gugu::binding::ReadStringArray(context, "multipleNames", multipleNames);
    gugu::binding::ReadEnumArray(context, "multipleWeapons", "weaponType", multipleWeapons);
    gugu::binding::ReadVector2Array(context, "multipleGridPositions", multipleGridPositions);
    gugu::binding::ReadVector2Array(context, "multiplePositions", multiplePositions);
    gugu::binding::ReadDatasheetReferenceArray(context, "multipleCharacters", multipleCharacters);
    gugu::binding::ReadDatasaveInstanceArray(context, "multipleItems", "itemSave", multipleItems);
}

void DS_GameSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::binding::WriteBool(context, "readTutorial", readTutorial);
    gugu::binding::WriteInt(context, "score", score);
    gugu::binding::WriteFloat(context, "walkedDistance", walkedDistance);
    gugu::binding::WriteString(context, "name", name);
    gugu::binding::WriteEnum(context, "singleWeapon", "weaponType", singleWeapon);
    gugu::binding::WriteVector2(context, "gridPosition", gridPosition);
    gugu::binding::WriteVector2(context, "position", position);
    gugu::binding::WriteDatasheetReference(context, "emptyCharacter", emptyCharacter);
    gugu::binding::WriteDatasheetReference(context, "singleCharacter", singleCharacter);
    gugu::binding::WriteDatasaveInstance(context, "emptyItem", "itemSave", emptyItem);
    gugu::binding::WriteDatasaveInstance(context, "singleItem", "itemSave", singleItem);
    gugu::binding::WriteBoolArray(context, "multipleBools", multipleBools);
    gugu::binding::WriteIntArray(context, "multipleScores", multipleScores);
    gugu::binding::WriteFloatArray(context, "multipleFloats", multipleFloats);
    gugu::binding::WriteStringArray(context, "multipleNames", multipleNames);
    gugu::binding::WriteEnumArray(context, "multipleWeapons", "weaponType", multipleWeapons);
    gugu::binding::WriteVector2Array(context, "multipleGridPositions", multipleGridPositions);
    gugu::binding::WriteVector2Array(context, "multiplePositions", multiplePositions);
    gugu::binding::WriteDatasheetReferenceArray(context, "multipleCharacters", multipleCharacters);
    gugu::binding::WriteDatasaveInstanceArray(context, "multipleItems", "itemSave", multipleItems);
}

const std::string& DS_GameSave::GetDataInstanceType() const
{
    static const std::string dataInstanceType = "gameSave";
    return dataInstanceType;
}

////////////////////////////////////////////////////////////////
DS_ItemSave::DS_ItemSave()
{
    item = nullptr;
    quantity = 0;
}

DS_ItemSave::~DS_ItemSave()
{
    item = nullptr;
}

void DS_ItemSave::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadDatasheetReference(context, "item", item);
    gugu::binding::ReadInt(context, "quantity", quantity);
}

void DS_ItemSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::binding::WriteDatasheetReference(context, "item", item);
    gugu::binding::WriteInt(context, "quantity", quantity);
}

const std::string& DS_ItemSave::GetDataInstanceType() const
{
    static const std::string dataInstanceType = "itemSave";
    return dataInstanceType;
}

////////////////////////////////////////////////////////////////
void DataBinding_Register()
{
    EWeaponType::Register();

    gugu::GetResources()->RegisterDataObjectFactory(DataBinding_InstanciateDataObject);
}

////////////////////////////////////////////////////////////////
gugu::DataObject* DataBinding_InstanciateDataObject(std::string_view classType)
{
    if (classType == "character")
    {
        return new DS_Character;
    }
    if (classType == "faction")
    {
        return new DS_Faction;
    }
    if (classType == "item")
    {
        return new DS_Item;
    }
    if (classType == "effectGroup")
    {
        return new DS_EffectGroup;
    }
    if (classType == "effectProjectile")
    {
        return new DS_EffectProjectile;
    }
    if (classType == "effectArea")
    {
        return new DS_EffectArea;
    }
    if (classType == "effectDamage")
    {
        return new DS_EffectDamage;
    }
    if (classType == "effectHeal")
    {
        return new DS_EffectHeal;
    }
    if (classType == "effectBuff")
    {
        return new DS_EffectBuff;
    }
    if (classType == "vfx")
    {
        return new DS_VFX;
    }
    if (classType == "conditionAnd")
    {
        return new DS_ConditionAnd;
    }
    if (classType == "conditionPlayerLevel")
    {
        return new DS_ConditionPlayerLevel;
    }
    if (classType == "conditionCheckFlag")
    {
        return new DS_ConditionCheckFlag;
    }
    if (classType == "actionList")
    {
        return new DS_ActionList;
    }
    if (classType == "actionIfCondition")
    {
        return new DS_ActionIfCondition;
    }
    if (classType == "actionSetFlag")
    {
        return new DS_ActionSetFlag;
    }
    if (classType == "gameSave")
    {
        return new DS_GameSave;
    }
    if (classType == "itemSave")
    {
        return new DS_ItemSave;
    }
    return nullptr;
}

} // namespace tests
