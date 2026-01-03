#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Types.h"
#include "Gugu/System/TypeTraits.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

bool StringEquals(std::string_view left, std::string_view right);

template<typename T>
std::string ToString(const T& value);
template<>
std::string ToString<bool>(const bool& value);
std::string ToString(int value);
std::string ToString(const char* value);

template<typename T>
std::string ToStringf(const T& value, int precision);

template<typename T>
bool TryFromString(const std::string& value, T& result);
template<>
bool TryFromString<bool>(const std::string& value, bool& result);

template<typename T>
T FromString(const std::string& value, const T& defaultValue);
template<>
bool FromString<bool>(const std::string& value, const bool& defaultValue);

std::string StdStringReplace(const std::string& _strValue, const std::string& _strFrom, const std::string& _strTo);
std::string StdStringReplace(const std::string& value, const char& from, const char& to);
void StdStringReplaceSelf(std::string& _strValue, const std::string& _strFrom, const std::string& _strTo);
void StdStringReplaceSelf(std::string& value, const char& from, const char& to);

void StdStringSplit(std::string_view _strValue, std::string_view _strDelimiter, std::vector<std::string>& _vecTokens);
void StdStringSplit(std::string_view _strValue, char _strDelimiter, std::vector<std::string>& _vecTokens);

std::string StdStringToLower(const std::string& _strValue);
void StdStringToLowerSelf(std::string& _strValue);

std::string StdStringToUpper(const std::string& _strValue);
void StdStringToUpperSelf(std::string& _strValue);

bool StdStringStartsWith(std::string_view _strValue, std::string_view _strSub);
bool StdStringEndsWith(std::string_view _strValue, std::string_view _strSub);
bool StdStringStartsWithInsensitive(std::string_view _strValue, std::string_view _strSub);
bool StdStringEndsWithInsensitive(std::string_view _strValue, std::string_view _strSub);

std::string StringFormat(const std::string& _tValue);
template<typename T1>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1);
template<typename T1, typename T2>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2);
template<typename T1, typename T2, typename T3>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2, const T3& _tArg3);
template<typename T1, typename T2, typename T3, typename T4>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2, const T3& _tArg3, const T4& _tArg4);
template<typename... TArgs>
std::string StringFormat(const std::string& _tValue, const TArgs&... _tArgs);

struct FormatParameters     // TODO: rename as StringFormatParameters ? or StringFormatArgs ?
{
    template<typename T>
    void Add(const std::string& strName, const T& tParam);
    const std::map<std::string, std::string>& GetParameters() const;

private:
    std::map<std::string, std::string> m_parameters;
};

std::string StringFormat(const std::string& _tValue, const FormatParameters& Params);

template<typename T>
std::string StringNumberFormat(const T& value, size_t leadingZeros, const std::string& delimiter = " ");
template<typename T>
std::string StringNumberFormatf(const T& value, size_t leadingZeros, int precision, const std::string& delimiter = " ");
std::string StringNumberFormat(const std::string& value, size_t leadingZeros, const std::string& delimiter = " ");
void StringNumberFormatSelf(std::string& value, size_t leadingZeros, const std::string& delimiter = " ");

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/System/String.tpp"
