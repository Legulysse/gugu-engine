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

template<typename T>
std::string ToString(const T& _tValue)
{
    // floats are better displayed this way than with std::to_string
    std::ostringstream os;
    os << _tValue;
    return os.str();
}

template<typename T>
std::string ToString(const T& _tValue, int precision)
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
    std::vector<std::string> vecArgs;
    vecArgs.reserve(sizeof...(_tArgs));
    gugu::impl::StringFormat_FormatArgs(vecArgs, _tArgs...);

    std::string strResult = _tValue;
    uint32 iSize = (uint32)vecArgs.size();  //TODO: Switch to 64bits ?
    iSize = (iSize < 100) ? iSize : 100;  // limit on 0-99 range : 2 chars + delimiters + ending = buffer size of 5 chars
    for (uint32 i = 0; i < iSize; ++i)
    {
        char strIndex[5];  // limit on 0-99 range : 2 chars + delimiters + ending = buffer size of 5 chars
        sprintf(strIndex, "{%u}", i);
        StdStringReplaceSelf(strResult, strIndex, vecArgs[i]);
    }
    return strResult;
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
void ClearStdVector(std::vector<T*>& _oVector)
{
    auto iteCurrent = _oVector.begin();
    auto iteEnd = _oVector.end();
    T* pElem = nullptr;

    while(iteCurrent != iteEnd)
    {
        pElem = *iteCurrent;
        ++iteCurrent;

        SafeDelete(pElem);
    }

    _oVector.clear();
}

template<typename T>
void ClearStdList(std::list<T*>& _oList)
{
    auto iteCurrent = _oList.begin();
    auto iteEnd = _oList.end();
    T* pElem = nullptr;

    while(iteCurrent != iteEnd)
    {
        pElem = *iteCurrent;
        ++iteCurrent;

        SafeDelete(pElem);
    }

    _oList.clear();
}

template<typename TKey, typename TValue>
void ClearStdMap(std::map<TKey, TValue*>& _oMap)
{
    auto iteCurrent = _oMap.begin();
    auto iteEnd = _oMap.end();
    TValue* pElem = nullptr;

    while(iteCurrent != iteEnd)
    {
        pElem = iteCurrent->second;
        ++iteCurrent;

        SafeDelete(pElem);
    }

    _oMap.clear();
}

template<typename T>
typename std::vector<T>::const_iterator StdVectorFind(const std::vector<T>& _vecContainer, const T& _tValue)
{
    return std::find(_vecContainer.cbegin(), _vecContainer.cend(), _tValue);
}

template<typename T>
bool StdVectorContains(const std::vector<T>& _vecContainer, const T& _tValue)
{
    return std::find(_vecContainer.begin(), _vecContainer.end(), _tValue) != _vecContainer.end();
}

template<typename T>
void StdVectorPushFront(std::vector<T>& _vecContainer, const T& _tValue)
{
    _vecContainer.insert(_vecContainer.begin(), _tValue);
}

template<typename T>
void StdVectorInsertAt(std::vector<T>& _vecContainer, size_t _iIndex, const T& _tValue)
{
    _vecContainer.insert(_vecContainer.begin() + _iIndex, _tValue);
}

template<typename T>
void StdVectorRemove(std::vector<T>& _vecContainer, const T& _tValue)
{
    _vecContainer.erase(std::remove(_vecContainer.begin(), _vecContainer.end(), _tValue), _vecContainer.end());
}

template<typename T>
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex)
{
    _vecContainer.erase(_vecContainer.begin() + _iIndex);
}

template<typename T>
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex, size_t count)
{
    _vecContainer.erase(_vecContainer.begin() + _iIndex, _vecContainer.begin() + _iIndex + count);
}

template<typename T, typename P>
void StdVectorRemoveIf(std::vector<T>& _vecContainer, P _tPredicate)
{
    _vecContainer.erase(std::remove_if(_vecContainer.begin(), _vecContainer.end(), _tPredicate), _vecContainer.end());
}

template<typename T1, typename T2>
void StdVectorAppend(const std::vector<T1>& _vecFrom, std::vector<T2>& _vecTo)
{
    _vecTo.insert(_vecTo.end(), _vecFrom.begin(), _vecFrom.end());
}

template<typename T>
void StdVectorDifference(const std::vector<T>& _vecFrom, const std::vector<T>& _vecSubset, std::vector<T>& _vecTo)
{
    //This will sort the containers and remove the duplicates
    std::set<T> setContainer(_vecFrom.begin(), _vecFrom.end());
    std::set<T> setSubset(_vecSubset.begin(), _vecSubset.end());

    //Compute the difference
    _vecTo.clear();
    std::set_difference(setContainer.begin(), setContainer.end(), setSubset.begin(), setSubset.end(), std::back_inserter(_vecTo));
}

template<typename T>
void StdVectorIntersection(const std::vector<T>& _vecContainerA, const std::vector<T>& _vecContainerB, std::vector<T>& _vecIntersection)
{
    //This will sort the containers and remove the duplicates
    std::set<T> setContainerA(_vecContainerA.begin(), _vecContainerA.end());
    std::set<T> setContainerB(_vecContainerB.begin(), _vecContainerB.end());

    //Compute the difference
    _vecIntersection.clear();
    std::set_intersection(setContainerA.begin(), setContainerA.end(), setContainerB.begin(), setContainerB.end(), std::back_inserter(_vecIntersection));
}

template<typename T>
bool StdSetContains(const std::set<T>& _vecContainer, const T& _tValue)
{
    return std::find(_vecContainer.begin(), _vecContainer.end(), _tValue) != _vecContainer.end();
}

}   // namespace gugu
