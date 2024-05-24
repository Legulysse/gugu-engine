////////////////////////////////////////////////////////////////
// Includes

#include <vector>

////////////////////////////////////////////////////////////////
// File Declarations

namespace test {

struct ContextA
{
};

class ConditionBaseA
{
public:

    bool IsValid(const ContextA& context) const
    {
        return IsValidImpl(context);
    }

protected:

    virtual bool IsValidImpl(const ContextA& context) const = 0;
};

//----------------------------------------------

template<typename TContext>
class ConditionBaseB
{
public:

    bool m_not = false;

    bool IsValid(const TContext& context) const
    {
        return IsValidImpl(context) != m_not;
    }

protected:

    virtual bool IsValidImpl(const TContext& context) const = 0;
};

template<typename TContext>
class ConditionGroupBaseB : public ConditionBaseB<TContext>
{
public:

    std::vector<ConditionBaseB<TContext>*> m_conditions;

protected:

    virtual bool IsValidImpl(const TContext& context) const override
    {
        return true;
    }
};

//----------------------------------------------

class ConditionContextC
{
public:

    virtual ~ConditionContextC() {}
};

class ConditionBaseC
{
public:

    bool IsValid(const ConditionContextC* context) const
    {
        return IsValidImpl(context);
    }

protected:

    virtual bool IsValidImpl(const ConditionContextC* context) const = 0;
};

}   //namespace test
