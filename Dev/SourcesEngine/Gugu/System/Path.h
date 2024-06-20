#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include <string>

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

// Constants.
namespace system
{
	const char PathSeparator = '/';
	const char ExtensionSeparator = '.';
}

// Convert all '\\' to '/', strip all "/xxx/.." parts, strip leading and trailing '/' if needed.
std::string NormalizePath(const std::string& path);
void NormalizePathSelf(std::string& path);

// Combine two given paths into a single path with a separator. The resulting path will be normalized.
std::string CombinePaths(std::string_view pathLeft, std::string_view pathRight);
void CombinePaths(std::string_view pathLeft, std::string_view pathRight, std::string& resultPath);

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

}   // namespace gugu
