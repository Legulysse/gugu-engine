#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <type_traits>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename T>
constexpr bool Constexpr_IsEnumClass = std::is_enum_v<T> && !std::is_convertible_v<T, int>;

template<typename T>
constexpr bool Constexpr_IsPointer = std::is_pointer_v<T>;

}   // namespace gugu
