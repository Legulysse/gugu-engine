////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Gugu/System/String.h"

////////////////////////////////////////////////////////////////
// Includes

#include <algorithm>
#include <locale>

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {

bool StringEquals(std::string_view left, std::string_view right)
{
    return left == right;
}

std::string ToString(int _iValue)
{
    return std::to_string(_iValue);
}

std::string ToString(const char* _strValue)
{
    return std::string(_strValue);
}

std::string StdStringReplace(const std::string& value, const std::string& from, const std::string& to)
{
    std::string result = value;
    StdStringReplaceSelf(result, from, to);
    return result;
}

void StdStringReplaceSelf(std::string& value, const std::string& from, const std::string& to)
 {
    size_t lengthFrom = from.length();
    size_t lengthTo = to.length();

    size_t pos = 0;
    while ((pos = value.find(from, pos)) != std::string::npos)
    {
        value.replace(pos, lengthFrom, to);
        pos += lengthTo;
    }
}

std::string StdStringReplace(const std::string& value, const char& from, const char& to)
{
    std::string result = value;
    StdStringReplaceSelf(result, from, to);
    return result;
}

void StdStringReplaceSelf(std::string& _strValue, const char& from, const char& to)
{
    std::replace(_strValue.begin(), _strValue.end(), from, to);
}

void StdStringSplit(std::string_view _strValue, std::string_view _strDelimiter, std::vector<std::string>& _vecTokens)
{
    _vecTokens.clear();

    if (_strValue.empty())
        return;

    size_t iStrLengthDelimiter = _strDelimiter.length();

    size_t iPos = 0;
    size_t iPosTokenStart = iPos;
    while ((iPos = _strValue.find(_strDelimiter, iPos)) != std::string::npos)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart, iPos - iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));

        iPos += iStrLengthDelimiter;
        iPosTokenStart = iPos;
    }

    size_t iPosEnd = _strValue.length()-1;
    if (iPosTokenStart <= iPosEnd)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));
    }
}

void StdStringSplit(std::string_view _strValue, char _strDelimiter, std::vector<std::string>& _vecTokens)
{
    _vecTokens.clear();

    if (_strValue.empty())
        return;

    size_t iStrLengthDelimiter = 1;

    size_t iPos = 0;
    size_t iPosTokenStart = iPos;
    while ((iPos = _strValue.find(_strDelimiter, iPos)) != std::string::npos)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart, iPos - iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));

        iPos += iStrLengthDelimiter;
        iPosTokenStart = iPos;
    }

    size_t iPosEnd = _strValue.length() - 1;
    if (iPosTokenStart <= iPosEnd)
    {
        std::string_view strToken = _strValue.substr(iPosTokenStart);
        if (!strToken.empty())
            _vecTokens.push_back(std::string(strToken));
    }
}

std::string StdStringToLower(const std::string& _strValue)
{
    std::string strResult = _strValue;
    StdStringToLowerSelf(strResult);
    return strResult;
}

void StdStringToLowerSelf(std::string& _strValue)
{
    std::locale loc;
    std::transform(_strValue.begin(), _strValue.end(), _strValue.begin(),
        [&loc](const unsigned char c) { return std::tolower(c, loc); });
}

std::string StdStringToUpper(const std::string& _strValue)
{
    std::string strResult = _strValue;
    StdStringToUpperSelf(strResult);
    return strResult;
}

void StdStringToUpperSelf(std::string& _strValue)
{
    std::locale loc;
    std::transform(_strValue.begin(), _strValue.end(), _strValue.begin(),
        [&loc](const unsigned char c) { return std::toupper(c, loc); });
}

bool StdStringStartsWith(std::string_view _strValue, std::string_view _strSub)
{
    return !_strSub.empty()
        && _strValue.size() >= _strSub.size()
        && std::equal(_strValue.begin(),
            _strValue.begin() + _strSub.size(),
            _strSub.begin());
}

bool StdStringEndsWith(std::string_view _strValue, std::string_view _strSub)
{
    return !_strSub.empty()
        && _strValue.size() >= _strSub.size()
        && std::equal(_strValue.begin() + _strValue.size() - _strSub.size(),
            _strValue.end(),
            _strSub.begin());
}

namespace impl
{
    bool TestCharEqualInsensitive(char a, char b)
    {
        std::locale loc;
        return std::toupper(a, loc) == std::toupper(b, loc);
    }
}

bool StdStringStartsWithInsensitive(std::string_view _strValue, std::string_view _strSub)
{
    return !_strSub.empty()
        && _strValue.size() >= _strSub.size()
        && std::equal(_strValue.begin(),
            _strValue.begin() + _strSub.size(),
            _strSub.begin(),
            impl::TestCharEqualInsensitive);
}

bool StdStringEndsWithInsensitive(std::string_view _strValue, std::string_view _strSub)
{
    return !_strSub.empty()
        && _strValue.size() >= _strSub.size()
        && std::equal(_strValue.begin() + _strValue.size() - _strSub.size(),
            _strValue.end(),
            _strSub.begin(),
            impl::TestCharEqualInsensitive);
}

std::string StringFormat(const std::string& _tValue)
{
    return _tValue;
}

const std::map<std::string, std::string>& FormatParameters::GetParameters() const
{
    return m_parameters;
}

std::string StringFormat(const std::string& _tValue, const FormatParameters& Params)
{
    std::string strResult = _tValue;
    for (auto kvp : Params.GetParameters())
    {
        StdStringReplaceSelf(strResult, kvp.first, kvp.second);
    }
    return strResult;
}

std::string StringNumberFormat(const std::string& value, size_t leadingZeros, const std::string& delimiter)
{
    std::string formattedValue = value;
    StringNumberFormatSelf(formattedValue, leadingZeros, delimiter);
    return formattedValue;
}

void StringNumberFormatSelf(std::string& value, size_t leadingZeros, const std::string& delimiter)
{
    if (value.empty())
    {
        return;
    }

    bool hasSign = value[0] == '-';
    size_t decimalSeparatorIndex = value.find('.');
    decimalSeparatorIndex = (decimalSeparatorIndex == std::string::npos ? value.find(',') : decimalSeparatorIndex);
    size_t fractionalPartOffset = (decimalSeparatorIndex == std::string::npos ? 0 : value.length() - decimalSeparatorIndex);

    // Add leading zeros
    size_t baseLength = value.length() + (hasSign ? -1 : 0) - fractionalPartOffset;
    size_t insertOffset = fractionalPartOffset;

    if (leadingZeros > baseLength)
    {
        size_t missingZeros = leadingZeros - baseLength;

        for (size_t i = 0; i < missingZeros; ++i)
        {
            value.insert(value.length() - insertOffset - baseLength - i, "0");
        }
    }

    // Add delimiter
    baseLength = value.length() + (hasSign ? -1 : 0) - fractionalPartOffset;
    insertOffset = fractionalPartOffset;

    size_t charIndex = 3;
    size_t addedLength = 0;
    while (charIndex < baseLength)
    {
        value.insert(value.length() - insertOffset - charIndex - addedLength, delimiter);

        addedLength += delimiter.length();
        charIndex += 3;
    }
}

}   // namespace gugu
