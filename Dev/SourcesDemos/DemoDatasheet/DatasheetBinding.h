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

    gugu::DatasheetEnum* GetDatasheetEnum();
    void GetEnumValues(std::vector<EWeaponType::Type>& enumValues);
    size_t GetSize();

    void Register();
}

////////////////////////////////////////////////////////////////
class DS_SpriteInfo : public gugu::Datasheet
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
class DS_Entity : public gugu::Datasheet
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
    DS_Faction* m_faction;
    DS_SpriteInfo* m_sprite;
    DS_SpriteInfo* m_sprite2;
    std::vector< std::string > m_names;
    std::vector< int > m_stats;
    std::vector< DS_Faction* > m_factions;
    std::vector< DS_SpriteInfo* > m_sprites;
    EWeaponType::Type m_weapon;
    std::vector< EWeaponType::Type > m_availableWeapons;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Faction : public gugu::Datasheet
{
public:

    DS_Faction();
    virtual ~DS_Faction();

public:

    std::string m_name;
    DS_Entity* m_leader;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Troop : public gugu::Datasheet
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
class DS_Restriction : public gugu::Datasheet
{
public:

    DS_Restriction();
    virtual ~DS_Restriction();

public:

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

    DS_Faction* m_faction;

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

    DS_Troop* m_troop;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Skill : public gugu::Datasheet
{
public:

    DS_Skill();
    virtual ~DS_Skill();

public:

    std::string m_name;
    DS_Restriction* m_emptyRestriction;
    DS_Restriction* m_singleRestrictionBase;
    DS_Restriction* m_singleRestrictionFaction;
    std::vector< DS_Restriction* > m_restrictions;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
void DatasheetBinding_Register();

////////////////////////////////////////////////////////////////
gugu::Datasheet* DatasheetBinding_InstanciateDatasheet(const std::string& classType);

} // namespace demoproject
