#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Data/DataBindingUtility.h"
#include "Gugu/Data/DatasheetObject.h"
#include "Gugu/Data/DatasaveObject.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace tests {

////////////////////////////////////////////////////////////////
class DS_Entity;
class DS_Character;
class DS_Faction;
class DS_Item;
class DS_Effect;
class DS_EffectGroup;
class DS_EffectProjectile;
class DS_EffectArea;
class DS_EffectDamage;
class DS_EffectHeal;
class DS_Condition;
class DS_ConditionAnd;
class DS_ConditionPlayerLevel;
class DS_ConditionCheckFlag;
class DS_Action;
class DS_ActionList;
class DS_ActionIfCondition;
class DS_ActionSetFlag;
class DS_GameSave;
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
class DS_Entity : public gugu::DatasheetObject
{
public:

    DS_Entity();
    virtual ~DS_Entity();

public:

    std::string name;
    int health;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Character : public DS_Entity
{
public:

    DS_Character();
    virtual ~DS_Character();

public:

    int stamina;
    float speed;
    EWeaponType::Type weapon;
    const DS_Condition* unlocked;
    const DS_Action* actionOnDeath;
    const DS_Effect* attackSkill;
    const DS_Effect* supportSkill;

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

    std::string name;
    const DS_Character* leader;

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
    gugu::Vector2i size;
    gugu::Vector2f scale;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Effect : public gugu::DatasheetObject
{
public:

    DS_Effect();
    virtual ~DS_Effect();

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_EffectGroup : public DS_Effect
{
public:

    DS_EffectGroup();
    virtual ~DS_EffectGroup();

public:

    std::vector<const DS_Effect*> effects;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_EffectProjectile : public DS_Effect
{
public:

    DS_EffectProjectile();
    virtual ~DS_EffectProjectile();

public:

    float speed;
    float lifetime;
    const DS_Effect* hitEffect;
    const DS_Effect* endEffect;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_EffectArea : public DS_Effect
{
public:

    DS_EffectArea();
    virtual ~DS_EffectArea();

public:

    int radius;
    float tick;
    float duration;
    const DS_Effect* areaTickEffect;
    const DS_Effect* areaEnterEffect;
    const DS_Effect* areaExitEffect;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_EffectDamage : public DS_Effect
{
public:

    DS_EffectDamage();
    virtual ~DS_EffectDamage();

public:

    int damage;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_EffectHeal : public DS_Effect
{
public:

    DS_EffectHeal();
    virtual ~DS_EffectHeal();

public:

    int heal;

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
class DS_ConditionCheckFlag : public DS_Condition
{
public:

    DS_ConditionCheckFlag();
    virtual ~DS_ConditionCheckFlag();

public:

    virtual bool IsValidImpl(const struct ConditionContext& context) const override;

public:

    std::string flag;
    bool raised;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Action : public gugu::DatasheetObject
{
public:

    DS_Action();
    virtual ~DS_Action();

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_ActionList : public DS_Action
{
public:

    DS_ActionList();
    virtual ~DS_ActionList();

public:

    std::vector<const DS_Action*> actions;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_ActionIfCondition : public DS_Action
{
public:

    DS_ActionIfCondition();
    virtual ~DS_ActionIfCondition();

public:

    const DS_Condition* condition;
    const DS_Action* actionIfTrue;
    const DS_Action* actionIfFalse;

protected:

    virtual void ParseMembers(gugu::DataParseContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_ActionSetFlag : public DS_Action
{
public:

    DS_ActionSetFlag();
    virtual ~DS_ActionSetFlag();

public:

    std::string flag;
    bool raised;

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

    bool readTutorial;
    int score;
    float walkedDistance;
    std::string name;
    EWeaponType::Type singleWeapon;
    gugu::Vector2i gridPosition;
    gugu::Vector2f position;
    const DS_Character* emptyCharacter;
    const DS_Character* singleCharacter;
    DS_ItemSave* emptyItem;
    DS_ItemSave* singleItem;
    std::vector<bool> multipleBools;
    std::vector<int> multipleScores;
    std::vector<float> multipleFloats;
    std::vector<std::string> multipleNames;
    std::vector<EWeaponType::Type> multipleWeapons;
    std::vector<gugu::Vector2i> multipleGridPositions;
    std::vector<gugu::Vector2f> multiplePositions;
    std::vector<const DS_Character*> multipleCharacters;
    std::vector<DS_ItemSave*> multipleItems;

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

} // namespace tests
