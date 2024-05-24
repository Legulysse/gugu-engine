#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
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
size_t StdVectorIndexOf(const std::vector<T>& _vecContainer, const T& _tValue)
{
    auto it = std::find(_vecContainer.begin(), _vecContainer.end(), _tValue);
    if (it == _vecContainer.end())
    {
        return system::InvalidIndex;
    }
    else
    {
        return it - _vecContainer.begin();
    }
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
void StdVectorRemoveFirst(std::vector<T>& _vecContainer, const T& _tValue)
{
    typename std::vector<T>::const_iterator it = std::find(_vecContainer.cbegin(), _vecContainer.cend(), _tValue);
    if (it != _vecContainer.cend())
    {
        _vecContainer.erase(it);
    }
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
bool StdSetContains(const std::set<T>& container, const T& value)
{
    return std::find(container.begin(), container.end(), value) != container.end();
}

template<typename TKey, typename TValue>
bool StdMapTryGet(const std::map<TKey, TValue>& container, const TKey& key, TValue& value)
{
    auto it = container.find(key);
    if (it != container.end())
    {
        value = it->second;
        return true;
    }

    return false;
}

}   // namespace gugu
