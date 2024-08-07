////////////////////////////////////////////////////////////////
// Header

#include "DataBinding_Generated.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Memory.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

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
DS_SpriteInfo::DS_SpriteInfo()
{
    m_imageSet = "";
    m_animSet = "";
}

DS_SpriteInfo::~DS_SpriteInfo()
{
}

void DS_SpriteInfo::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "imageset", m_imageSet);
    gugu::binding::ReadString(context, "animset", m_animSet);
}

////////////////////////////////////////////////////////////////
DS_Entity::DS_Entity()
{
    m_name = "Plop";
    m_life = 0;
    m_stamina = 10;
}

DS_Entity::~DS_Entity()
{
}

void DS_Entity::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", m_name);
    gugu::binding::ReadInt(context, "life", m_life);
    gugu::binding::ReadInt(context, "stamina", m_stamina);
}

////////////////////////////////////////////////////////////////
DS_General::DS_General()
{
    m_speed = 0.f;
    m_isMonarch = false;
    m_faction = nullptr;
    m_sprite = nullptr;
    m_sprite2 = nullptr;
    m_weapon = EWeaponType::Sword;
    playableCondition = nullptr;
}

DS_General::~DS_General()
{
    m_faction = nullptr;
    m_sprite = nullptr;
    m_sprite2 = nullptr;
    m_factions.clear();
    m_sprites.clear();
    playableCondition = nullptr;
}

void DS_General::ParseMembers(gugu::DataParseContext& context)
{
    DS_Entity::ParseMembers(context);

    gugu::binding::ReadFloat(context, "speed", m_speed);
    gugu::binding::ReadBool(context, "is monarch", m_isMonarch);
    gugu::binding::ReadDatasheetReference(context, "faction", m_faction);
    gugu::binding::ReadDatasheetInstance(context, "sprite", "spriteInfo", m_sprite);
    gugu::binding::ReadDatasheetInstance(context, "sprite 2", "spriteInfo", m_sprite2);
    gugu::binding::ReadStringArray(context, "names list", m_names);
    gugu::binding::ReadIntArray(context, "stats list", m_stats);
    gugu::binding::ReadDatasheetReferenceArray(context, "factions list", m_factions);
    gugu::binding::ReadDatasheetInstanceArray(context, "more sprites", "spriteInfo", m_sprites);
    gugu::binding::ReadEnum(context, "weapon", "weaponType", m_weapon);
    gugu::binding::ReadEnumArray(context, "available weapons", "weaponType", m_availableWeapons);
    gugu::binding::ReadDatasheetInstance(context, "playableCondition", "condition", playableCondition);
}

////////////////////////////////////////////////////////////////
DS_Faction::DS_Faction()
{
    m_name = "";
    m_leader = nullptr;
}

DS_Faction::~DS_Faction()
{
    m_leader = nullptr;
}

void DS_Faction::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", m_name);
    gugu::binding::ReadDatasheetReference(context, "leader", m_leader);
}

////////////////////////////////////////////////////////////////
DS_Troop::DS_Troop()
{
    m_name = "";
}

DS_Troop::~DS_Troop()
{
}

void DS_Troop::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", m_name);
}

////////////////////////////////////////////////////////////////
DS_Item::DS_Item()
{
    name = "";
    icon = "";
    price = 0;
}

DS_Item::~DS_Item()
{
}

void DS_Item::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadString(context, "icon", icon);
    gugu::binding::ReadInt(context, "price", price);
}

////////////////////////////////////////////////////////////////
DS_Restriction::DS_Restriction()
{
}

DS_Restriction::~DS_Restriction()
{
}

void DS_Restriction::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
DS_RestrictionFaction::DS_RestrictionFaction()
{
    m_faction = nullptr;
}

DS_RestrictionFaction::~DS_RestrictionFaction()
{
    m_faction = nullptr;
}

void DS_RestrictionFaction::ParseMembers(gugu::DataParseContext& context)
{
    DS_Restriction::ParseMembers(context);

    gugu::binding::ReadDatasheetReference(context, "faction", m_faction);
}

////////////////////////////////////////////////////////////////
DS_RestrictionTroop::DS_RestrictionTroop()
{
    m_troop = nullptr;
}

DS_RestrictionTroop::~DS_RestrictionTroop()
{
    m_troop = nullptr;
}

void DS_RestrictionTroop::ParseMembers(gugu::DataParseContext& context)
{
    DS_Restriction::ParseMembers(context);

    gugu::binding::ReadDatasheetReference(context, "troop", m_troop);
}

////////////////////////////////////////////////////////////////
DS_Skill::DS_Skill()
{
    m_name = "";
    m_emptyRestriction = nullptr;
    m_nullRestriction = nullptr;
    m_singleRestrictionBase = nullptr;
    m_singleRestrictionFaction = nullptr;
}

DS_Skill::~DS_Skill()
{
    m_emptyRestriction = nullptr;
    m_nullRestriction = nullptr;
    m_singleRestrictionBase = nullptr;
    m_singleRestrictionFaction = nullptr;
    m_restrictions.clear();
}

void DS_Skill::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", m_name);
    gugu::binding::ReadDatasheetInstance(context, "empty restriction", "restriction", m_emptyRestriction);
    gugu::binding::ReadDatasheetInstance(context, "null restriction", "restriction", m_nullRestriction);
    gugu::binding::ReadDatasheetInstance(context, "single restriction base", "restriction", m_singleRestrictionBase);
    gugu::binding::ReadDatasheetInstance(context, "single restriction faction", "restriction", m_singleRestrictionFaction);
    gugu::binding::ReadDatasheetInstanceArray(context, "restrictions", "restriction", m_restrictions);
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
DS_Player::DS_Player()
{
    name = "";
}

DS_Player::~DS_Player()
{
}

void DS_Player::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
}

////////////////////////////////////////////////////////////////
DS_GameSave::DS_GameSave()
{
    player = nullptr;
}

DS_GameSave::~DS_GameSave()
{
    SafeDelete(player);
}

void DS_GameSave::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadDatasaveInstance(context, "player", "playerSave", player);
}

void DS_GameSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::binding::WriteDatasaveInstance(context, "player", "playerSave", player);
}

const std::string& DS_GameSave::GetDataInstanceType() const
{
    static const std::string dataInstanceType = "gameSave";
    return dataInstanceType;
}

////////////////////////////////////////////////////////////////
DS_PlayerSave::DS_PlayerSave()
{
    money = 0;
}

DS_PlayerSave::~DS_PlayerSave()
{
    ClearStdVector(inventory);
}

void DS_PlayerSave::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadInt(context, "money", money);
    gugu::binding::ReadDatasaveInstanceArray(context, "inventory", "itemSave", inventory);
}

void DS_PlayerSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::binding::WriteInt(context, "money", money);
    gugu::binding::WriteDatasaveInstanceArray(context, "inventory", "itemSave", inventory);
}

const std::string& DS_PlayerSave::GetDataInstanceType() const
{
    static const std::string dataInstanceType = "playerSave";
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
    if (classType == "spriteInfo")
    {
        return new DS_SpriteInfo;
    }
    if (classType == "general")
    {
        return new DS_General;
    }
    if (classType == "faction")
    {
        return new DS_Faction;
    }
    if (classType == "troop")
    {
        return new DS_Troop;
    }
    if (classType == "item")
    {
        return new DS_Item;
    }
    if (classType == "restrictionFaction")
    {
        return new DS_RestrictionFaction;
    }
    if (classType == "restrictionTroop")
    {
        return new DS_RestrictionTroop;
    }
    if (classType == "skill")
    {
        return new DS_Skill;
    }
    if (classType == "conditionAnd")
    {
        return new DS_ConditionAnd;
    }
    if (classType == "conditionPlayerLevel")
    {
        return new DS_ConditionPlayerLevel;
    }
    if (classType == "player")
    {
        return new DS_Player;
    }
    if (classType == "gameSave")
    {
        return new DS_GameSave;
    }
    if (classType == "playerSave")
    {
        return new DS_PlayerSave;
    }
    if (classType == "itemSave")
    {
        return new DS_ItemSave;
    }
    return nullptr;
}

} // namespace demoproject
