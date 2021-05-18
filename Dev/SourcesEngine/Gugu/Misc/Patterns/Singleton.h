#pragma once

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename T>
class Singleton
{
protected:

            Singleton   ()  {}
    virtual ~Singleton  ()  {}

public:

    static T* GetInstance()
    {
        if(!m_instance)
        {
            m_instance = new T;
        }
        return m_instance;
    }

    static void DeleteInstance()
    {
        if(m_instance)
        {
            delete m_instance ;
            m_instance = 0;
        }
    }

protected:

    static T* m_instance;
};

template <typename T> T* Singleton<T>::m_instance = 0;

}   // namespace gugu
