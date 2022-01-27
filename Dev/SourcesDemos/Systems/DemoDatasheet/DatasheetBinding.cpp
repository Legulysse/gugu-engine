////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "DatasheetBinding.h"

////////////////////////////////////////////////////////////////
// Includes

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {

bool DS_Condition::IsValid(const ConditionContext& context) const
{
    return IsValidImpl(context) != not;
}

bool DS_ConditionAnd::IsValidImpl(const struct ConditionContext& context) const
{
    for (const DS_Condition* condition : conditions)
    {
        if (!condition->IsValid(context))
        {
            return false;
        }
    }

    return true;
}

bool DS_ConditionPlayerLevel::IsValidImpl(const struct ConditionContext& context) const
{
    return (minLevel < 0 || context.playerLevel >= minLevel)
        && (maxLevel < 0 || context.playerLevel <= maxLevel);
}

} // namespace demoproject
