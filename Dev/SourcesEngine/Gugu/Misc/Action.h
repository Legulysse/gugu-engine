#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {
        
//----------------------------------------------
// Abstract Action

class Action
{
public:

            Action  () {}
    virtual ~Action () {}

    virtual void Call   () = 0;
    void operator()     () { Call(); }
};

//----------------------------------------------
// Class Action

template<typename TClass>
class ActionClass : public Action
{
private:

    typedef void (TClass::*PMethod)(void);
    PMethod     m_classMethod;
    TClass*     m_classObject;

public:

    virtual ~ActionClass () {}

    ActionClass (TClass* _pObject, PMethod _pMethod)
    {
        m_classObject = _pObject;
        m_classMethod = _pMethod;
    }

    virtual void Call() override
    {
        (m_classObject->*m_classMethod)();
    }
};

template<typename TClass, typename TParam1>
class ActionClass1P : public Action
{
private:

    typedef void (TClass::*PMethod)(TParam1);
    PMethod     m_classMethod;
    TClass*     m_classObject;
    TParam1     m_value1;

public:

    virtual ~ActionClass1P () {}

    ActionClass1P (TClass* _pObject, PMethod _pMethod, TParam1 _oValue1)
    {
        m_classObject   = _pObject;
        m_classMethod   = _pMethod;
        m_value1        = _oValue1;
    }

    virtual void Call() override
    {
        return (m_classObject->*m_classMethod)(m_value1);
    }
};

template<typename TClass, typename TParam1, typename TParam2>
class ActionClass2P : public Action
{
private:

    typedef void (TClass::*PMethod)(TParam1, TParam2);
    PMethod     m_classMethod;
    TClass*     m_classObject;
    TParam1     m_value1;
    TParam2     m_value2;

public:

    virtual ~ActionClass2P () {}

    ActionClass2P (TClass* _pObject, PMethod _pMethod, TParam1 _oValue1, TParam2 _oValue2)
    {
        m_classObject   = _pObject;
        m_classMethod   = _pMethod;
        m_value1        = _oValue1;
        m_value2        = _oValue2;
    }

    virtual void Call() override
    {
        (m_classObject->*m_classMethod)(m_value1, m_value2);
    }
};

//----------------------------------------------
// Static Action

class ActionStatic : public Action
{
private:

    typedef void (*PMethod)(void);
    PMethod     m_staticMethod;

public:

    virtual ~ActionStatic () {}

    ActionStatic (PMethod _pMethod)
    {
        m_staticMethod = _pMethod;
    }

    virtual void Call() override
    {
        (m_staticMethod)();
    }
};

template<typename TParam1>
class ActionStatic1P : public Action
{
private:

    typedef void (*PMethod)(TParam1);
    PMethod     m_staticMethod;
    TParam1     m_value1;

public:

    virtual ~ActionStatic1P () {}

    ActionStatic1P (PMethod _pMethod, TParam1 _oValue1)
    {
        m_staticMethod  = _pMethod;
        m_value1    = _oValue1;
    }

    virtual void Call() override
    {
        (m_staticMethod)(m_value1);
    }
};

template<typename TParam1, typename TParam2>
class ActionStatic2P : public Action
{
private:

    typedef void (*PMethod)(TParam1, TParam2);
    PMethod     m_staticMethod;
    TParam1     m_value1;
    TParam2     m_value2;

public:

    virtual ~ActionStatic2P () {}

    ActionStatic2P (PMethod _pMethod, TParam1 _oValue1, TParam2 _oValue2)
    {
        m_staticMethod  = _pMethod;
        m_value1    = _oValue1;
        m_value2    = _oValue2;
    }

    virtual void Call() override
    {
        (m_staticMethod)(m_value1, m_value2);
    }
};

}   // namespace gugu
