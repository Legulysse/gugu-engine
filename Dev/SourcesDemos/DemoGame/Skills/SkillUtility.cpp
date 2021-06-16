////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Skills/SkillUtility.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Game/Game.h"
#include "Actors/Characters/Character.h"
#include "Actors/Characters/CharacterHero.h"
#include "Actors/Projectiles/Projectile.h"

#include "DatasheetBinding.h"

#include "Gugu/World/Level.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

namespace SkillUtility
{
    void UseSkill(const SkillContext& skillContext)
    {
        ApplySkillEffectList(skillContext, skillContext.skill->effects, nullptr);
    }

    void ApplySkillEffectList(const SkillContext& skillContext, std::vector<DS_Effect*> effects, Character* affected, const Vector2f& affectedPosition)
    {
        for (DS_Effect* effect : effects)
        {
            ApplySkillEffect(skillContext, effect, affected, affectedPosition);
        }
    }

    void ApplySkillEffect(const SkillContext& skillContext, DS_Effect* effect, Character* affectedCharacter, const Vector2f& affectedPosition)
    {
        // TODO: maybe find a more elegant way to retrieve the affectedPosition (avoid the implicit parameter).
        // TODO: affected character should be a list, to ensure we only call this code once per effect application (and avoid calling a caster effect for each affected characters in an area).
        Character* center = nullptr;
        if (effect->center == EEffectCenter::Caster && skillContext.caster)
        {
            center = skillContext.caster;
        }
        else if (effect->center == EEffectCenter::Target && skillContext.target)
        {
            center = skillContext.target;
        }
        else if (effect->center == EEffectCenter::Affected && affectedCharacter)
        {
            center = affectedCharacter;
        }

        if (dynamic_cast<DS_EffectProjectile*>(effect) != nullptr)
        {
            DS_EffectProjectile* effectProjectile = (DS_EffectProjectile*)effect;

            Projectile* newProjectile = new Projectile();
            skillContext.level->AddActor(newProjectile);

            newProjectile->InitProjectile(skillContext, effectProjectile, skillContext.caster->GetPosition(), skillContext.aim);
        }
        else if (dynamic_cast<DS_EffectArea*>(effect) != nullptr)
        {
            DS_EffectArea* effectArea = (DS_EffectArea*)effect;

            std::vector<Character*> charactersInArea;
            GetGame()->GetCharactersInRange(charactersInArea, affectedPosition, effectArea->maxRadius);

            if (effectArea->excludeCaster)
            {
                StdVectorRemove(charactersInArea, skillContext.caster);
            }

            if (effectArea->excludeTarget)
            {
                StdVectorRemove(charactersInArea, skillContext.target);
            }

            // TODO: replace this loop, send the characters list instead.
            for (Character* character : charactersInArea)
            {
                ApplySkillEffectList(skillContext, effectArea->effectsOnHit, character);
            }
        }
        else if (dynamic_cast<DS_EffectDamage*>(effect) != nullptr)
        {
            DS_EffectDamage* effectDamage = (DS_EffectDamage*)effect;

            if (center)
            {
                center->ReceiveDamage(skillContext.caster, effectDamage->damage);
            }
        }
        else if (dynamic_cast<DS_EffectHeal*>(effect) != nullptr)
        {
            DS_EffectHeal* effectHeal = (DS_EffectHeal*)effect;

            if (center)
            {
                center->ReceiveHeal(effectHeal->heal);
            }
        }
    }
}

}   //namespace demoproject
