#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Math/Vector2.h"

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
    const DS_Skill* skill;
    Character* caster;
    Character* target;
    gugu::Vector2f aim;
    gugu::Level* level;
};

namespace SkillUtility
{
    void UseSkill(const SkillContext& skillContext);
    void ApplySkillEffectList(const SkillContext& skillContext, std::vector<const DS_Effect*> effects, Character* affected, const gugu::Vector2f& affectedPosition = gugu::Vector2f());
    void ApplySkillEffect(const SkillContext& skillContext, const DS_Effect* effect, Character* affected, const gugu::Vector2f& affectedPosition = gugu::Vector2f());
}

}   //namespace demoproject
