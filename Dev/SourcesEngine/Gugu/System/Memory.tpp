#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
template<typename T>
void SafeDelete(T*& _pObject)
{
    if(_pObject)
    {
        delete _pObject;
        _pObject = nullptr;
    }
}

template<typename T>
void SafeDeleteArray(T*& _aObjects)
{
    if(_aObjects)
    {
        delete[] _aObjects;
        _aObjects = nullptr;
    }
}

}   // namespace gugu
