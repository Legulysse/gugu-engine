////////////////////////////////////////////////////////////////
// Header

#include "DataBinding.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/System/SystemUtility.h"

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

    gugu::DataBinding::ReadString(context, "imageset", imageSet);
    gugu::DataBinding::ReadString(context, "animset", animSet);
}

////////////////////////////////////////////////////////////////
DS_Character::DS_Character()
{
    name = "CHARACTER_NAME";
    health = 50;
    speed = 50;
    sprite = nullptr;
}

DS_Character::~DS_Character()
{
    SafeDelete(sprite);
}

void DS_Character::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", name);
    gugu::DataBinding::ReadFloat(context, "health", health);
    gugu::DataBinding::ReadFloat(context, "speed", speed);
    gugu::DataBinding::ReadDatasheetInstance(context, "sprite", "spriteInfo", sprite);
}

////////////////////////////////////////////////////////////////
DS_Hero::DS_Hero()
{
    stamina = 50;
    staminaRecovery = 50;
    staminaRecoveryDelay = 50;
    mana = 50;
    manaRecovery = 50;
}

DS_Hero::~DS_Hero()
{
}

void DS_Hero::ParseMembers(gugu::DataParseContext& context)
{
    DS_Character::ParseMembers(context);

    gugu::DataBinding::ReadFloat(context, "stamina", stamina);
    gugu::DataBinding::ReadFloat(context, "staminaRecovery", staminaRecovery);
    gugu::DataBinding::ReadFloat(context, "staminaRecoveryDelay", staminaRecoveryDelay);
    gugu::DataBinding::ReadFloat(context, "mana", mana);
    gugu::DataBinding::ReadFloat(context, "manaRecovery", manaRecovery);
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
    staminaCost = -1;
    manaCost = -1;
    useAttackSpeed = false;
    cooldown = -1;
}

DS_Skill::~DS_Skill()
{
    ClearStdVector(effects);
}

void DS_Skill::ParseMembers(gugu::DataParseContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    gugu::DataBinding::ReadString(context, "name", name);
    gugu::DataBinding::ReadFloat(context, "staminaCost", staminaCost);
    gugu::DataBinding::ReadFloat(context, "manaCost", manaCost);
    gugu::DataBinding::ReadBool(context, "useAttackSpeed", useAttackSpeed);
    gugu::DataBinding::ReadFloat(context, "cooldown", cooldown);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "effects", "effect", effects);
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

    gugu::DataBinding::ReadEnum(context, "center", "effectCenter", center);
}

////////////////////////////////////////////////////////////////
DS_EffectArea::DS_EffectArea()
{
    excludeCaster = false;
    excludeTarget = false;
    maxRadius = 50;
    maxAngleFromDirection = -1;
}

DS_EffectArea::~DS_EffectArea()
{
    ClearStdVector(effectsOnHit);
}

void DS_EffectArea::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::DataBinding::ReadBool(context, "excludeCaster", excludeCaster);
    gugu::DataBinding::ReadBool(context, "excludeTarget", excludeTarget);
    gugu::DataBinding::ReadFloat(context, "maxRadius", maxRadius);
    gugu::DataBinding::ReadFloat(context, "maxAngleFromDirection", maxAngleFromDirection);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "effectsOnHit", "effect", effectsOnHit);
}

////////////////////////////////////////////////////////////////
DS_EffectProjectile::DS_EffectProjectile()
{
    aim = EProjectileAim::Direction;
    speed = 500;
    lifetime = 2;
    maximumHits = 1;
}

DS_EffectProjectile::~DS_EffectProjectile()
{
    ClearStdVector(effectsOnHit);
    ClearStdVector(effectsOnDestination);
}

void DS_EffectProjectile::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::DataBinding::ReadEnum(context, "aim", "projectileAim", aim);
    gugu::DataBinding::ReadFloat(context, "speed", speed);
    gugu::DataBinding::ReadFloat(context, "lifetime", lifetime);
    gugu::DataBinding::ReadInt(context, "maximumHits", maximumHits);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "effectsOnHit", "effect", effectsOnHit);
    gugu::DataBinding::ReadDatasheetInstanceArray(context, "effectsOnDestination", "effect", effectsOnDestination);
}

////////////////////////////////////////////////////////////////
DS_EffectDamage::DS_EffectDamage()
{
    damage = 10;
}

DS_EffectDamage::~DS_EffectDamage()
{
}

void DS_EffectDamage::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::DataBinding::ReadFloat(context, "damage", damage);
}

////////////////////////////////////////////////////////////////
DS_EffectHeal::DS_EffectHeal()
{
    heal = 10;
}

DS_EffectHeal::~DS_EffectHeal()
{
}

void DS_EffectHeal::ParseMembers(gugu::DataParseContext& context)
{
    DS_Effect::ParseMembers(context);

    gugu::DataBinding::ReadFloat(context, "heal", heal);
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
