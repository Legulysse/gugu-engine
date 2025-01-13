#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
template<typename T>
std::string ToString(const T& _tValue)
{
    // floats are better displayed this way than with std::to_string
    std::ostringstream os;
    os << _tValue;
    return os.str();
}

template<typename T>
std::string ToStringf(const T& _tValue, int precision)
{
    // floats are better displayed this way than with std::to_string
    std::ostringstream os;
    os.precision(precision);
    os << std::fixed;
    os << _tValue;
    return os.str();
}

template<typename T>
bool FromString(const std::string& _strValue, T& _tValue)
{
    std::istringstream iss( _strValue );
    return (iss >> _tValue) ? true : false;
}

template<typename T>
T FromString(const std::string& _strValue)
{
    std::istringstream iss( _strValue );
    T tValue;
    iss >> tValue;  // std::boolalpha could be used
    return tValue;
}

template<typename T1>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1)
{
    std::string strResult = _tValue;
    StdStringReplaceSelf(strResult, "{0}", ToString(_tArg1));
    return strResult;
}

template<typename T1, typename T2>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2)
{
    std::string strResult = _tValue;
    StdStringReplaceSelf(strResult, "{0}", ToString(_tArg1));
    StdStringReplaceSelf(strResult, "{1}", ToString(_tArg2));
    return strResult;
}

template<typename T1, typename T2, typename T3>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2, const T3& _tArg3)
{
    std::string strResult = _tValue;
    StdStringReplaceSelf(strResult, "{0}", ToString(_tArg1));
    StdStringReplaceSelf(strResult, "{1}", ToString(_tArg2));
    StdStringReplaceSelf(strResult, "{2}", ToString(_tArg3));
    return strResult;
}

template<typename T1, typename T2, typename T3, typename T4>
std::string StringFormat(const std::string& _tValue, const T1& _tArg1, const T2& _tArg2, const T3& _tArg3, const T4& _tArg4)
{
    std::string strResult = _tValue;
    StdStringReplaceSelf(strResult, "{0}", ToString(_tArg1));
    StdStringReplaceSelf(strResult, "{1}", ToString(_tArg2));
    StdStringReplaceSelf(strResult, "{2}", ToString(_tArg3));
    StdStringReplaceSelf(strResult, "{3}", ToString(_tArg4));
    return strResult;
}

namespace impl
{
    template<typename T>
    void StringFormat_FormatArgs(std::vector<std::string>& _vecArgs, const T& _tArg)
    {
        _vecArgs.push_back(ToString(_tArg));
    }

    template<typename T, typename... TArgs>
    void StringFormat_FormatArgs(std::vector<std::string>& _vecArgs, const T& _tArg, const TArgs&... _tArgs)
    {
        _vecArgs.push_back(ToString(_tArg));
        StringFormat_FormatArgs(_vecArgs, _tArgs...);
    }
}

template<typename... TArgs>
std::string StringFormat(const std::string& _tValue, const TArgs&... _tArgs)
{
    std::vector<std::string> args_str;
    args_str.reserve(sizeof...(_tArgs));
    gugu::impl::StringFormat_FormatArgs(args_str, _tArgs...);

    std::string result = _tValue;
    size_t argCount = args_str.size();
    argCount = (argCount < 100) ? argCount : 100;  // limit on 0-99 range : 2 chars + delimiters + ending = buffer size of 5 chars
    for (size_t i = 0; i < argCount; ++i)
    {
        char indexToken[5];  // limit on 0-99 range : 2 chars + delimiters + ending = buffer size of 5 chars
        sprintf(indexToken, "{%zu}", i);
        StdStringReplaceSelf(result, indexToken, args_str[i]);
    }

    return result;
}

template<typename T>
void FormatParameters::Add(const std::string& strName, const T& tParam)
{
    std::string strNameParam = "{}";
    strNameParam.insert(1, strName);
    m_parameters[strNameParam] = ToString(tParam);
}

template<typename T>
std::string StringNumberFormat(const T& value, size_t leadingZeros, const std::string& delimiter)
{
    std::string formattedValue = ToString(value);
    StringNumberFormatSelf(formattedValue, leadingZeros, delimiter);
    return formattedValue;
}

template<typename T>
std::string StringNumberFormatf(const T& value, size_t leadingZeros, int precision, const std::string& delimiter)
{
    std::string formattedValue = ToStringf(value, precision);
    StringNumberFormatSelf(formattedValue, leadingZeros, delimiter);
    return formattedValue;
}

}   // namespace gugu
