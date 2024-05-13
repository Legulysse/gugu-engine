////////////////////////////////////////////////////////////////
// Header

#include "DataBinding.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

////////////////////////////////////////////////////////////////
namespace EEffectCenter
{
    void Register()
    {
        gugu::DataEnumInfos* enumInfos = new gugu::DataEnumInfos;
        enumInfos->values.reserve(3);
        enumInfos->values.push_back("Caster");
        enumInfos->values.push_back("Target");
        enumInfos->values.push_back("Affected");

        gugu::GetResources()->RegisterDataEnumInfos("effectCenter", enumInfos);
    }

    const gugu::DataEnumInfos* GetDataEnumInfos()
    {
        return gugu::GetResources()->GetDataEnumInfos("effectCenter");
    }

    void GetEnumValues(std::vector<EEffectCenter::Type>& enumValues)
    {
        enumValues.reserve(3);
        enumValues.push_back(EEffectCenter::Caster);
        enumValues.push_back(EEffectCenter::Target);
        enumValues.push_back(EEffectCenter::Affected);
    }

    size_t GetSize()
    {
        const gugu::DataEnumInfos* enumInfos = gugu::GetResources()->GetDataEnumInfos("effectCenter");
        if (enumInfos)
            return enumInfos->values.size();
        return 0;
    }
}

////////////////////////////////////////////////////////////////
namespace EProjectileAim
{
    void Register()
    {
        gugu::DataEnumInfos* enumInfos = new gugu::DataEnumInfos;
        enumInfos->values.reserve(2);
        enumInfos->values.push_back("Direction");
        enumInfos->values.push_back("Cursor");

        gugu::GetResources()->RegisterDataEnumInfos("projectileAim", enumInfos);
    }

    const gugu::DataEnumInfos* GetDataEnumInfos()
    {
        return gugu::GetResources()->GetDataEnumInfos("projectileAim");
    }

    void GetEnumValues(std::vector<EProjectileAim::Type>& enumValues)
    {
        enumValues.reserve(2);
        enumValues.push_back(EProjectileAim::Direction);
        enumValues.push_back(EProjectileAim::Cursor);
    }

    size_t GetSize()
    {
        const gugu::DataEnumInfos* enumInfos = gugu::GetResources()->GetDataEnumInfos("projectileAim");
        if (enumInfos)
            return enumInfos->values.size();
        return 0;
    }
}

////////////////////////////////////////////////////////////////
DS_SpriteInfo::DS_SpriteInfo()
{
    imageSet = "";
    animSet = "";
}

DS_SpriteInfo::~DS_SpriteInfo()
{
}

void DS_SpriteInfo::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "imageset", imageSet);
    gugu::binding::ReadString(context, "animset", animSet);
}

////////////////////////////////////////////////////////////////
DS_Character::DS_Character()
{
    name = "CHARACTER_NAME";
    health = 50.f;
    speed = 50.f;
    sprite = nullptr;
}

DS_Character::~DS_Character()
{
    sprite = nullptr;
}

void DS_Character::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadFloat(context, "health", health);
    gugu::binding::ReadFloat(context, "speed", speed);
    gugu::binding::ReadDatasheetInstance(context, "sprite", "spriteInfo", sprite);
}

////////////////////////////////////////////////////////////////
DS_Hero::DS_Hero()
{
    stamina = 50.f;
    staminaRecovery = 50.f;
    staminaRecoveryDelay = 50.f;
    mana = 50.f;
    manaRecovery = 50.f;
}

DS_Hero::~DS_Hero()
{
}

void DS_Hero::ParseMembers(gugu::DataParseContext& context)
{
    DS_Character::ParseMembers(context);

    gugu::binding::ReadFloat(context, "stamina", stamina);
    gugu::binding::ReadFloat(context, "staminaRecovery", staminaRecovery);
    gugu::binding::ReadFloat(context, "staminaRecoveryDelay", staminaRecoveryDelay);
    gugu::binding::ReadFloat(context, "mana", mana);
    gugu::binding::ReadFloat(context, "manaRecovery", manaRecovery);
}

////////////////////////////////////////////////////////////////
DS_Enemy::DS_Enemy()
{
}

DS_Enemy::~DS_Enemy()
{
}

void DS_Enemy::ParseMembers(gugu::DataParseContext& context)
{
    DS_Character::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
DS_Skill::DS_Skill()
{
    name = "";
    staminaCost = -1.f;
    manaCost = -1.f;
    useAttackSpeed = false;
    cooldown = -1.f;
}

DS_Skill::~DS_Skill()
{
    effects.clear();
}

void DS_Skill::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadString(context, "name", name);
    gugu::binding::ReadFloat(context, "staminaCost", staminaCost);
    gugu::binding::ReadFloat(context, "manaCost", manaCost);
    gugu::binding::ReadBool(context, "useAttackSpeed", useAttackSpeed);
    gugu::binding::ReadFloat(context, "cooldown", cooldown);
    gugu::binding::ReadDatasheetInstanceArray(context, "effects", "effect", effects);
}

////////////////////////////////////////////////////////////////
DS_Effect::DS_Effect()
{
    center = EEffectCenter::Target;
}

DS_Effect::~DS_Effect()
{
}

void DS_Effect::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::binding::ReadEnum(context, "center", "effectCenter", center);
}

////////////////////////////////////////////////////////////////
DS_EffectArea::DS_EffectArea()
{
    excludeCaster = false;
    excludeTarget = false;
    maxRadius = 50.f;
    maxAngleFromDirection = -1.f;
}

DS_EffectArea::~DS_EffectArea()
{
    effectsOnHit.clear();
}

void DS_EffectArea::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadBool(context, "excludeCaster", excludeCaster);
    gugu::binding::ReadBool(context, "excludeTarget", excludeTarget);
    gugu::binding::ReadFloat(context, "maxRadius", maxRadius);
    gugu::binding::ReadFloat(context, "maxAngleFromDirection", maxAngleFromDirection);
    gugu::binding::ReadDatasheetInstanceArray(context, "effectsOnHit", "effect", effectsOnHit);
}

////////////////////////////////////////////////////////////////
DS_EffectProjectile::DS_EffectProjectile()
{
    aim = EProjectileAim::Direction;
    speed = 500.f;
    lifetime = 2.f;
    maximumHits = 1;
}

DS_EffectProjectile::~DS_EffectProjectile()
{
    effectsOnHit.clear();
    effectsOnDestination.clear();
}

void DS_EffectProjectile::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadEnum(context, "aim", "projectileAim", aim);
    gugu::binding::ReadFloat(context, "speed", speed);
    gugu::binding::ReadFloat(context, "lifetime", lifetime);
    gugu::binding::ReadInt(context, "maximumHits", maximumHits);
    gugu::binding::ReadDatasheetInstanceArray(context, "effectsOnHit", "effect", effectsOnHit);
    gugu::binding::ReadDatasheetInstanceArray(context, "effectsOnDestination", "effect", effectsOnDestination);
}

////////////////////////////////////////////////////////////////
DS_EffectDamage::DS_EffectDamage()
{
    damage = 10.f;
}

DS_EffectDamage::~DS_EffectDamage()
{
}

void DS_EffectDamage::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadFloat(context, "damage", damage);
}

////////////////////////////////////////////////////////////////
DS_EffectHeal::DS_EffectHeal()
{
    heal = 10.f;
}

DS_EffectHeal::~DS_EffectHeal()
{
}

void DS_EffectHeal::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::binding::ReadFloat(context, "heal", heal);
}

////////////////////////////////////////////////////////////////
void DataBinding_Register()
{
    EEffectCenter::Register();
    EProjectileAim::Register();

    gugu::GetResources()->RegisterDataObjectFactory(DataBinding_InstanciateDataObject);
}

////////////////////////////////////////////////////////////////
gugu::DataObject* DataBinding_InstanciateDataObject(std::string_view classType)
{
    if (classType == "spriteInfo")
    {
        return new DS_SpriteInfo;
    }
    if (classType == "hero")
    {
        return new DS_Hero;
    }
    if (classType == "enemy")
    {
        return new DS_Enemy;
    }
    if (classType == "skill")
    {
        return new DS_Skill;
    }
    if (classType == "effectArea")
    {
        return new DS_EffectArea;
    }
    if (classType == "effectProjectile")
    {
        return new DS_EffectProjectile;
    }
    if (classType == "effectDamage")
    {
        return new DS_EffectDamage;
    }
    if (classType == "effectHeal")
    {
        return new DS_EffectHeal;
    }
    return nullptr;
}

} // namespace demoproject
