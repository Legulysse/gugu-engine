#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Datasheet.h"

#include <vector>

namespace demoproject {

////////////////////////////////////////////////////////////////
// Forward Declarations

class DS_SpriteInfo;
class DS_Entity;
class DS_General;
class DS_Faction;
class DS_Troop;
class DS_Restriction;
class DS_RestrictionFaction;
class DS_RestrictionTroop;
class DS_Skill;
class DS_Condition;
class DS_ConditionAnd;
class DS_ConditionPlayerLevel;

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

    const gugu::DatasheetEnum* GetDatasheetEnum();
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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
    std::vector< std::string > m_names;
    std::vector< int > m_stats;
    std::vector< const DS_Faction* > m_factions;
    std::vector< const DS_SpriteInfo* > m_sprites;
    EWeaponType::Type m_weapon;
    std::vector< EWeaponType::Type > m_availableWeapons;
    const DS_Condition* playableCondition;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Restriction : public gugu::DatasheetObject
{
public:

    DS_Restriction();
    virtual ~DS_Restriction();

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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
    std::vector< const DS_Restriction* > m_restrictions;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    bool not;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    std::vector< const DS_Condition* > conditions;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
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

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
void DatasheetBinding_Register();

////////////////////////////////////////////////////////////////
gugu::DatasheetObject* DatasheetBinding_InstanciateDatasheetObject(const std::string& classType);

} // namespace demoproject
