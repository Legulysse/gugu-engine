////////////////////////////////////////////////////////////////
// Includes

#include "Test/Condition/Condition.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

class ConditionSpecA : public ConditionBaseA
{
protected:

    virtual bool IsValidImpl(const ContextA& context) const override
    {
        return true;
    }
};

//----------------------------------------------

struct ContextB
{
    int characterLevel = 0;
};

using ConditionB = ConditionBaseB<ContextB>;
using ConditionGroupB = ConditionGroupBaseB<ContextB>;

class ConditionSpecB : public ConditionB
{
public:

    int m_minLevel = 0;

protected:

    virtual bool IsValidImpl(const ContextB& context) const override
    {
        return context.characterLevel >= m_minLevel;
    }
};

//----------------------------------------------

class ContextC : public ConditionContextC
{
public:

    virtual ~ContextC() {}

    int characterLevel = 0;
};

class ConditionSpecC : public ConditionBaseC
{
public:

    int m_minLevel = 0;

protected:

    virtual bool IsValidImpl(const ConditionContextC* context) const override
    {
        const ContextC* contextCast = (const ContextC*)context;
        return contextCast->characterLevel >= m_minLevel;
    }
};

//----------------------------------------------

void RunTestCondition();
    
}   //namespace test
