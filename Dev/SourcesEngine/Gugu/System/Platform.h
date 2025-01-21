#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/System/FileInfo.h"

////////////////////////////////////////////////////////////////
// File Declarations

namespace gugu {

void WriteInConsole(const std::string& _strLine, bool printInIDE);
void WriteInConsoleEndline(const std::string& _strLine, bool printInIDE);
void WriteInFileEndline(const std::string& _strFileName, const std::string& _strLine);

void OpenFileExplorer(std::string_view path);
void OpenWebBrowser(std::string_view url);

void GetFiles(std::string_view rootPath_utf8, std::vector<FileInfo>& files, bool recursive);
void GetDirectories(std::string_view rootPath_utf8, std::vector<std::string>& directories, bool recursive);

bool DirectoryExists(std::string_view path_utf8);
bool FileExists(std::string_view path_utf8);

bool EnsureDirectoryExists(std::string_view path_utf8);

bool RemoveFile(std::string_view path_utf8);
bool RemoveTargetDirectory(std::string_view path_utf8);
bool RemoveDirectoryTree(std::string_view path_utf8);

}   // namespace gugu
