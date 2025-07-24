#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/Memory.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Constants.
namespace system
{
	const size_t InvalidIndex = (size_t)-1;
}

template<typename T>
void ClearStdVector(std::vector<T*>& _oVector);

template<typename T>
void ClearStdList(std::list<T*>& _oList);

template<typename TKey, typename TValue>
void ClearStdMap(std::map<TKey, TValue*>& _oMap);

template<typename T>
size_t StdVectorIndexOf(const std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
typename std::vector<T>::const_iterator StdVectorFind(const std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
bool StdVectorContains(const std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
void StdVectorPushFront(std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
void StdVectorInsertAt(std::vector<T>& _vecContainer, size_t _iIndex, const T& _tValue);

template<typename T>
void StdVectorRemove(std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
void StdVectorRemoveFirst(std::vector<T>& _vecContainer, const T& _tValue);

template<typename T>
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex);

template<typename T>
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex, size_t count);

template<typename T, typename P>
void StdVectorRemoveIf(std::vector<T>& container, const P& predicate);

template<typename T>
void StdVectorRemoveIfNull(std::vector<T>& container);

// Apply the delete operation on entries targeted by the predicate.
template<typename T, typename P>
void StdVectorDeleteIf(std::vector<T>& container, const P& predicate);

// Apply the delete operation on entries targeted by the predicate, then remove all null entries.
// Warning: This implies that existing null entries will also be removed.
template<typename T, typename P>
void StdVectorDeleteAndRemoveIf(std::vector<T>& container, const P& predicate);

template<typename T1, typename T2>
void StdVectorAppend(const std::vector<T1>& _vecFrom, std::vector<T2>& _vecTo);

template<typename T>
void StdVectorDifference(const std::vector<T>& _vecFrom, const std::vector<T>& _vecSubset, std::vector<T>& _vecTo);

template<typename T>
void StdVectorIntersection(const std::vector<T>& _vecContainerA, const std::vector<T>& _vecContainerB, std::vector<T>& _vecIntersection);

template<typename T>
bool StdSetContains(const std::set<T>& container, const T& value);

template<typename TKey, typename TValue>
bool StdMapContainsKey(const std::map<TKey, TValue>& container, const TKey& key);

template<typename TKey, typename TValue>
bool StdMapTryGetValue(const std::map<TKey, TValue>& container, const TKey& key, TValue& value);

template<typename TKey, typename TValue>
bool StdMapTryGetValueConstRef(const std::map<TKey, TValue>& container, const TKey& key, const TValue*& valueRef);

template<typename TKey, typename TValue>
bool StdMapTryGetValueRef(std::map<TKey, TValue>& container, const TKey& key, TValue*& valueRef);

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/System/Container.tpp"
