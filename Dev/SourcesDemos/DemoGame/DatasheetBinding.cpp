////////////////////////////////////////////////////////////////
// Header

#include "DatasheetBinding.h"

////////////////////////////////////////////////////////////////
// Includes

#include <Gugu/Manager/ManagerResources.h>
#include <Gugu/Utility/System.h>

namespace demoproject {


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
    gugu::Datasheet::ParseMembers(context);

    ReadString(context, "imageset", imageSet);
    ReadString(context, "animset", animSet);
}

////////////////////////////////////////////////////////////////
DS_Character::DS_Character()
{
    name = "CHARACTER_NAME";
    health = 100;
    speed = 100;
    sprite = nullptr;
}

DS_Character::~DS_Character()
{
    SafeDelete(sprite);
}

void DS_Character::ParseMembers(gugu::DatasheetParserContext& context)
{
    gugu::Datasheet::ParseMembers(context);

    ReadString(context, "name", name);
    ReadFloat(context, "health", health);
    ReadFloat(context, "speed", speed);
    ReadInstance(context, "sprite", "spriteInfo", sprite);
}

////////////////////////////////////////////////////////////////
DS_Hero::DS_Hero()
{
    stamina = 100;
    staminaRecovery = 100;
    staminaRecoveryDelay = 100;
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
DS_Condition::DS_Condition()
{
}

DS_Condition::~DS_Condition()
{
}

void DS_Condition::ParseMembers(gugu::DatasheetParserContext& context)
{
    gugu::Datasheet::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
DS_Skill::DS_Skill()
{
    name = "";
}

DS_Skill::~DS_Skill()
{
}

void DS_Skill::ParseMembers(gugu::DatasheetParserContext& context)
{
    gugu::Datasheet::ParseMembers(context);

    ReadString(context, "name", name);
}

////////////////////////////////////////////////////////////////
DS_Effect::DS_Effect()
{
}

DS_Effect::~DS_Effect()
{
}

void DS_Effect::ParseMembers(gugu::DatasheetParserContext& context)
{
    gugu::Datasheet::ParseMembers(context);

}

////////////////////////////////////////////////////////////////
void DatasheetBinding_Register()
{
    gugu::GetResources()->RegisterDatasheetFactory(new gugu::DelegateStatic1P<const std::string&, gugu::Datasheet*>(&DatasheetBinding_InstanciateDatasheet));
}

////////////////////////////////////////////////////////////////
gugu::Datasheet* DatasheetBinding_InstanciateDatasheet(const std::string& classType)
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
    if (classType == "condition")
    {
        return new DS_Condition;
    }
    if (classType == "skill")
    {
        return new DS_Skill;
    }
    if (classType == "effect")
    {
        return new DS_Effect;
    }
    return nullptr;
}

} // namespace demoproject
