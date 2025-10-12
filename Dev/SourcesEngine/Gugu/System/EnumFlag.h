#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/TypeTraits.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename TEnumFlag>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right);

template<typename TEnumFlag, typename... TFlagArgs>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right, TFlagArgs... args);

template<typename TEnumFlag>
void SetFlag(TEnumFlag& reference, TEnumFlag flag);

template<typename TEnumFlag>
void UnsetFlag(TEnumFlag& reference, TEnumFlag flag);

template<typename TEnumFlag>
void ToggleFlag(TEnumFlag& reference, TEnumFlag flag);

template<typename TEnumFlag>
bool HasFlag(TEnumFlag reference, TEnumFlag flag);

template<typename TEnumFlag>
bool HasAnyFlag(TEnumFlag reference, TEnumFlag flag);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/System/EnumFlag.tpp"
