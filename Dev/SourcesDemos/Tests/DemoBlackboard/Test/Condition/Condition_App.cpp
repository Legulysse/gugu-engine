////////////////////////////////////////////////////////////////
// Header

#include "Test/Condition/Condition_App.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/SystemUtility.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace test {
    
void RunTestCondition()
{
    //----------------------------------------------
    {
        ContextA context;
        ConditionSpecA* condition = new ConditionSpecA;
        bool result = condition->IsValid(context);

        ConditionBaseA* genericCondition = condition;
        result = genericCondition->IsValid(context);

        SafeDelete(condition);
    }
    //----------------------------------------------
    {
        ContextB context;
        context.characterLevel = 5;
        ConditionSpecB* condition = new ConditionSpecB;
        condition->m_not = true;
        condition->m_minLevel = 2;
        bool result = condition->IsValid(context);

        ConditionBaseB<ContextB>* genericCondition = condition;
        result = genericCondition->IsValid(context);

        ConditionGroupB* conditionGroup = new ConditionGroupB;
        result = conditionGroup->IsValid(context);

        SafeDelete(condition);
        SafeDelete(conditionGroup);
    }
    //----------------------------------------------
    {
        ContextC context;
        context.characterLevel = 5;
        ConditionSpecC* condition = new ConditionSpecC;
        condition->m_minLevel = 2;
        bool result = condition->IsValid(&context);

        ConditionBaseC* genericCondition = condition;
        result = genericCondition->IsValid(&context);

        SafeDelete(condition);
    }
}

}   //namespace test
