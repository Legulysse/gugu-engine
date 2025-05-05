#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TEnumFlag>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) | static_cast<std::underlying_type_t<TEnumFlag>>(right));
    }
    else
    {
        return (left | right);
    }
}

template<typename TEnumFlag, typename... TFlagArgs>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right, TFlagArgs... args)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        return CombineFlags(static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) | static_cast<std::underlying_type_t<TEnumFlag>>(right)), args...);
    }
    else
    {
        return CombineFlags((left | right), args...);
    }
}

template<typename TEnumFlag>
void SetFlag(TEnumFlag& reference, TEnumFlag flag)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        reference = static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) | static_cast<std::underlying_type_t<TEnumFlag>>(flag));
    }
    else
    {
        reference = (reference | flag);
    }
}

template<typename TEnumFlag>
void UnsetFlag(TEnumFlag& reference, TEnumFlag flag)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        reference = static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) & ~static_cast<std::underlying_type_t<TEnumFlag>>(flag));
    }
    else
    {
        reference = (reference & ~flag);
    }
}

template<typename TEnumFlag>
void ToggleFlag(TEnumFlag& reference, TEnumFlag flag)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        reference = static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) ^ static_cast<std::underlying_type_t<TEnumFlag>>(flag));
    }
    else
    {
        reference = (reference ^ flag);
    }
}

template<typename TEnumFlag>
bool HasFlag(TEnumFlag reference, TEnumFlag flag)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        return (static_cast<std::underlying_type_t<TEnumFlag>>(reference) & static_cast<std::underlying_type_t<TEnumFlag>>(flag)) == static_cast<std::underlying_type_t<TEnumFlag>>(flag);
    }
    else
    {
        return (reference & flag) == flag;
    }
}

template<typename TEnumFlag>
bool HasAnyFlag(TEnumFlag reference, TEnumFlag flag)
{
    if constexpr (Constexpr_IsEnumClass<TEnumFlag>)
    {
        return (static_cast<std::underlying_type_t<TEnumFlag>>(reference) & static_cast<std::underlying_type_t<TEnumFlag>>(flag)) != 0;
    }
    else
    {
        return (reference & flag) != 0;
    }
}

//template<typename TEnumFlag>
//TEnumFlag operator | (TEnumFlag left, TEnumFlag right)
//{
//    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) | static_cast<std::underlying_type_t<TEnumFlag>>(right));
//}
//
//template<typename TEnumFlag>
//TEnumFlag operator & (TEnumFlag left, TEnumFlag right)
//{
//    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) & static_cast<std::underlying_type_t<TEnumFlag>>(right));
//}
//
//template<typename TEnumFlag>
//TEnumFlag operator ~ (TEnumFlag right)
//{
//    return static_cast<TEnumFlag>(~static_cast<std::underlying_type_t<TEnumFlag>>(right));
//}

}   // namespace gugu
