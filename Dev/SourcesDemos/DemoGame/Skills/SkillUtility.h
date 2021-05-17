#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Utility/Vector2.h"

#include <vector>

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace demoproject
{
    class DS_Skill;
    class DS_Effect;
    class Character;
}

namespace gugu
{
    class Level;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {

struct SkillContext
{
    DS_Skill* skill;
    Character* caster;
    Character* target;
    sf::Vector2f aim;
    gugu::Level* level;
};

namespace SkillUtility
{
    void UseSkill(const SkillContext& skillContext);
    void ApplySkillEffectList(const SkillContext& skillContext, std::vector<DS_Effect*> effects, Character* affected, const sf::Vector2f& affectedPosition = sf::Vector2f());
    void ApplySkillEffect(const SkillContext& skillContext, DS_Effect* effect, Character* affected, const sf::Vector2f& affectedPosition = sf::Vector2f());
}

}   //namespace demoproject
