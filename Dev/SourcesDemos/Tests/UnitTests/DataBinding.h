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
class DS_Condition;
class DS_ConditionAnd;
class DS_ConditionPlayerLevel;
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
    const DS_Character* emptyCharacter;
    const DS_Character* singleCharacter;
    DS_ItemSave* emptyItem;
    DS_ItemSave* singleItem;
    std::vector<bool> multipleBools;
    std::vector<int> multipleScores;
    std::vector<float> multipleFloats;
    std::vector<std::string> multipleNames;
    std::vector<EWeaponType::Type> multipleWeapons;
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
