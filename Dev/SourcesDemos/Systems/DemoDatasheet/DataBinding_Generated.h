#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/Data/DatasheetObject.h"
#include "Gugu/Data/DatasaveObject.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

////////////////////////////////////////////////////////////////
class DS_SpriteInfo;
class DS_Entity;
class DS_General;
class DS_Faction;
class DS_Troop;
class DS_Item;
class DS_Restriction;
class DS_RestrictionFaction;
class DS_RestrictionTroop;
class DS_Skill;
class DS_Condition;
class DS_ConditionAnd;
class DS_ConditionPlayerLevel;
class DS_Player;
class DS_GameSave;
class DS_PlayerSave;
class DS_ItemSave;

////////////////////////////////////////////////////////////////
namespace EWeaponType {
    enum Type
    {
        Unknown,
        Sword,
        Mace,
        Axe,
        Crossbow,
    };

    const gugu::DataEnumInfos* GetDataEnumInfos();
    void GetEnumValues(std::vector<EWeaponType::Type>& enumValues);
    size_t GetSize();

    void Register();
}

////////////////////////////////////////////////////////////////
class DS_SpriteInfo : public gugu::DatasheetObject
{
public:

    DS_SpriteInfo();
    virtual ~DS_SpriteInfo();

public:

    std::string m_imageSet;
    std::string m_animSet;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Entity : public gugu::DatasheetObject
{
public:

    DS_Entity();
    virtual ~DS_Entity();

public:

    std::string m_name;
    int m_life;
    int m_stamina;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_General : public DS_Entity
{
public:

    DS_General();
    virtual ~DS_General();

public:

    float m_speed;
    bool m_isMonarch;
    const DS_Faction* m_faction;
    const DS_SpriteInfo* m_sprite;
    const DS_SpriteInfo* m_sprite2;
    std::vector<std::string> m_names;
    std::vector<int> m_stats;
    std::vector<const DS_Faction*> m_factions;
    std::vector<const DS_SpriteInfo*> m_sprites;
    EWeaponType::Type m_weapon;
    std::vector<EWeaponType::Type> m_availableWeapons;
    const DS_Condition* playableCondition;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Faction : public gugu::DatasheetObject
{
public:

    DS_Faction();
    virtual ~DS_Faction();

public:

    std::string m_name;
    const DS_Entity* m_leader;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Troop : public gugu::DatasheetObject
{
public:

    DS_Troop();
    virtual ~DS_Troop();

public:

    std::string m_name;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Item : public gugu::DatasheetObject
{
public:

    DS_Item();
    virtual ~DS_Item();

public:

    std::string name;
    std::string icon;
    int price;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Restriction : public gugu::DatasheetObject
{
public:

    DS_Restriction();
    virtual ~DS_Restriction();

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_RestrictionFaction : public DS_Restriction
{
public:

    DS_RestrictionFaction();
    virtual ~DS_RestrictionFaction();

public:

    const DS_Faction* m_faction;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_RestrictionTroop : public DS_Restriction
{
public:

    DS_RestrictionTroop();
    virtual ~DS_RestrictionTroop();

public:

    const DS_Troop* m_troop;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Skill : public gugu::DatasheetObject
{
public:

    DS_Skill();
    virtual ~DS_Skill();

public:

    std::string m_name;
    const DS_Restriction* m_emptyRestriction;
    const DS_Restriction* m_nullRestriction;
    const DS_Restriction* m_singleRestrictionBase;
    const DS_Restriction* m_singleRestrictionFaction;
    std::vector<const DS_Restriction*> m_restrictions;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Condition : public gugu::DatasheetObject
{
public:

    DS_Condition();
    virtual ~DS_Condition();

public:

    bool IsValid(const struct ConditionContext& context) const;
    virtual bool IsValidImpl(const struct ConditionContext& context) const = 0;

public:

    bool intendedResult;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_ConditionAnd : public DS_Condition
{
public:

    DS_ConditionAnd();
    virtual ~DS_ConditionAnd();

public:

    virtual bool IsValidImpl(const struct ConditionContext& context) const override;

public:

    std::vector<const DS_Condition*> conditions;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_ConditionPlayerLevel : public DS_Condition
{
public:

    DS_ConditionPlayerLevel();
    virtual ~DS_ConditionPlayerLevel();

public:

    virtual bool IsValidImpl(const struct ConditionContext& context) const override;

public:

    int minLevel;
    int maxLevel;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Player : public gugu::DatasheetObject
{
public:

    DS_Player();
    virtual ~DS_Player();

public:

    std::string name;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_GameSave : public gugu::DatasaveObject
{
public:

    DS_GameSave();
    virtual ~DS_GameSave();

public:

    DS_PlayerSave* player;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
    virtual void SerializeMembers(gugu::DataSaveContext& context) const override;

    virtual const std::string& GetDataInstanceType() const override;
};

////////////////////////////////////////////////////////////////
class DS_PlayerSave : public gugu::DatasaveObject
{
public:

    DS_PlayerSave();
    virtual ~DS_PlayerSave();

public:

    int money;
    std::vector<DS_ItemSave*> inventory;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
    virtual void SerializeMembers(gugu::DataSaveContext& context) const override;

    virtual const std::string& GetDataInstanceType() const override;
};

////////////////////////////////////////////////////////////////
class DS_ItemSave : public gugu::DatasaveObject
{
public:

    DS_ItemSave();
    virtual ~DS_ItemSave();

public:

    const DS_Item* item;
    int quantity;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
    virtual void SerializeMembers(gugu::DataSaveContext& context) const override;

    virtual const std::string& GetDataInstanceType() const override;
};

////////////////////////////////////////////////////////////////
void DataBinding_Register();

////////////////////////////////////////////////////////////////
gugu::DataObject* DataBinding_InstanciateDataObject(std::string_view classType);

} // namespace demoproject
