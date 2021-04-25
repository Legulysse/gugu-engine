#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Resources/Datasheet.h"

#include <vector>

namespace demoproject {

////////////////////////////////////////////////////////////////
// Forward Declarations

class DS_Character;
class DS_Hero;
class DS_Enemy;
class DS_Condition;
class DS_Skill;
class DS_Effect;


////////////////////////////////////////////////////////////////
class DS_Character : public gugu::Datasheet
{
public:

    DS_Character();
    virtual ~DS_Character();

public:

    std::string name;
    float health;
    float speed;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Hero : public DS_Character
{
public:

    DS_Hero();
    virtual ~DS_Hero();

public:

    float stamina;
    float staminaRecovery;
    float staminaRecoveryDelay;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Enemy : public DS_Character
{
public:

    DS_Enemy();
    virtual ~DS_Enemy();

public:

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Condition : public gugu::Datasheet
{
public:

    DS_Condition();
    virtual ~DS_Condition();

public:

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

    std::string name;

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
class DS_Effect : public gugu::Datasheet
{
public:

    DS_Effect();
    virtual ~DS_Effect();

public:

protected:

    virtual void ParseMembers (gugu::DatasheetParserContext& context) override;
};

////////////////////////////////////////////////////////////////
void DatasheetBinding_Register();

////////////////////////////////////////////////////////////////
gugu::Datasheet* DatasheetBinding_InstanciateDatasheet(const std::string& classType);

} // namespace demoproject
