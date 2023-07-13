#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Common.h"
#include "Gugu/System/Types.h"
#include "Gugu/System/FileInfo.h"

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

//----------------------------------------------
// Constants

namespace system
{
    const size_t InvalidIndex = (size_t)-1;
    const char PathSeparator = '/';
    const char ExtensionSeparator = '.';
}

//----------------------------------------------
// General

template<typename T>
void SafeDelete(T*& _pObject);

template<typename T>
void SafeDeleteArray(T*& _aObjects);

//----------------------------------------------
// Strings

bool StringEquals(std::string_view left, std::string_view right);

template<typename T>
std::string ToString(const T& _tValue);

template<typename T>
std::string ToString(const T& _tValue, int precision);

std::string ToString(int _iValue);
std::string ToString(const char* _strValue);

template<typename T>
bool FromString(const std::string& _strValue, T& _tValue);

template<typename T>
T FromString(const std::string& _strValue);

std::string StdStringReplace(const std::string& _strValue, const std::string& _strFrom, const std::string& _strTo);
void StdStringReplaceSelf(std::string& _strValue, const std::string& _strFrom, const std::string& _strTo);

std::string StdStringReplace(const std::string& value, const char& from, const char& to);
void StdStringReplaceSelf(std::string& value, const char& from, const char& to);

void StdStringSplit(std::string_view _strValue, std::string_view _strDelimiter, std::vector<std::string>& _vecTokens);
void StdStringSplit(std::string_view _strValue, char _strDelimiter, std::vector<std::string>& _vecTokens);

std::string StdStringToLower(const std::string& _strValue);
void StdStringToLowerSelf(std::string& _strValue);

std::string StdStringToUpper(const std::string& _strValue);
void StdStringToUpperSelf(std::string& _strValue);

bool StdStringStartsWith(std::string_view _strValue, std::string_view _strSub);
bool StdStringEndsWith(std::string_view _strValue, std::string_view _strSub);

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
std::string StringNumberFormat(const T& value, size_t leadingZeros = 0, const std::string& delimiter = " ");
std::string StringNumberFormat(const std::string& value, size_t leadingZeros = 0, const std::string& delimiter = " ");
void StringNumberFormatSelf(std::string& value, size_t leadingZeros = 0, const std::string& delimiter = " ");

//----------------------------------------------
// Paths

// Convert all '\\' to '/', strip all "/xxx/.." parts, strip leading and trailing '/' if needed.
std::string NormalizePath(const std::string& path);
void NormalizePathSelf(std::string& path);

// Combine two given paths into a single path with a separator. The resulting path will be normalized.
std::string CombinePaths(const std::string& pathLeft, const std::string& pathRight);
void CombinePaths(const std::string& pathLeft, const std::string& pathRight, std::string& resultPath);

// Simply append a trailing path separator '/' if there is none already. The given path is considered as normalized.
std::string EnsureTrailingPathSeparator(const std::string& path);
void EnsureTrailingPathSeparatorSelf(std::string& path);

// Return the directories path part of a given pathFile. The given path is considered as normalized.
std::string DirectoryPartFromPath(const std::string& pathFile);
void DirectoryPartFromPathSelf(std::string& pathFile);

// Return the file name part of a given pathFile. The given path is considered as normalized.
std::string NamePartFromPath(const std::string& pathFile);
void NamePartFromPathSelf(std::string& pathFile);

// Check if a given path starts with a given subPath. Both paths are considered as complete and normalized.
bool PathStartsWith(std::string_view path, std::string_view subPath);

//----------------------------------------------
// Containers

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
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex);

template<typename T>
void StdVectorRemoveAt(std::vector<T>& _vecContainer, size_t _iIndex, size_t count);

template<typename T, typename P>
void StdVectorRemoveIf(std::vector<T>& _vecContainer, P _tPredicate);

template<typename T1, typename T2>
void StdVectorAppend(const std::vector<T1>& _vecFrom, std::vector<T2>& _vecTo);

template<typename T>
void StdVectorDifference(const std::vector<T>& _vecFrom, const std::vector<T>& _vecSubset, std::vector<T>& _vecTo);

template<typename T>
void StdVectorIntersection(const std::vector<T>& _vecContainerA, const std::vector<T>& _vecContainerB, std::vector<T>& _vecIntersection);

template<typename T>
bool StdSetContains(const std::set<T>& _vecContainer, const T& _tValue);

//----------------------------------------------
// OS

void WriteInConsole(const std::string& _strLine, bool printInIDE);
void WriteInConsoleEndline(const std::string& _strLine, bool printInIDE);
void WriteInFileEndline(const std::string& _strFileName, const std::string& _strLine);

void OpenFileExplorer(const std::string& path);
void OpenWebBrowser(const std::string& _strURL);

void GetFiles(const std::string& rootPath, std::vector<FileInfo>& files, bool recursive);
void GetDirectories(const std::string& rootPath, std::vector<std::string>& directories, bool recursive);

bool DirectoryExists(const std::string& path);
bool FileExists(const std::string& path);

bool EnsureDirectoryExists(const std::string& _strPath);

bool RemoveFile(const std::string& path);
bool RemoveTargetDirectory(const std::string& path);
bool RemoveDirectoryTree(const std::string& path);

// Get the current timestamp as milliseconds
int64 GetTimestamp();
std::string GetTimestampAsString();

}   // namespace gugu

////////////////////////////////////////////////////////////////
// Template Implementation

#include "Gugu/System/SystemUtility.tpp"
