#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

template<typename TEnumFlag>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right)
{
    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) | static_cast<std::underlying_type_t<TEnumFlag>>(right));
}

template<typename TEnumFlag, typename... TFlagArgs>
TEnumFlag CombineFlags(TEnumFlag left, TEnumFlag right, TFlagArgs... args)
{
    return CombineFlags(static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(left) | static_cast<std::underlying_type_t<TEnumFlag>>(right)), args...);
}

template<typename TEnumFlag>
TEnumFlag SetFlag(TEnumFlag reference, TEnumFlag flag)
{
    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) | static_cast<std::underlying_type_t<TEnumFlag>>(flag));
}

template<typename TEnumFlag>
TEnumFlag UnsetFlag(TEnumFlag reference, TEnumFlag flag)
{
    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) & ~static_cast<std::underlying_type_t<TEnumFlag>>(flag));
}

template<typename TEnumFlag>
TEnumFlag ToggleFlag(TEnumFlag reference, TEnumFlag flag)
{
    return static_cast<TEnumFlag>(static_cast<std::underlying_type_t<TEnumFlag>>(reference) ^ static_cast<std::underlying_type_t<TEnumFlag>>(flag));
}

template<typename TEnumFlag>
bool HasFlag(TEnumFlag reference, TEnumFlag flag)
{
    return (static_cast<std::underlying_type_t<TEnumFlag>>(reference) & static_cast<std::underlying_type_t<TEnumFlag>>(flag)) == static_cast<std::underlying_type_t<TEnumFlag>>(flag);
}

template<typename TEnumFlag>
bool HasAnyFlag(TEnumFlag reference, TEnumFlag flag)
{
    return (static_cast<std::underlying_type_t<TEnumFlag>>(reference) & static_cast<std::underlying_type_t<TEnumFlag>>(flag)) != 0;
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
