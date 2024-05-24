#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename T>
void SafeDelete(T*& _pObject);

template<typename T>
void SafeDeleteArray(T*& _aObjects);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/System/Memory.tpp"
