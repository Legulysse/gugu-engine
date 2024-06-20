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

void OpenFileExplorer(const std::string& path);
void OpenWebBrowser(const std::string& _strURL);

void GetFiles(const std::string& rootPath_utf8, std::vector<FileInfo>& files, bool recursive);
void GetDirectories(const std::string& rootPath_utf8, std::vector<std::string>& directories, bool recursive);

bool DirectoryExists(std::string_view path_utf8);
bool FileExists(std::string_view path_utf8);

bool EnsureDirectoryExists(std::string_view path_utf8);

bool RemoveFile(std::string_view path_utf8);
bool RemoveTargetDirectory(std::string_view path_utf8);
bool RemoveDirectoryTree(std::string_view path_utf8);

}   // namespace gugu
