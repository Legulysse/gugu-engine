#pragma once

////////////////////////////////////////////////////////////////
// Includes

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

template<typename T>
constexpr bool Constexpr_IsEnumClass = std::is_enum_v<T> && !std::is_convertible_v<T, int>;

}   // namespace gugu
