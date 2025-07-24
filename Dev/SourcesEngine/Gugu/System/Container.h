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
void ClearStdVector(std::vector<T*>& container);

template<typename T>
void ClearStdList(std::list<T*>& container);

template<typename TKey, typename TValue>
void ClearStdMap(std::map<TKey, TValue*>& container);

template<typename T>
size_t StdVectorIndexOf(const std::vector<T>& container, const T& value);

template<typename T>
typename std::vector<T>::const_iterator StdVectorFind(const std::vector<T>& container, const T& value);

template<typename T>
bool StdVectorContains(const std::vector<T>& container, const T& value);

template<typename T>
void StdVectorPushFront(std::vector<T>& container, const T& value);

template<typename T>
void StdVectorInsertAt(std::vector<T>& container, size_t index, const T& value);

template<typename T>
void StdVectorRemove(std::vector<T>& container, const T& value);

template<typename T>
void StdVectorRemoveFirst(std::vector<T>& container, const T& value);

template<typename T>
void StdVectorRemoveAt(std::vector<T>& container, size_t index);

template<typename T>
void StdVectorRemoveAt(std::vector<T>& container, size_t index, size_t count);

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
void StdVectorAppend(const std::vector<T1>& containerFrom, std::vector<T2>& containerTo);

template<typename T>
void StdVectorDifference(const std::vector<T>& containerFrom, const std::vector<T>& containerSubset, std::vector<T>& containerTo);

template<typename T>
void StdVectorIntersection(const std::vector<T>& containerA, const std::vector<T>& containerB, std::vector<T>& intersection);

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
