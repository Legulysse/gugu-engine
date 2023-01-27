////////////////////////////////////////////////////////////////
// Header

#include "DataBinding_Generated.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

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

void DS_SpriteInfo::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "imageset", m_imageSet);
    gugu::DataBinding::ReadString(context, "animset", m_animSet);
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

void DS_Entity::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", m_name);
    gugu::DataBinding::ReadInt(context, "life", m_life);
    gugu::DataBinding::ReadInt(context, "stamina", m_stamina);
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
    SafeDelete(m_sprite);
    SafeDelete(m_sprite2);
    m_factions.clear();
    ClearStdVector(m_sprites);
    SafeDelete(playableCondition);
}

void DS_General::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Entity::ParseMembers(context);

    gugu::DataBinding::ReadFloat(context, "speed", m_speed);
    gugu::DataBinding::ReadBool(context, "is monarch", m_isMonarch);
    gugu::DataBinding::ReadDatasheetReference(context, "faction", m_faction);
    gugu::DataBinding::ReadDatasheetInstance(context, "sprite", "spriteInfo", m_sprite);
    gugu::DataBinding::ReadDatasheetInstance(context, "sprite 2", "spriteInfo", m_sprite2);
    gugu::DataBinding::ReadStringArray(context, "names list", m_names);
    gugu::DataBinding::ReadIntArray(context, "stats list", m_stats);
    gugu::DataBinding::ReadDatasheetReferenceArray(context, "factions list", m_factions);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "more sprites", "spriteInfo", m_sprites);
    gugu::DataBinding::ReadEnum(context, "weapon", "weaponType", m_weapon);
    gugu::DataBinding::ReadEnumArray(context, "available weapons", "weaponType", m_availableWeapons);
    gugu::DataBinding::ReadDatasheetInstance(context, "playableCondition", "condition", playableCondition);
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

void DS_Faction::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", m_name);
    gugu::DataBinding::ReadDatasheetReference(context, "leader", m_leader);
}

////////////////////////////////////////////////////////////////
DS_Troop::DS_Troop()
{
    m_name = "";
}

DS_Troop::~DS_Troop()
{
}

void DS_Troop::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", m_name);
}

////////////////////////////////////////////////////////////////
DS_Item::DS_Item()
{
    name = "";
}

DS_Item::~DS_Item()
{
}

void DS_Item::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", name);
}

////////////////////////////////////////////////////////////////
DS_Restriction::DS_Restriction()
{
}

DS_Restriction::~DS_Restriction()
{
}

void DS_Restriction::ParseMembers(gugu::DatasheetParserContext& context)
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

void DS_RestrictionFaction::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Restriction::ParseMembers(context);

    gugu::DataBinding::ReadDatasheetReference(context, "faction", m_faction);
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

void DS_RestrictionTroop::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Restriction::ParseMembers(context);

    gugu::DataBinding::ReadDatasheetReference(context, "troop", m_troop);
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
    SafeDelete(m_emptyRestriction);
    SafeDelete(m_nullRestriction);
    SafeDelete(m_singleRestrictionBase);
    SafeDelete(m_singleRestrictionFaction);
    ClearStdVector(m_restrictions);
}

void DS_Skill::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", m_name);
    gugu::DataBinding::ReadDatasheetInstance(context, "empty restriction", "restriction", m_emptyRestriction);
    gugu::DataBinding::ReadDatasheetInstance(context, "null restriction", "restriction", m_nullRestriction);
    gugu::DataBinding::ReadDatasheetInstance(context, "single restriction base", "restriction", m_singleRestrictionBase);
    gugu::DataBinding::ReadDatasheetInstance(context, "single restriction faction", "restriction", m_singleRestrictionFaction);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "restrictions", "restriction", m_restrictions);
}

////////////////////////////////////////////////////////////////
DS_Condition::DS_Condition()
{
    intendedResult = true;
}

DS_Condition::~DS_Condition()
{
}

void DS_Condition::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadBool(context, "intendedResult", intendedResult);
}

////////////////////////////////////////////////////////////////
DS_ConditionAnd::DS_ConditionAnd()
{
}

DS_ConditionAnd::~DS_ConditionAnd()
{
    ClearStdVector(conditions);
}

void DS_ConditionAnd::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Condition::ParseMembers(context);

    gugu::DataBinding::ReadDatasheetInstanceArray(context, "conditions", "condition", conditions);
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

void DS_ConditionPlayerLevel::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Condition::ParseMembers(context);

    gugu::DataBinding::ReadInt(context, "minLevel", minLevel);
    gugu::DataBinding::ReadInt(context, "maxLevel", maxLevel);
}

////////////////////////////////////////////////////////////////
DS_GameSave::DS_GameSave()
{
    experience = 0;
    name = "";
    weapon = EWeaponType::Unknown;
    general = nullptr;
    singleItem = nullptr;
}

DS_GameSave::~DS_GameSave()
{
    general = nullptr;
    generals.clear();
    SafeDelete(singleItem);
    ClearStdVector(multipleItems);
}

void DS_GameSave::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadInt(context, "experience", experience);
    gugu::DataBinding::ReadString(context, "name", name);
    gugu::DataBinding::ReadIntArray(context, "stats", stats);
    gugu::DataBinding::ReadStringArray(context, "names", names);
    gugu::DataBinding::ReadEnum(context, "weapon", "weaponType", weapon);
    gugu::DataBinding::ReadEnumArray(context, "weapons", "weaponType", weapons);
    gugu::DataBinding::ReadDatasheetReference(context, "general", general);
    gugu::DataBinding::ReadDatasheetReferenceArray(context, "generals", generals);
    gugu::DataBinding::ReadDatasaveInstance(context, "singleItem", "itemSave", singleItem);
    gugu::DataBinding::ReadDatasaveInstanceArray(context, "multipleItems", "itemSave", multipleItems);
}

void DS_GameSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::DataBinding::WriteInt(context, "experience", experience);
    gugu::DataBinding::WriteString(context, "name", name);
    gugu::DataBinding::WriteIntArray(context, "stats", stats);
    gugu::DataBinding::WriteStringArray(context, "names", names);
    gugu::DataBinding::WriteEnum(context, "weapon", "weaponType", weapon);
    gugu::DataBinding::WriteEnumArray(context, "weapons", "weaponType", weapons);
    gugu::DataBinding::WriteDatasheetReference(context, "general", general);
    gugu::DataBinding::WriteDatasheetReferenceArray(context, "generals", generals);
    gugu::DataBinding::WriteDatasaveInstance(context, "singleItem", "itemSave", singleItem);
    gugu::DataBinding::WriteDatasaveInstanceArray(context, "multipleItems", "itemSave", multipleItems);
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

void DS_ItemSave::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadDatasheetReference(context, "item", item);
    gugu::DataBinding::ReadInt(context, "quantity", quantity);
}

void DS_ItemSave::SerializeMembers(gugu::DataSaveContext& context) const
{
    //gugu::DatasheetObject::SerializeMembers(context);

    gugu::DataBinding::WriteDatasheetReference(context, "item", item);
    gugu::DataBinding::WriteInt(context, "quantity", quantity);
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

} // namespace demoproject
