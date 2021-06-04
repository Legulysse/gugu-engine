#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

//----------------------------------------------
// Class Delegate

template<typename TClass, typename TReturn = void>
class DelegateClass
{
private:

    typedef TReturn (TClass::*PMethod)(void);
    PMethod     m_classMethod;
    TClass*     m_classObject;

public:

    DelegateClass (TClass* _pObject, PMethod _pMethod)
    {
        m_classObject = _pObject;
        m_classMethod = _pMethod;
    }

    TReturn Call ()
    {
        return (m_classObject->*m_classMethod)();
    }
    TReturn operator() () { return Call(); }
};

template<typename TClass, typename TParam1, typename TReturn = void>
class DelegateClass1P
{
private:

    typedef TReturn (TClass::*PMethod)(TParam1);
    PMethod     m_classMethod;
    TClass*     m_classObject;

public:

    DelegateClass1P (TClass* _pObject, PMethod _pMethod)
    {
        m_classObject   = _pObject;
        m_classMethod   = _pMethod;
    }

    TReturn Call (TParam1 _oValue1)
    {
        return (m_classObject->*m_classMethod)(_oValue1);
    }
    TReturn operator() (TParam1 _oValue1) { return Call(_oValue1); }
};

template<typename TClass, typename TParam1, typename TParam2, typename TReturn = void>
class DelegateClass2P
{
private:

    typedef TReturn (TClass::*PMethod)(TParam1, TParam2);
    PMethod     m_classMethod;
    TClass*     m_classObject;

public:

    DelegateClass2P (TClass* _pObject, PMethod _pMethod)
    {
        m_classObject   = _pObject;
        m_classMethod   = _pMethod;
    }

    TReturn Call (TParam1 _oValue1, TParam2 _oValue2)
    {
        return (m_classObject->*m_classMethod)(_oValue1, _oValue2);
    }
    TReturn operator() (TParam1 _oValue1, TParam2 _oValue2) { return Call(_oValue1, _oValue2); }
};

//----------------------------------------------
// Static Delegate

template<typename TReturn = void>
class DelegateStatic
{
private:

    typedef TReturn (*PMethod)(void);
    PMethod     m_staticMethod;

public:

    DelegateStatic (PMethod _pMethod)
    {
        m_staticMethod = _pMethod;
    }

    TReturn Call ()
    {
        return (m_staticMethod)();
    }
    TReturn operator() () { return Call(); }
};

template<typename TParam1, typename TReturn = void>
class DelegateStatic1P
{
private:

    typedef TReturn (*PMethod)(TParam1);
    PMethod     m_staticMethod;

public:

    DelegateStatic1P (PMethod _pMethod)
    {
        m_staticMethod  = _pMethod;
    }

    TReturn Call (TParam1 _oValue1)
    {
        return (m_staticMethod)(_oValue1);
    }
    TReturn operator() (TParam1 _oValue1) { return Call(_oValue1); }
};

template<typename TParam1, typename TParam2, typename TReturn = void>
class DelegateStatic2P
{
private:

    typedef TReturn (*PMethod)(TParam1, TParam2);
    PMethod     m_staticMethod;

public:

    DelegateStatic2P (PMethod _pMethod)
    {
        m_staticMethod  = _pMethod;
    }

    TReturn Call (TParam1 _oValue1, TParam2 _oValue2)
    {
        return (m_staticMethod)(_oValue1, _oValue2);
    }
    TReturn operator() (TParam1 _oValue1, TParam2 _oValue2) { return Call(_oValue1, _oValue2); }
};

}   // namespace gugu
