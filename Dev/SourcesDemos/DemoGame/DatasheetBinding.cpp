////////////////////////////////////////////////////////////////
// Header

#include "DatasheetBinding.h"

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
        gugu::DatasheetEnum* datasheetEnum = new gugu::DatasheetEnum;
        datasheetEnum->values.reserve(3);
        datasheetEnum->values.push_back("Caster");
        datasheetEnum->values.push_back("Target");
        datasheetEnum->values.push_back("Affected");

        gugu::GetResources()->RegisterDatasheetEnum("effectCenter", datasheetEnum);
    }

    const gugu::DatasheetEnum* GetDatasheetEnum()
    {
        return gugu::GetResources()->GetDatasheetEnum("effectCenter");
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
        const gugu::DatasheetEnum* datasheetEnum = gugu::GetResources()->GetDatasheetEnum("effectCenter");
        if (datasheetEnum)
            return datasheetEnum->values.size();
        return 0;
    }
}
////////////////////////////////////////////////////////////////
namespace EProjectileAim
{
    void Register()
    {
        gugu::DatasheetEnum* datasheetEnum = new gugu::DatasheetEnum;
        datasheetEnum->values.reserve(2);
        datasheetEnum->values.push_back("Direction");
        datasheetEnum->values.push_back("Cursor");

        gugu::GetResources()->RegisterDatasheetEnum("projectileAim", datasheetEnum);
    }

    const gugu::DatasheetEnum* GetDatasheetEnum()
    {
        return gugu::GetResources()->GetDatasheetEnum("projectileAim");
    }

    void GetEnumValues(std::vector<EProjectileAim::Type>& enumValues)
    {
        enumValues.reserve(2);
        enumValues.push_back(EProjectileAim::Direction);
        enumValues.push_back(EProjectileAim::Cursor);
    }

    size_t GetSize()
    {
        const gugu::DatasheetEnum* datasheetEnum = gugu::GetResources()->GetDatasheetEnum("projectileAim");
        if (datasheetEnum)
            return datasheetEnum->values.size();
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

void DS_SpriteInfo::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    ReadString(context, "imageset", imageSet);
    ReadString(context, "animset", animSet);
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

void DS_Character::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    ReadString(context, "name", name);
    ReadFloat(context, "health", health);
    ReadFloat(context, "speed", speed);
    ReadInstance(context, "sprite", "spriteInfo", sprite);
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

void DS_Hero::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Character::ParseMembers(context);

    ReadFloat(context, "stamina", stamina);
    ReadFloat(context, "staminaRecovery", staminaRecovery);
    ReadFloat(context, "staminaRecoveryDelay", staminaRecoveryDelay);
    ReadFloat(context, "mana", mana);
    ReadFloat(context, "manaRecovery", manaRecovery);
}

////////////////////////////////////////////////////////////////
DS_Enemy::DS_Enemy()
{
}

DS_Enemy::~DS_Enemy()
{
}

void DS_Enemy::ParseMembers(gugu::DatasheetParserContext& context)
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

void DS_Skill::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    ReadString(context, "name", name);
    ReadFloat(context, "staminaCost", staminaCost);
    ReadFloat(context, "manaCost", manaCost);
    ReadBool(context, "useAttackSpeed", useAttackSpeed);
    ReadFloat(context, "cooldown", cooldown);
    ReadArrayInstance(context, "effects", "effect", effects);
}

////////////////////////////////////////////////////////////////
DS_Effect::DS_Effect()
{
    center = EEffectCenter::Target;
}

DS_Effect::~DS_Effect()
{
}

void DS_Effect::ParseMembers(gugu::DatasheetParserContext& context)
{
    //gugu::DatasheetObject::ParseMembers(context);

    ReadEnum(context, "center", "effectCenter", center);
}

////////////////////////////////////////////////////////////////
DS_EffectArea::DS_EffectArea()
{
    excludeCaster = false;
    excludeTarget = false;
    maxRadius = 50;
    angleFromDirection = -1;
}

DS_EffectArea::~DS_EffectArea()
{
    ClearStdVector(effectsOnHit);
}

void DS_EffectArea::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Effect::ParseMembers(context);

    ReadBool(context, "excludeCaster", excludeCaster);
    ReadBool(context, "excludeTarget", excludeTarget);
    ReadFloat(context, "maxRadius", maxRadius);
    ReadFloat(context, "maxAngleFromDirection", angleFromDirection);
    ReadArrayInstance(context, "effectsOnHit", "effect", effectsOnHit);
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

void DS_EffectProjectile::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Effect::ParseMembers(context);

    ReadEnum(context, "aim", "projectileAim", aim);
    ReadFloat(context, "speed", speed);
    ReadFloat(context, "lifetime", lifetime);
    ReadInt(context, "maximumHits", maximumHits);
    ReadArrayInstance(context, "effectsOnHit", "effect", effectsOnHit);
    ReadArrayInstance(context, "effectsOnDestination", "effect", effectsOnDestination);
}

////////////////////////////////////////////////////////////////
DS_EffectDamage::DS_EffectDamage()
{
    damage = 10;
}

DS_EffectDamage::~DS_EffectDamage()
{
}

void DS_EffectDamage::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Effect::ParseMembers(context);

    ReadFloat(context, "damage", damage);
}

////////////////////////////////////////////////////////////////
DS_EffectHeal::DS_EffectHeal()
{
    heal = 10;
}

DS_EffectHeal::~DS_EffectHeal()
{
}

void DS_EffectHeal::ParseMembers(gugu::DatasheetParserContext& context)
{
    DS_Effect::ParseMembers(context);

    ReadFloat(context, "heal", heal);
}

////////////////////////////////////////////////////////////////
void DatasheetBinding_Register()
{
    EEffectCenter::Register();
    EProjectileAim::Register();

    gugu::GetResources()->RegisterDatasheetObjectFactory(DatasheetBinding_InstanciateDatasheetObject);
}

////////////////////////////////////////////////////////////////
gugu::DatasheetObject* DatasheetBinding_InstanciateDatasheetObject(const std::string& classType)
{
    if (classType == "spriteInfo")
    {
        return new DS_SpriteInfo;
    }
    if (classType == "character")
    {
        return new DS_Character;
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
    if (classType == "effect")
    {
        return new DS_Effect;
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
