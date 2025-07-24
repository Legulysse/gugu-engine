#pragma once

////////////////////////////////////////////////////////////////
// File Implementation

namespace gugu {
    
template<typename T>
void ClearStdVector(std::vector<T*>& container)
{
    auto end = container.end();
    for (auto it = container.begin(); it != end; ++it)
    {
        SafeDelete(*it);
    }

    container.clear();
}

template<typename T>
void ClearStdList(std::list<T*>& container)
{
    auto end = container.end();
    for (auto it = container.begin(); it != end; ++it)
    {
        SafeDelete(*it);
    }

    container.clear();
}

template<typename TKey, typename TValue>
void ClearStdMap(std::map<TKey, TValue*>& container)
{
    auto end = container.end();
    for (auto it = container.begin(); it != end; ++it)
    {
        SafeDelete(it->second);
    }

    container.clear();
}

template<typename T>
size_t StdVectorIndexOf(const std::vector<T>& container, const T& value)
{
    auto it = std::find(container.begin(), container.end(), value);
    if (it == container.end())
    {
        return system::InvalidIndex;
    }
    else
    {
        return it - container.begin();
    }
}

template<typename T>
typename std::vector<T>::const_iterator StdVectorFind(const std::vector<T>& container, const T& value)
{
    // const_iterator is mandatory because we receive the container as a const ref.
    return std::find(container.begin(), container.end(), value);
}

template<typename T>
bool StdVectorContains(const std::vector<T>& container, const T& value)
{
    return std::find(container.begin(), container.end(), value) != container.end();
}

template<typename T>
void StdVectorPushFront(std::vector<T>& container, const T& value)
{
    container.insert(container.begin(), value);
}

template<typename T>
void StdVectorInsertAt(std::vector<T>& container, size_t index, const T& value)
{
    container.insert(container.begin() + index, value);
}

template<typename T>
void StdVectorRemove(std::vector<T>& container, const T& value)
{
    container.erase(std::remove(container.begin(), container.end(), value), container.end());
}

//TODO: why const ?
template<typename T>
void StdVectorRemoveFirst(std::vector<T>& container, const T& value)
{
    auto it = std::find(container.begin(), container.end(), value);
    if (it != container.end())
    {
        container.erase(it);
    }
}

template<typename T>
void StdVectorRemoveAt(std::vector<T>& container, size_t index)
{
    container.erase(container.begin() + index);
}

template<typename T>
void StdVectorRemoveAt(std::vector<T>& container, size_t index, size_t count)
{
    container.erase(container.begin() + index, container.begin() + index + count);
}

template<typename T, typename P>
void StdVectorRemoveIf(std::vector<T>& container, const P& predicate)
{
    container.erase(std::remove_if(container.begin(), container.end(), predicate), container.end());
}

template<typename T>
void StdVectorRemoveIfNull(std::vector<T>& container)
{
    container.erase(std::remove(container.begin(), container.end(), nullptr), container.end());
}

template<typename T, typename P>
void StdVectorDeleteIf(std::vector<T>& container, const P& predicate)
{
    auto end = container.end();
    for (auto it = container.begin(); it != end; ++it)
    {
        if (predicate(*it))
        {
            SafeDelete(*it);
        }
    }
}

template<typename T, typename P>
void StdVectorDeleteAndRemoveIf(std::vector<T>& container, const P& predicate)
{
    StdVectorDeleteIf(container, predicate);
    StdVectorRemoveIfNull(container);
}

template<typename T1, typename T2>
void StdVectorAppend(const std::vector<T1>& containerFrom, std::vector<T2>& containerTo)
{
    containerTo.insert(containerTo.end(), containerFrom.begin(), containerFrom.end());
}

template<typename T>
void StdVectorDifference(const std::vector<T>& containerFrom, const std::vector<T>& containerSubset, std::vector<T>& containerTo)
{
    //This will sort the containers and remove the duplicates
    std::set<T> setContainer(containerFrom.begin(), containerFrom.end());
    std::set<T> setSubset(containerSubset.begin(), containerSubset.end());

    //Compute the difference
    containerTo.clear();    // TODO: remove ?
    std::set_difference(setContainer.begin(), setContainer.end(), setSubset.begin(), setSubset.end(), std::back_inserter(containerTo));
}

template<typename T>
void StdVectorIntersection(const std::vector<T>& containerA, const std::vector<T>& containerB, std::vector<T>& intersection)
{
    //This will sort the containers and remove the duplicates
    std::set<T> setContainerA(containerA.begin(), containerA.end());
    std::set<T> setContainerB(containerB.begin(), containerB.end());

    //Compute the difference
    intersection.clear();    // TODO: remove ?
    std::set_intersection(setContainerA.begin(), setContainerA.end(), setContainerB.begin(), setContainerB.end(), std::back_inserter(intersection));
}

template<typename T>
bool StdSetContains(const std::set<T>& container, const T& value)
{
    return container.find(value) != container.end();
}

template<typename TKey, typename TValue>
bool StdMapContainsKey(const std::map<TKey, TValue>& container, const TKey& key)
{
    return container.find(key) != container.end();
}

template<typename TKey, typename TValue>
bool StdMapTryGetValue(const std::map<TKey, TValue>& container, const TKey& key, TValue& value)
{
    auto it = container.find(key);
    if (it != container.end())
    {
        value = it->second;
        return true;
    }

    return false;
}

template<typename TKey, typename TValue>
bool StdMapTryGetValueConstRef(const std::map<TKey, TValue>& container, const TKey& key, const TValue*& valueRef)
{
    auto it = container.find(key);
    if (it != container.end())
    {
        valueRef = &it->second;
        return true;
    }

    return false;
}

template<typename TKey, typename TValue>
bool StdMapTryGetValueRef(std::map<TKey, TValue>& container, const TKey& key, TValue*& valueRef)
{
    auto it = container.find(key);
    if (it != container.end())
    {
        valueRef = &it->second;
        return true;
    }

    return false;
}

}   // namespace gugu
